#ifndef IAD_DATA_UNIT
#define IAD_DATA_UNIT

#include <cstdint>
#include "IADFormat/IADDefine.h"

namespace IAD
{
    // All data units are in the form of "header + data", this is the base class of Block
    class DataFragment
    {
    private:
        BaseHeader *m_header;
        void *m_data;

    public:
        inline DataFragment(void *source);
        inline BaseHeader *headerPtr();
        inline void *dataPtr();
        inline void setAddr(void *ptr);
        virtual ~DataFragment(){};

    protected:
        inline DataFragment();
    };

    DataFragment::DataFragment(void *source) : m_header((BaseHeader *)source)
    {
        m_data = (char *)m_header + m_header->headerSize;
    }

    BaseHeader *DataFragment::headerPtr() { return m_header; }

    void *DataFragment::dataPtr() { return m_data; }

    void DataFragment::setAddr(void *ptr)
    {
        m_header = (BaseHeader *)ptr;
        if(m_header->marker != Base_header_marker){
            throw std::runtime_error("Error: Can't not construct DataFragment. Wrong marker");
        }
        m_data = (uint8_t *)m_header + m_header->headerSize;
    }

    DataFragment::DataFragment() : m_header(nullptr), m_data(nullptr){};

}

#endif