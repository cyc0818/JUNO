#ifndef IAD_DATA_ID_H
#define IAD_DATA_ID_H

namespace IAD
{
    struct EventHeader
    {
        uint32_t marker;
        uint32_t headerSize;
        uint32_t dataSize;
        uint32_t dataId;
        //--------------------------------------------
        uint32_t indexNum;
        uint32_t entry;
        uint32_t sec;
        uint32_t nanoSec;
    };
    //*****************************************Data ID used to specify the IAD type****************************
    template <typename T>
    class DataId
    {
    public:
        const static uint32_t id;
    };

    template <typename T>
    const uint32_t DataId<T>::id = 0;
}

#endif