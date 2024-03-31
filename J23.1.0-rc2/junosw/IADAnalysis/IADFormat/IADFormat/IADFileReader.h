#ifndef IAD_FILE_READER
#define IAD_FILE_READER

#include "IADFormat/IADDefine.h"
#include <string>
#include "IADFormat/Buffer.h"
#include "IADFormat/IReader.h"

namespace IAD
{
    class IADFileReader
    {
    private:
        std::string m_fileName;
        int m_fileDescriptor;
        BufferPtr m_startBlockBuffer;
        FileStartRecord m_startRecord;
        
        BufferPtr m_currentBuffer;
        BufferPtr m_metaBuffer;//metaBuffer, which locates the end of the file

        SeperatorRecord readSeperator();
        BufferPtr readBuffer(const SeperatorRecord& seperator);
    public:
        IADFileReader(const std::string& file);
        bool next();//
        inline BufferPtr getData(){return m_currentBuffer;};
        inline BufferPtr getMetaBuffer(){return m_metaBuffer;};
        ~IADFileReader();
    };
}

#endif