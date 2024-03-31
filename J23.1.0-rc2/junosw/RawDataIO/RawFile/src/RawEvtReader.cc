#include "RawFile/RawEvtReader.h"
#include <iostream>
#include <cstring>

RawEvtReader::RawEvtReader(const std::vector<std::string>& files)
    : m_files(files),
      m_ifile(0),
      m_fileHdrSize(256),
      m_fileHdr(nullptr),
      m_DPHdr(nullptr),
      m_cacheSize(16*1024*1024),
      m_leftEvt(0),
      m_leftSize(0),
      m_shift(0),
      m_dptr(nullptr)
{
    m_status = initFile( m_files[m_ifile] );
}

RawEvtReader::~RawEvtReader()
{
    delete[] m_fileHdr;
    delete[] m_DPHdr;
    delete[] m_dptr;
    if ( m_ifs.is_open() ) {
        m_ifs.close();
    }
}

bool RawEvtReader::readEvent(char*& addr, int& size/*in bytes*/)
{
    // - Handle DP_Header_Record:
    if ( (m_leftEvt == 0) && (m_leftSize == 0) ) {

        // - read DP_Header_Record from file:
        //m_ifs.read(m_DPHdr, m_DPHdrSize);
        if ( ! tryRead(m_DPHdr, m_DPHdrSize) ) {
            return false;
        }

        uint32_t* _dpHdr = (uint32_t*)m_DPHdr;
        //for ( int i = 0; i < (m_DPHdrSize/4); ++i ) std::cout << "0x" << std::hex << _dpHdr[i] << std::dec << std::endl;

        // - check the marker of DP_Header_Record:
        if ( _dpHdr[0] != 0xff1234ff ) {
            std::cerr << "RawEvtReader::readEvent() wrong DP_Header_Record marker 0x"
                << std::hex << _dpHdr[0] << std::dec << std::endl;
            m_status = false;
            return false;
        }

        // - reset m_leftEvt:
        m_leftEvt = _dpHdr[5];

        // - reset m_leftSize:
        m_leftSize = _dpHdr[3] - m_DPHdrSize;               // size of the DP_Fragment except the DP_Header_Record in bytes

        // - reset m_shift:
        m_shift = 0;                                        // DP_Header_Record's size in bytes

        // TODO get other information from DP_Header_Record if necessary

        // - read the whole Evt_Fragment from file:
        uint32_t dataSize = _dpHdr[3] - m_DPHdrSize;       // size of the DP_Fragment except the DP_Header_Record in bytes
        if ( dataSize > m_cacheSize ) {
            while(dataSize > m_cacheSize) m_cacheSize *= 2;
            delete[] m_dptr;
            m_dptr = new char[m_cacheSize];
        }

        if ( ! tryRead(m_dptr, dataSize) ) {
            return false;
        }
    }

    // - Handle RawEvt_Header_Record:
    if ( (m_leftEvt != 0) && (m_leftSize != 0) ) {

        uint32_t* _evtHdr = (uint32_t*)(m_dptr + m_shift);

        // - check the marker of RawEvt_Header_Record:
        if ( _evtHdr[0] != 0xee1234ee ) {
            std::cerr << "RawEvtReader::readEvent() wrong Event_Header_Record marker 0x"
                << std::hex << _evtHdr[0] << std::dec << std::endl;
            m_status = false;
            return false;
        }
        // TODO check other Event_Fragment markers if exsits

        // - set addr & size:
        size = _evtHdr[3];
        addr = new char[size];
        memcpy(addr, m_dptr+m_shift, size);

        // - set m_leftEvt:
        m_leftEvt--;

        // - set m_leftSize:
        m_leftSize -= size;

        // - set m_shift:
        m_shift += size;

        return true;
    }
    else {
        std::cerr << "RawEvtReader::readEvent() unmatched DP size" << std::endl;
        m_status = false;
    }

    return false;
}

bool RawEvtReader::skipEvent(int /*num*/)
{
    //TODO: if necessary in future...
    return true;
}

bool RawEvtReader::initFile(const std::string& file)
{
    if ( m_ifs.is_open() ) {
        m_ifs.close();
    }

    std::cout << "RawEvtReader now open: " << file << std::endl;

    m_ifs.open(file.c_str(), std::ios::binary);

    if ( ! m_ifs.is_open() ) {
        std::cerr << "RawEvtReader::initFile() failed to open: " << file << std::endl;
        m_status = false;
        return false;
    }

    // - Handle File_Start_Record:
    m_fileHdr = new char[m_fileHdrSize];
    uint32_t* _fhr = (uint32_t*)m_fileHdr;
    m_ifs.read(m_fileHdr, m_fileHdrSize);
    if ( m_ifs.good() && (_fhr[0] == 0x00000100) ) {
        m_version = _fhr[1];     // get the version
        if ( m_version == 0x00010001 ) {
            m_DPHdrSize = 24;
            m_DPHdr = new char[ m_DPHdrSize ];
        }
        m_dptr = new char[m_cacheSize];
    }
    else {
        std::cerr << "RawEvtReader::initFile() failed to init: " << file << std::endl;
        m_status = false;
        return false;
    }

    return true;
}

bool RawEvtReader::tryRead(char* dest, int size)
{
    if ( m_ifs.read(dest, size).good() ) {
        return true;
    }

    ++m_ifile;

    if ( m_ifile < m_files.size() && initFile( m_files[m_ifile] ) ) {
        return tryRead(dest, size);
    }

    if ( m_ifile != m_files.size() ) {
        std::cerr << "RawEvtReader::tryRead() failed to read data from file" << std::endl;
    }

    m_status = false;
    return false;
}
