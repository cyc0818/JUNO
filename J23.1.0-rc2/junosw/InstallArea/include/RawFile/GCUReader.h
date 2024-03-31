#ifndef GCU_READER_H
#define GCU_READER_H

#include "RawFile/IRawReader.h"
#include <vector>
#include <string>
#include <fstream>

class GCUReader : public IRawReader
{
    public :

        GCUReader(const std::vector<std::string>& files);
        virtual ~GCUReader();

        bool status() { return m_status; }
        bool readEvent(char*& addr, int& size /*in bytes*/);
        bool skipEvent(int num);

    private :

        bool initFile(const std::string& file);
        bool tryRead(char* dest, int size);

        const std::vector<std::string>& m_files;

        unsigned int   m_ifile;
        bool           m_status;

        std::ifstream  m_ifs;
};

#endif
