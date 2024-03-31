#include "IADFormat/MetaBlock.h"
#include "IADFormat/IADDefine.h"
#include <stdint.h>
#include <string>
#include <iostream>

using namespace IAD;

MetaBlock::MetaBlock(BufferPtr buffer) : m_header(), m_index2Path()
{
    uint8_t *ptr = (uint8_t *)(buffer->getAddr());

    // verify
    MetaBlockHeader *headerPtr = (MetaBlockHeader *)ptr;
    if(headerPtr->marker != Base_header_marker){
        throw std::runtime_error("Fatal: Failed to construct metablock. Wrong marker.");
    }
    if(headerPtr->headerSize + headerPtr->dataSize != buffer->size()){
        throw std::runtime_error("Fatal: Failed to construct metablock. Wrong size");
    }

    m_header = *headerPtr;
    ptr += m_header.headerSize;

    // generate index map
    for (uint32_t i = 0; i < headerPtr->indexNum; i++)
    {
        IndexItemHeader *_h = (IndexItemHeader *)ptr;
        if(_h->marker != Base_header_marker){
            throw std::runtime_error("Fatal: Wrong marker");
        }
        ptr += _h->headerSize; // set ptr to path
        const char *path = (char *)ptr;
        ptr += _h->dataSize; // set ptr to next index header

        m_index2Path.insert({_h->pathId, std::string(path)});
    }
}

uint32_t MetaBlock::countSize()
{
    m_header.dataSize = 0;
    m_header.indexNum = 0;
    for (std::pair<uint32_t, std::string> _i : m_index2Path)
    {
        m_header.dataSize += sizeof(IndexItemHeader) + _i.second.size() + 1; // std::string to c_str，there is additional\0
        m_header.indexNum++;
    }

    return m_header.headerSize + m_header.dataSize;
}

uint32_t MetaBlock::streamOut(void *dest)
{
    countSize();

    uint8_t *ptr = (uint8_t *)dest;

    *((MetaBlockHeader *)ptr) = m_header;
    ptr += m_header.headerSize;

    // serialize the map
    for (const std::pair<const uint32_t, std::string> &_i : m_index2Path)
    {
        IndexItemHeader *_h = (IndexItemHeader *)ptr; // write index header
        _h->marker = Base_header_marker;
        _h->headerSize = sizeof(IndexItemHeader);
        _h->dataSize = _i.second.size() + 1;
        _h->pathId = _i.first;
        ptr += _h->headerSize;

        char *s = (char *)ptr; // write path
        _i.second.copy(s, _i.second.size());
        s[_i.second.size()] = '\0'; // end of string: \0
        ptr += _i.second.size() + 1;
    }

    uint32_t res = ptr - (uint8_t *)dest;
    if(res != countSize()){
        throw std::runtime_error("Fatal: Failed to stream out");
    }
    return res;
}