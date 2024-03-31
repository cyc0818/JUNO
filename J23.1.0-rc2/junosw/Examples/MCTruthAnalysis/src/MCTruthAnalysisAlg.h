#ifndef MCTruthAnalysisAlg_h
#define MCTruthAnalysisAlg_h

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

class MCTruthAnalysisAlg : public AlgBase {

public:
    MCTruthAnalysisAlg(const std::string& name);

    bool initialize();
    bool execute();
    bool finalize();

private:
    JM::NavBuffer* m_buf;
};

#endif
