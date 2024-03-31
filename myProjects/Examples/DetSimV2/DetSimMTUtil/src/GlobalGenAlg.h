#ifndef GLOBALGENALG_H
#define GLOBALGENALG_H

/*
 * Produce an event navigator and put it into global buffer.
 * This algorithm is running in an 'input' task/thread.
 */

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"

#include "SniperMuster/GlobalBuffer.h"


class GlobalGenAlg: public AlgBase {
 public:

    GlobalGenAlg(const std::string& name);
    ~GlobalGenAlg();

    bool initialize();
    bool execute();
    bool finalize();

 private:

    GlobalBuffer* m_gbuf;
};

#endif
