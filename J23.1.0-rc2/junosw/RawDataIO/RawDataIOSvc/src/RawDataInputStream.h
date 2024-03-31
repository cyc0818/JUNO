#ifndef RAWDATA_INPUT_STREAM_H
#define RAWDATA_INPUT_STREAM_H

#include "RootIOSvc/IInputStream.h"
#include "RawFile/IRawReader.h"
#include <vector>
#include <string>

class RawDataInputStream : public IInputStream
{
    public :
        RawDataInputStream(const std::vector<std::string>& files, const std::string& type);
        virtual ~RawDataInputStream();

        //for SniperLog
        const std::string& scope() { return m_scope; }
        const std::string& objName() { return m_type; }

        //implement the interface in IInputStream
        virtual bool     initialize();
        virtual bool     finalize();
        virtual bool     next(int step = 1, bool read = true);
        virtual bool     first(bool read = true);
        virtual TObject* get();

    private :

        char*         m_addr;
        int           m_readbyte;
        IRawReader*   m_reader;

        const std::vector<std::string>& m_files;

        std::string   m_scope;
        std::string   m_type;
};

#endif
