#ifndef GLOBALRECINPUTALG_H
#define GLOBALRECINPUTALG_H

/*
 * load an event navigator with calib event and put it into global buffer.
 * This algorithm is running in an 'input' task/thread.
 */

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"

#include "SniperMuster/GlobalBuffer.h"

#include "EvtNavigator/NavBuffer.h"

#include "EvtNavMT.h"

class GlobalRecInputAlg: public AlgBase {
 public:

    GlobalRecInputAlg(const std::string& name);
    ~GlobalRecInputAlg();

    bool initialize();
    bool execute();
    bool finalize();

 private:

    GlobalBuffer* m_gbuf;

    JM::NavBuffer* m_buf; 

};

#endif
