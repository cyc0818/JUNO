#ifndef GLOBALRECOUTALG_H
#define GLOBALRECOUTALG_H

/*
 * access the rec event in global buffer.
 * This algorithm is running in an 'output' task/thread.
 */

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"

#include "SniperMuster/GlobalBuffer.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavMT.h"
#include "BufferMemMgr/IDataMemMgr.h"

class GlobalRecOutAlg: public AlgBase {
 public:

    GlobalRecOutAlg(const std::string& name);
    ~GlobalRecOutAlg();

    bool initialize();
    bool execute();
    bool finalize();

 private:

    GlobalBuffer* m_gbuf;
    IDataMemMgr* m_buf; 

};

#endif
