#include "IADFormat/Buffer.h"
#include <mutex>
#include <algorithm>
#include <stdlib.h>
#include <iostream>

using namespace IAD;

Buffer::Buffer(uint32_t size) : m_size(0)
{
    m_addr = MemoryPool::instance()->allocateMem(size, m_maxSize);
}

Buffer::~Buffer()
{
    MemoryPool::instance()->deallocateMem(m_addr, m_maxSize);
}

MemoryPool *MemoryPool::s_obj = nullptr;

const std::vector<size_t> MemoryPool::s_memSizes = {
    256,
    1024,
    4096,
    8192,
    16384,
    32768,
    65536,
    131072,
    262144,
    524288,
    1048576,
    2097152,
    4194204,
    6291456,
    8388608,
    11534336, // 11M
    // 22020096, // 21M
    32505856,   // 31M
    53477376,   // 51M
    104857600,  // 100M
    209715200,  // 200M
    524288000,  // 500M
    629145600,  // 600M
    1073741824, // 1000M
};

MemoryPool::MemoryPool() : m_memCount(0),
                           m_freeMemNum(s_memSizes.size(), 0),
                           m_busyMemNum(s_memSizes.size(), 0),
                           m_freeMemPool(s_memSizes.size(), std::vector<void *>())
{
}

void *MemoryPool::allocateMem(size_t size, size_t &maxSize)
{
    auto _upper = std::lower_bound(s_memSizes.begin(), s_memSizes.end(), size);
    if(_upper == s_memSizes.end()){
        throw std::runtime_error("Fatal: Failed to allocate Mem, too large");
    }
    int index = _upper - s_memSizes.begin();

    std::unique_lock<std::mutex> lock(m_memPoolMutex); // avoid data racing, when applying and releasing memory
    if (m_freeMemNum[index] == 0)
    { // Need to new create new memory slice
        void *ptr = malloc(s_memSizes[index] + sizeof(MemHeader));
        MemHeader *header = (MemHeader *)ptr;
        header->size = s_memSizes[index];
        header->marker = Mem_marker;

        m_freeMemPool[index].push_back(ptr);
        m_freeMemNum[index]++;
        m_memCount++;
    }
    void *ptr = m_freeMemPool[index].back();
    m_freeMemPool[index].pop_back();
    maxSize = s_memSizes[index];
    m_freeMemNum[index]--;
    m_busyMemNum[index]++;
    return (char *)ptr + sizeof(MemHeader);
}

void MemoryPool::deallocateMem(void *ptr, size_t maxSize)
{
    MemHeader *header = (MemHeader *)((char *)ptr - sizeof(MemHeader));
    int index = std::lower_bound(s_memSizes.begin(), s_memSizes.end(), maxSize) - s_memSizes.begin();
    
    if(header->marker != Mem_marker || header->size != s_memSizes[index] || header->size != maxSize){
        throw std::runtime_error("Fatal: deallocating invalid Mem");
    }

    std::unique_lock<std::mutex> lock(m_memPoolMutex);
    m_freeMemPool[index].push_back(header);
    m_freeMemNum[index]++;
    m_busyMemNum[index]--;

    return;
}

MemoryPool *MemoryPool::instance()
{
    static std::once_flag initFlag;
    std::call_once(initFlag, []()
                   {
        std::cout<<"Creating a pool"<<std::endl;
        s_obj = new MemoryPool(); });
    return s_obj;
}

void MemoryPool::printStatus()
{
    std::cout << "Total " << m_memCount << std::endl;
    for (size_t i = 0; i < s_memSizes.size(); i++)
    {
        if (m_freeMemNum[i] != 0)
            std::cout << "Free Mem " << s_memSizes[i] << " " << m_freeMemNum[i] << std::endl;
        if (m_busyMemNum[i] != 0)
            std::cout << "Busy Mem " << s_memSizes[i] << " " << m_busyMemNum[i] << std::endl;
    }
}