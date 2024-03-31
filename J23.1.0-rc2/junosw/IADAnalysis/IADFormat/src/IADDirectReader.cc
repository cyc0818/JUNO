#include "IADFormat/IADDirectReader.h"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/mount.h>

using namespace IAD;

size_t IADDirectReader::dread(int fd, void *stylus, size_t sz)
{
    ssize_t readResult = 0;
    void *bufferPtr = stylus;
    size_t count = sz;

    if (count <= m_residualSize)
    {
        memcpy(bufferPtr, (char *)m_localCache + (m_localCacheSize - m_residualSize), count);
        m_residualSize -= count;
        readResult = count;
        return readResult;
    }

    memcpy(bufferPtr, (char *)m_localCache + (m_localCacheSize - m_residualSize), m_residualSize);
    bufferPtr = (char *)bufferPtr + m_residualSize;
    readResult += m_residualSize;
    count -= m_residualSize;
    m_residualSize = 0;

    size_t _loadNum = count / m_localCacheSize;
    size_t _loadBytes = _loadNum * m_localCacheSize;
    size_t _readBytes = 0;
    for (size_t i = 0; i < _loadNum; i++)
    {
        _readBytes = read(fd, m_localCache, m_localCacheSize);
        if (_readBytes != m_localCacheSize)
        {
            throw std::runtime_error("Failed to load diskBlocks");
            return -1;
        }
        memcpy(bufferPtr, m_localCache, m_localCacheSize);
        bufferPtr = (char *)bufferPtr + m_localCacheSize;
    }
    readResult += _loadBytes;
    count -= _loadBytes;

    if (count == 0)
        return readResult;

    _readBytes = read(fd, m_localCache, m_localCacheSize);
    if ((int)_readBytes < 0)
    {
        std::cout << "failed to read one DiskBlock " << (int)_readBytes << std::endl;
        return -1;
    }
    m_residualSize = m_localCacheSize;
    memcpy(bufferPtr, m_localCache, count);
    readResult += count;
    m_residualSize -= count;

    if(readResult != sz){
        throw std::runtime_error("Error: Wrong size for direct IO");
    }
    return readResult;
}

SeperatorRecord IADDirectReader::readSeperator()
{
    SeperatorRecord seperator;
    ssize_t _size = dread(m_fileDescriptor, (void *)&seperator, sizeof(SeperatorRecord));
    bool stat = (_size == sizeof(SeperatorRecord));
    stat = stat && (seperator.marker == Seperator_record_marker || seperator.marker == End_Seperator_marker);
    if(!stat){
        throw std::runtime_error("Error: Failed to read seperator");
    }
    return seperator;
}

BufferPtr IADDirectReader::readBuffer(const SeperatorRecord &seperator)
{
    auto buffer = Buffer::allocateBuffer(seperator.dataSize);
    size_t _size = dread(m_fileDescriptor, buffer->getAddr(), seperator.dataSize);
    buffer->setSize(seperator.dataSize);

    if(_size != seperator.dataSize){
        throw std::runtime_error("Error: Failed to read seperator");
    };
    BaseHeader *header = (BaseHeader *)(buffer->getAddr());
    if(header->dataSize + header->headerSize != _size){
        throw std::runtime_error("Error: Failed to read buffer. Wrong meta data");
    }

    return buffer;
}

IADDirectReader::IADDirectReader(const std::string &fileName, int cache) : m_fileName(fileName), m_residualSize(0), m_localCacheSize(BLOCK_SIZE)
{
    std::cout << "The BLOCK_SIZE of device " << BLOCK_SIZE << std::endl;
    m_localCacheSize = cache * BLOCK_SIZE;
    m_localCache = aligned_alloc(BLOCK_SIZE, m_localCacheSize);

    m_fileDescriptor = open(m_fileName.c_str(), O_RDONLY | O_DIRECT);
    if (m_fileDescriptor <= 0)
    {
        throw std::runtime_error("Error: Can not open the file ");
    }
    off_t curP = lseek(m_fileDescriptor, 0, SEEK_SET);

    dread(m_fileDescriptor, (void *)&m_startRecord, sizeof(FileStartRecord));
    if(m_startRecord.marker != File_start_marker){
        throw std::runtime_error("Error: Failed to open file. Wrong marker");
    }
    off_t starBlockLocate = m_startRecord.recordSize;

    m_startBlockBuffer = Buffer::allocateBuffer(m_startRecord.startBlockSize);
    dread(m_fileDescriptor, m_startBlockBuffer->getAddr(), m_startRecord.startBlockSize);
    m_startBlockBuffer->setSize(m_startRecord.startBlockSize);
}

bool IADDirectReader::next()
{
    SeperatorRecord seperator(readSeperator()); // read a seperator
    if (seperator.marker == End_Seperator_marker)
    { // To the file end
        return false;
    }

    m_currentBuffer = readBuffer(seperator);
    return true;
}

IADDirectReader::~IADDirectReader()
{
    if (close(m_fileDescriptor) == -1)
    {
        std::cout << "Fatal Error can not close file: " << m_fileName << std::endl;
    }
}