#ifndef IAD_BLOCK_H
#define IAD_BLOCK_H

#include "IADFormat/Buffer.h"
#include "IADFormat/DataFragment.h"
#include <iostream>
#include "IADFormat/IADDataId.h"
#include <cstring>
#include <exception>

namespace IAD
{
    class Block : public DataFragment
    {
    private:
        BufferPtr m_buffer;
        BlockHeader *m_blockHeader;
        // When reading evt from block, m_cur remember the current event, and it will be returned by getCur()
        BaseHeader *m_cur;

    public:
        // Create Block object from the buffer read from the file
        Block(BufferPtr buffer);
        Block() = delete;
        // Create the object for writing data to files, when generating IAD data
        Block(uint32_t bufferSize);

        // Functions for data access
        bool next();
        inline BaseHeader *getCur();
        template <typename T, typename H>
        static T *getEvt(H *h);
        uint32_t getPrezipSize() { return m_blockHeader->preZipDataSize; }

        // Functions for data writing
        template <typename T>
        bool addElem(const T &elem, uint32_t index = 0, uint32_t entry = 0, uint32_t sec = 0, uint32_t nanoSec = 0);
        void reset(); // Clear the data in the Block

        bool zip(int level = 1); // Fixme: When data Size exceeds 10MByte, compression algorithms malfunction
        bool unzip();
        void update();

        inline BufferPtr getBuffer();
        virtual ~Block(){};
    };

    BaseHeader *Block::getCur() { return m_cur; }

    template <typename T, typename H>
    T *Block::getEvt(H *h)
    {
        if (h->dataId != DataId<T>::id)
        {
            std::cout << "Wrong Data ID. Failed to get a Evt from a BaseHeader" << std::endl;
            return nullptr;
        }
        return reinterpret_cast<T *>((uint8_t *)h + h->headerSize);
    }

    BufferPtr Block::getBuffer() { return m_buffer; }

    template <typename T>
    bool Block::addElem(const T &elem, uint32_t index, uint32_t entry, uint32_t sec, uint32_t nanoSec)
    {
        uint32_t _size = sizeof(EventHeader) + sizeof(T);
        BaseHeader *blockHeader = headerPtr();
        if (_size + blockHeader->headerSize + blockHeader->dataSize > m_buffer->capacity())
        {
            throw std::runtime_error("Failed to addElem. Insufficient buffer");
            return false;
        }

        // Write header
        uint8_t *_tail = (uint8_t *)dataPtr() + blockHeader->dataSize; // Get the ptr of the tail
        EventHeader *_header = (EventHeader *)_tail;
        _header->marker = Base_header_marker;
        _header->headerSize = sizeof(EventHeader);
        _header->dataSize = sizeof(T);
        _header->dataId = DataId<T>::id;
        _header->indexNum = index;
        _header->entry = entry;
        _header->sec = sec;
        _header->nanoSec = nanoSec;

        // Write the struct
        T *_data = (T *)(_tail + _header->headerSize);
        *_data = elem;

        // update the information of block header
        blockHeader->dataSize += _header->headerSize + _header->dataSize;
        // Update the memta data of the buffer
        m_buffer->setSize(blockHeader->headerSize + blockHeader->dataSize);

        if(m_buffer->size() <= m_buffer->capacity()){// fix me: This validation should be done before adding element
            throw std::runtime_error("Too many elements, exceeding the buffer of the block");
        }

        if (_header->sec == 0 && _header->nanoSec == 0)
            std::cout << "Warning *********Wrong Time" << std::endl;

        return true;
    }

    typedef std::shared_ptr<Block> IBlockPtr;
}
#endif