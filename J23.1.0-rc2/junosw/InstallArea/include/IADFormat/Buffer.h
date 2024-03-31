#ifndef IAD_BUFFER_H
#define IAD_BUFFER_H

#include <memory>
#include <vector>
#include <deque>
#include <mutex>

namespace IAD
{

    class Buffer
    {
    private:
        void *m_addr;
        size_t m_maxSize;
        size_t m_size;
        Buffer(uint32_t size); // Get memory from the pool
        Buffer() {}
        // FixMe: copy construtor should be private
    public:
        inline static std::shared_ptr<Buffer> allocateBuffer(uint32_t size);
        inline void *getAddr();
        inline void setSize(size_t size);
        inline size_t size();
        inline size_t capacity();
        bool writeBuffer(void *source, uint32_t size);
        ~Buffer(); // Return the memory to the pool when destroy the object
    };
    typedef std::shared_ptr<Buffer> BufferPtr;

    BufferPtr Buffer::allocateBuffer(uint32_t size) { return BufferPtr(new Buffer(size)); }

    void *Buffer::getAddr() { return m_addr; }

    void Buffer::setSize(size_t size) { m_size = size; }

    size_t Buffer::size() { return m_size; }

    size_t Buffer::capacity() { return m_maxSize; }

    const uint32_t Mem_marker = 0xccddee11;
    class MemoryPool
    {
        friend class Buffer;
        struct MemHeader
        {
            size_t size;
            uint32_t marker;
        };

    private:
        static MemoryPool *s_obj;
        static const std::vector<size_t> s_memSizes;

        std::mutex m_memPoolMutex;
        int m_memCount;
        std::vector<size_t> m_freeMemNum;
        std::vector<size_t> m_busyMemNum;
        std::vector<std::vector<void *>> m_freeMemPool;

        void *allocateMem(size_t size, size_t &maxSize);
        void deallocateMem(void *ptr, size_t maxSize);
        MemoryPool();

    public:
        static MemoryPool *instance();
        void printStatus(); // for test
        ~MemoryPool();
    };

}

#endif