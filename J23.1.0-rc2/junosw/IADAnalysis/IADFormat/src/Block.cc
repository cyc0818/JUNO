#include "IADFormat/Block.h"
#include <iterator>
#include <iostream>
#include <RZip.h>

using namespace IAD;

Block::Block(BufferPtr buffer) : DataFragment(buffer->getAddr()),
                                 m_buffer(buffer)
{
    m_blockHeader = (BlockHeader *)(headerPtr());
    if(m_buffer->size() != headerPtr()->headerSize + headerPtr()->dataSize){
        throw std::runtime_error("Error: Failed to construct block from buffer. Wrong data size");
    }
    m_cur = (BaseHeader *)(buffer->getAddr());
    if(m_cur->marker != Base_header_marker){
        throw std::runtime_error("Error: Failed to construct block from buffer. Wrong marker");
    }

    unzip();
}

Block::Block(uint32_t bufferSize) : DataFragment()
{
    m_buffer = Buffer::allocateBuffer(bufferSize);
    reset();
}

bool Block::next()
{
    uint32_t distance = 0;
    BaseHeader *blockHeader = headerPtr();
    if (m_cur == blockHeader)
    { //
        distance = blockHeader->headerSize;
    }
    else
    {
        distance = m_cur->headerSize + m_cur->dataSize;
    }

    // Consider if it exceeds the limit of block
    if (distance + (uint8_t *)m_cur - (uint8_t *)blockHeader >= blockHeader->headerSize + blockHeader->dataSize)
    {
        // std::cout<<"Failed to next: out of range"<<std::endl;
        return false;
    }
    m_cur = (BaseHeader *)((uint8_t *)m_cur + distance);
    if(m_cur->marker != Base_header_marker){
        throw std::runtime_error("Error: Failed to get next evt. Wrong marker");
    }
    return true;
}

void Block::reset()
{
    m_blockHeader = (BlockHeader *)(m_buffer->getAddr());
    m_blockHeader->marker = Base_header_marker;
    m_blockHeader->headerSize = sizeof(BlockHeader);
    m_blockHeader->dataSize = 0;
    m_blockHeader->zipLevel = 0;
    m_blockHeader->preZipDataSize = 0;
    setAddr(m_buffer->getAddr());
    m_buffer->setSize(m_blockHeader->headerSize + m_blockHeader->dataSize);
    setAddr(m_buffer->getAddr());
}

bool Block::zip(int level)
{
    if (m_blockHeader->zipLevel > 0)
        return true;

    auto cache = Buffer::allocateBuffer(m_buffer->size());
    int sizeBeforezip = m_blockHeader->dataSize;
    int zippedBufSize = cache->capacity();
    int sizeAfterZip = 0;

    R__zipMultipleAlgorithm(level, &sizeBeforezip, (char *)dataPtr(), &zippedBufSize, (char *)(cache->getAddr()), &sizeAfterZip, ROOT::RCompressionSetting::EAlgorithm::EValues::kZLIB);
    std::memcpy(dataPtr(), cache->getAddr(), sizeAfterZip);

    m_blockHeader->preZipDataSize = sizeBeforezip;
    m_blockHeader->dataSize = sizeAfterZip;
    m_blockHeader->zipLevel = level;
    m_buffer->setSize(m_blockHeader->headerSize + m_blockHeader->dataSize);
    return true;
}

bool Block::unzip()
{
    if (m_blockHeader->zipLevel == 0)
        return true;

    auto cache = Buffer::allocateBuffer(m_blockHeader->headerSize + m_blockHeader->preZipDataSize);
    std::memcpy(cache->getAddr(), m_buffer->getAddr(), m_blockHeader->headerSize);
    unsigned char *_unzipDest = (unsigned char *)(cache->getAddr()) + m_blockHeader->headerSize;

    int zippedSize = m_blockHeader->dataSize;
    int unzipBufSz = m_blockHeader->preZipDataSize; // dataSize before zip
    int sizeAfterUnzip = 0;
    R__unzip(&zippedSize, (unsigned char *)dataPtr(), &unzipBufSz, _unzipDest, &sizeAfterUnzip);
    // std::cout<<"unzipping preZipDataSize "<<(int)(m_blockHeader->preZipDataSize)<<std::endl;
    if(sizeAfterUnzip != (int)(m_blockHeader->preZipDataSize)){
        throw std::runtime_error("Error: Failed to zip");
    }

    m_buffer = cache;
    m_blockHeader = (BlockHeader *)(m_buffer->getAddr());
    // std::cout<<__LINE__<<" "<<m_blockHeader->preZipDataSize<<std::endl;
    m_blockHeader->dataSize = m_blockHeader->preZipDataSize;
    m_blockHeader->zipLevel = 0;
    m_buffer->setSize(m_blockHeader->headerSize + m_blockHeader->dataSize);
    setAddr(m_buffer->getAddr());
    if(m_blockHeader->marker != Base_header_marker){
        throw std::runtime_error("Fatal: Wrong marker. Failed to unzip");
    }
    m_cur = headerPtr();
    return true;
}