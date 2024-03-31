#ifndef TEST_SPMT_ELEC_ALG_H
#define TEST_SPMT_ELEC_ALG_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "TRandom.h"
#include "TFile.h"
#include "TH1F.h"

#include "Event/ElecHeader.h"
#include "Event/ElecEvent.h"
#include "Event/ElecTruthHeader.h"
#include "Event/SpmtElecTruthEvent.h"
#include "Event/SimHeader.h"

#include <string>

class TestSpmtElecAlg : public AlgBase
{
  public :

    TestSpmtElecAlg(const std::string& name);

    bool initialize();
    bool execute();
    bool finalize();

  private :

    JM::NavBuffer* m_buf;
    TRandom        m_r;

    void ShowEvent();
    void FillHistos();

    int event;
    int showEvent;
    bool showLPMT;
    bool showSPMT;
    std::string outRootName;


    JM::ElecHeader* elec_header;
    JM::ElecTruthHeader* elec_truth_header;
    std::vector<JM::HeaderObject*> sim_hdrs;

    TFile* outUserFile;

    TH1F* histoBlocksNum;
    TH1F* histoSinglePMT;
    TH1F* histoAllPMTs;
    TH1F* histoSpmtTruthHits;
    TH1F* histoSpmtElecTruthHits;


};

#endif
