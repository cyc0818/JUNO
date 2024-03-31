#include "IADFormat/IADFileWriter.h"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using namespace IAD;

IADFileWriter::IADFileWriter(const std::string &fileName, uint32_t startBlockSize) : m_fileName(fileName), m_offset(0), m_isopen(false)
{
    m_fileDescriptor = open(m_fileName.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    off_t curP = lseek(m_fileDescriptor, 0, SEEK_SET);
    if(m_fileDescriptor <= 0 || curP != 0){
        throw std::runtime_error("Fatal: Fail to open IAD file");
    }

    m_startRecord.marker = File_start_marker;
    m_startRecord.recordSize = sizeof(FileStartRecord);
    m_startRecord.startBlockSize = startBlockSize;
    m_startRecord.dataSeperatorSize = sizeof(SeperatorRecord);

    auto _size = write(m_fileDescriptor, (char *)&m_startRecord, m_startRecord.recordSize);
    if(_size != m_startRecord.recordSize){
        throw std::runtime_error("Error: Failed to write start record");
    }
    m_offset += _size;

    m_isopen = true;
}

bool IADFileWriter::writeBuffer(BufferPtr buffer, uint32_t marker)
{ // special marker of last seperator
    SeperatorRecord seperator({marker, (uint32_t)sizeof(SeperatorRecord), (uint32_t)buffer->size()});

    auto _size = write(m_fileDescriptor, (char *)&seperator, sizeof(SeperatorRecord));

    m_offset += _size;
    _size = write(m_fileDescriptor, (char *)buffer->getAddr(), buffer->size());
    if((uint32_t)_size != buffer->size()){
        throw std::runtime_error("Error: Failed to write buffer");
    }
    m_offset += _size;

    return true;
}

bool IADFileWriter::writeMetaBlock(MetaBlock &metaBlock)
{
    metaBlock.syncHeader(m_metaHeader);
    auto buffer = Buffer::allocateBuffer(metaBlock.countSize());
    buffer->setSize(metaBlock.streamOut(buffer->getAddr()));
    m_startRecord.metaBufferLocte = m_offset;

    if (writeBuffer(buffer, End_Seperator_marker))
    { // write MetaBlock and close the file
        // Update the locate of metaBlock
        off_t _curP = lseek(m_fileDescriptor, 0, SEEK_SET);
        if(_curP != 0)  return false;
        
        auto _size = write(m_fileDescriptor, (char *)&m_startRecord, m_startRecord.recordSize);
        if(_size != m_startRecord.recordSize){
            throw std::runtime_error("Error: failed to uodate start Record");
        }

        if (close(m_fileDescriptor) == -1)
        {
            std::cout << "Fatal Error can not close file: " << m_fileName << std::endl;
            return false;
        }
        m_isopen = false;
        return true;
    }
    else
        return false;
}

IADFileWriter::~IADFileWriter()
{

    // Write last seperator

    if (m_isopen == false)
        return;

    // Write default MetaBlock，close file
    MetaBlock metaBlock(m_metaHeader);
    writeMetaBlock(metaBlock);
}
