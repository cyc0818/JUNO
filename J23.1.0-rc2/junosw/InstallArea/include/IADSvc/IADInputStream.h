#ifndef IAD_INPUT_STREAM_H
#define IAD_INPUT_STREAM_H

#include "IADFormat/Block.h"
#include "SniperKernel/SniperLog.h"
#include "IADEvtBox.h"
#include "IADFormat/IADFileReader.h"

class IADInputStream
{
public:
    IADInputStream(std::string inputFile);
    bool initialize() { return true; };
    bool finalize() { return true; };
    bool next();
    IADEvtBox get();

private:
    std::unique_ptr<IAD::Block> m_block; // current block to be read
    IAD::IADFileReader *m_reader;
};

#endif
