#ifndef GLOBALDETSIMOUTALG_H
#define GLOBALDETSIMOUTALG_H

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"

// #include "SniperMuster/GlobalBuffer.h"
#include "MtUtilities/EventContext.h"

#include "TTree.h"

class GlobalDetSimOutAlg: public AlgBase {
 public:

    GlobalDetSimOutAlg(const std::string& name);
    ~GlobalDetSimOutAlg();

    bool initialize();
    bool execute();
    bool finalize();

 private:

    JunoGlobalBuffer* m_gbuf;

    TTree* m_evttree;
    int m_evtid;
    int m_nHits;
    int m_totalPE;
    std::vector<int> m_nPE;
    std::vector<double> m_hitTime;

};


#endif
