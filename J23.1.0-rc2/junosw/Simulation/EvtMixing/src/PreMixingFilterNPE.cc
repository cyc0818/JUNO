/*
 * Description:
 *   This tool implements a simple filter based on nPE.
 *   It will look at all the sub-detectors. 
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 *   Sept 1st, 2022
 */
#include "SniperKernel/ToolBase.h"
#include "IPreMixingFilter.hh"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "Event/SimHeader.h"
#include "EvtNavigator/EvtNavHelper.h"

class PreMixingFilterNPE: public ToolBase,
                          public IPreMixingFilter {
public:
    PreMixingFilterNPE(const std::string& name);
    ~PreMixingFilterNPE();

    bool accept(JM::EvtNavigator* nav);

private:
    int threshold_nhits_cdlpmt;
    int threshold_nhits_cdspmt;

    int threshold_nhits_wp;
    int threshold_nhits_tt;

};

PreMixingFilterNPE::PreMixingFilterNPE(const std::string& name)
    : ToolBase(name) {
    declProp("ThresholdNhitsCdLpmt", threshold_nhits_cdlpmt=0);
    declProp("ThresholdNhitsCdSpmt", threshold_nhits_cdspmt=0);
    declProp("ThresholdNhitsWp", threshold_nhits_wp=0);
    declProp("ThresholdNhitsTt", threshold_nhits_tt=0);
}

PreMixingFilterNPE::~PreMixingFilterNPE() {

}

bool PreMixingFilterNPE::accept(JM::EvtNavigator* nav) {
    bool is_accepted = false;

    if (not nav) {
        LogError << "Cannot load event navigator. " << std::endl;
        return is_accepted;
    }

    auto simhdr = JM::getHeaderObject<JM::SimHeader>(nav);
    if (not simhdr) {
        LogError << "Cannot load SimHeader" << std::endl;
        return is_accepted;
    }

    // get the SimEvent
    auto simevt = dynamic_cast<JM::SimEvt*>(simhdr->event());

    // CD
    auto nhits_cdlpmt = simevt->getLpmtHits();
    auto nhits_cdspmt = simevt->getSpmtHits();

    if (nhits_cdlpmt > threshold_nhits_cdlpmt) {
        is_accepted = true;
    }

    if (nhits_cdspmt > threshold_nhits_cdspmt) {
        is_accepted = true;
    }

    // WP
    auto hits_wp = simevt->getWPHitsVec();

    if (hits_wp.size() > threshold_nhits_wp) {
        is_accepted = true;
    }

    // TT
    auto hits_tt = simevt->getTTHitsVec();
    
    if (hits_tt.size() > threshold_nhits_tt) {
        is_accepted = true;
    }

    return is_accepted;
}


DECLARE_TOOL(PreMixingFilterNPE);
