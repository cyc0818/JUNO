#ifndef IAD_IREADER_H
#define IAD_IREADER_H

#include "IADFormat/Buffer.h"

namespace IAD
{
    class IReader
    {
    private:
    public:
        IReader();
        virtual bool next() = 0;
        virtual BufferPtr getData() = 0;
        virtual ~IReader();
    };
}

#endif