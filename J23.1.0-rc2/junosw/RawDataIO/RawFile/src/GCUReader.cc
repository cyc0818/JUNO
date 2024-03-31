#include "RawFile/GCUReader.h"
#include <iostream>
#include <cstring>

GCUReader::GCUReader(const std::vector<std::string>& files)
    : m_files(files),
      m_ifile(0)
{
    m_status = initFile( m_files[m_ifile] );
}

GCUReader::~GCUReader()
{
    if ( m_ifs.is_open() ) {
        m_ifs.close();
    }
}

bool GCUReader::readEvent(char*& addr, int& size)
{
    char header[16];
    //m_ifs.read(header, 16);
    if ( ! tryRead(header, 16) ) {
        return false;
    }

    for ( int i = 0; i < 16; i += 2 ) {
        char tmp = header[i];
        header[i] = header[i+1];
        header[i+1] = tmp;
    }

    uint16_t* data = (uint16_t*)header;
    //for ( int i = 0; i < 8; ++i ) std::cout << "0x" << std::hex << data[i] << std::dec << std::endl;

    if ( data[0] != 0x805a ) {
        std::cerr << "GCUReader::readEvent() wrong header marker 0x"
            << std::hex << data[0] << std::dec << std::endl;
        m_status = false;
        return false;
    }

    size = data[2] * 8 * 2; //in bytes;

    //std::cout << "size " << size << std::endl;
    char* dptr = new char[size];

    memcpy(dptr, header, 16);

    //m_ifs.read(dptr+16, size-16);
    if ( ! tryRead(dptr+16, size-16) ) {
        return false;
    }

    for ( int i = 16; i < size; i += 2 ) {
        char tmp = dptr[i];
        dptr[i] = dptr[i+1];
        dptr[i+1] = tmp;
    }

    data = (uint16_t*)dptr;

    if ( data[size/2-1] != 0x8069 ) {
        //... check other markers?
        std::cerr << "GCUReader::readEvent() wrong trailer marker " << std::hex;
        for ( int i = (size/2 - 8); i < size/2; ++i ) {
            std::cerr << " 0x" << data[i];
        }
        std::cerr << std::dec << std::endl;
        m_status = false;
        return false;
    }

    addr = dptr;
    return true;
}

bool GCUReader::skipEvent(int /*num*/)
{
    //TODO: if necessary in future...
    return true;
}

bool GCUReader::initFile(const std::string& file)
{
    if ( m_ifs.is_open() ) {
        m_ifs.close();
    }

    std::cout << "GCUReader now open: " << file << std::endl;

    m_ifs.open(file.c_str(), std::ios::binary);

    if ( ! m_ifs.is_open() ) {
        std::cerr << "GCUReader::initFile() failed to open: " << file << std::endl;
        m_status = false;
        return false;
    }

    uint16_t raw;
    uint16_t data = 0;

    while ( m_ifs.good() ) {
        m_ifs.read((char*)(&raw), 2);
        data = (raw << 8) | (raw >> 8);

        //std::cout << "0x" << std::hex << data << std::dec << std::endl;
        if ( data == 0x805A ) {
            m_ifs.seekg(-2, std::ios::cur);
            break;
        }
    }

    if ( ! m_ifs.good() ) {
        std::cerr << "GCUReader::initFile() failed to init: " << file << std::endl;
        m_status = false;
        return false;
    }

    return true;
}

bool GCUReader::tryRead(char* dest, int size)
{
    if ( m_ifs.read(dest, size).good() ) {
        return true;
    }

    ++m_ifile;

    if ( m_ifile < m_files.size() && initFile( m_files[m_ifile] ) ) {
        return tryRead(dest, size);
    }

    if ( m_ifile != m_files.size() ) {
        std::cerr << "GCUReader::tryRead() failed to read data from file" << std::endl;
    }

    m_status = false;
    return false;
}
