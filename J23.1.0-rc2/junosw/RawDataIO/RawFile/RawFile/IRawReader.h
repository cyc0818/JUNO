#ifndef I_RAW_READER_H
#define I_RAW_READER_H

class IRawReader
{
    public :

        virtual bool status() = 0;

        virtual bool readEvent(char*& addr, int& size /*in bytes*/) = 0;

        virtual bool skipEvent(int num) = 0;
};

#endif
