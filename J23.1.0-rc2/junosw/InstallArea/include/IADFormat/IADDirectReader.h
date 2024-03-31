#ifndef IAD_DIRECT_READER_H
#define IAD_DIRECT_READER_H

#include "IADFormat/IADDefine.h"
#include <string>
#include "IADFormat/Buffer.h"
#include "IADFormat/IReader.h"

namespace IAD
{
    class IADDirectReader//The reader that bypasses the linux kernel cache
    {
    private:
        std::string m_fileName;
        int m_fileDescriptor;
        BufferPtr m_startBlockBuffer;
        FileStartRecord m_startRecord;

        BufferPtr m_currentBuffer;
        BufferPtr m_metaBuffer; // metaBuffer, which locates the end of the file

        void *m_localCache;
        size_t m_residualSize;
        size_t m_localCacheSize;
        size_t dread(int fd, void *stylus, size_t sz);

        SeperatorRecord readSeperator();
        BufferPtr readBuffer(const SeperatorRecord &seperator);

    public:
        IADDirectReader(const std::string &file, int cache);
        bool next(); //
        inline BufferPtr getData() { return m_currentBuffer; };
        inline BufferPtr getMetaBuffer() { return m_metaBuffer; };
        ~IADDirectReader();
    };
}

#endif