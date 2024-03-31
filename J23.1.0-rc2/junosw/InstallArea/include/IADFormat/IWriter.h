#ifndef IAD_IWRITER_H
#define IAD_IWRITER_H

#include "IADFormat/Buffer.h"

namespace IAD
{
    class IWriter{
    private:
    public:
        IWriter();
        virtual bool writeBuffer(BufferPtr buffer) = 0;
        virtual ~IWriter();
    };
}

#endif