#include "IADFormat/IADFileReader.h"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using namespace IAD;

SeperatorRecord IADFileReader::readSeperator()
{
    SeperatorRecord seperator;
    ssize_t _size = read(m_fileDescriptor, (void *)&seperator, sizeof(SeperatorRecord));
    bool stat = _size == sizeof(SeperatorRecord);
    stat = stat && (seperator.marker == Seperator_record_marker || seperator.marker == End_Seperator_marker);
    if(!stat){
        throw std::runtime_error("Error: Failed to read seperator");
    }
    return seperator;
}

BufferPtr IADFileReader::readBuffer(const SeperatorRecord &seperator)
{
    auto buffer = Buffer::allocateBuffer(seperator.dataSize);
    size_t _size = read(m_fileDescriptor, buffer->getAddr(), seperator.dataSize);
    buffer->setSize(seperator.dataSize);

    BaseHeader *header = (BaseHeader *)(buffer->getAddr());
    if(header->dataSize + header->headerSize != _size){
        throw std::runtime_error("Fatal: Failed to read buffer from file");
    }

    return buffer;
}

IADFileReader::IADFileReader(const std::string &fileName) : m_fileName(fileName)
{
    m_fileDescriptor = open(m_fileName.c_str(), O_RDONLY);
    if (m_fileDescriptor <= 0)
    {
        throw std::runtime_error("Error: Can not open the file" + fileName);
    }
    off_t curP = lseek(m_fileDescriptor, 0, SEEK_SET);

    read(m_fileDescriptor, (void *)&m_startRecord, sizeof(FileStartRecord));
    if(m_startRecord.marker != File_start_marker){
        throw std::runtime_error("Error: Failed to openm file. Wrong marker");
    }
    off_t starBlockLocate = m_startRecord.recordSize;

    // Read MetaBuffer
    curP = lseek(m_fileDescriptor, m_startRecord.metaBufferLocte, SEEK_SET);
    // std::cout<<__LINE__<<std::endl;
    SeperatorRecord seperator(readSeperator());
    // std::cout<<__LINE__<<std::endl;
    if(seperator.marker != End_Seperator_marker){
        throw std::runtime_error("Error: Failed to read metabuffer. Wrong marker");
    }
    m_metaBuffer = readBuffer(seperator);
    curP = lseek(m_fileDescriptor, starBlockLocate, SEEK_SET); // navigate the file ptr to the first block

    m_startBlockBuffer = Buffer::allocateBuffer(m_startRecord.startBlockSize);
    read(m_fileDescriptor, m_startBlockBuffer->getAddr(), m_startRecord.startBlockSize);
    m_startBlockBuffer->setSize(m_startRecord.startBlockSize);
}

bool IADFileReader::next()
{
    SeperatorRecord seperator(readSeperator()); // read seperator
    if (seperator.marker == End_Seperator_marker)
    { // The file end
        return false;
    }

    m_currentBuffer = readBuffer(seperator);
    return true;
}

IADFileReader::~IADFileReader()
{
    if (close(m_fileDescriptor) == -1)
    {
        std::cout << "Fatal Error can not close file: " << m_fileName << std::endl;
    }
}