#ifndef RawEvt_READER_H
#define RawEvt_READER_H

#include "RawFile/IRawReader.h"
#include <vector>
#include <string>
#include <fstream>

class RawEvtReader : public IRawReader
{
    public :

        RawEvtReader(const std::vector<std::string>& files);
        virtual ~RawEvtReader();

        bool status() { return m_status; }
        bool readEvent(char*& addr, int& size /*in bytes*/);
        bool skipEvent(int num);
        char* getFileHeader() { return m_fileHdr; }
        //char* getDPHeader() { return m_DPHdr; }

    private :

        bool initFile(const std::string& file);
        bool tryRead(char* dest, int size);

        const std::vector<std::string>& m_files;

        unsigned int   m_ifile;

        // - about File_Start_Record:
        unsigned int   m_fileHdrSize;
        char*          m_fileHdr;       // points to File_Start_Record
        unsigned int   m_version;

        // - about DP_Header_Record:
        unsigned int   m_DPHdrSize;
        char*          m_DPHdr;         // points to DP_Header_Record

        // - about reading data:
        unsigned int   m_cacheSize;
        unsigned int   m_leftEvt;       // the number of the rest evt in current DP block
        unsigned int   m_leftSize;      // the rest size of current events data
        unsigned int   m_shift;         // the number of bytes to jump from m_dptr
        char*          m_dptr;          // points to the events data in a DP block

        bool           m_status;
        std::ifstream  m_ifs;
};

#endif
