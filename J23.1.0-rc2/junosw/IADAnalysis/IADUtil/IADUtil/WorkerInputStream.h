#ifndef WORKER_INPUT_STREAM_H
#define WORKER_INPUT_STREAM_H

#include "IADFormat/Block.h"
#include <memory>
#include "SniperKernel/SniperLog.h"
#include "IADUtil/IADBlockContext.h"
#include <mutex>
#include "IADSvc/IADEvtBox.h"

class WorkerInputStream{
public:
    WorkerInputStream();
    WorkerInputStream(std::vector<std::string> fileList);
    virtual bool initialize();
    virtual bool finalize();
    virtual bool first(bool read);
    virtual bool next(int step = 1, bool read = true);
    virtual IADEvtBox get();
private:
    IADGlobalBuffer *m_gbuf;
    std::string m_sname;
    std::unique_ptr<IAD::Block> m_block;//The current block, that is read
    IADGlobalBuffer::Elem* m_elem;//The current elem
};

#endif

