#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "Event/SimHeader.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "MCTruthMuonBundleClassifier.h"

DECLARE_TOOL(MCTruthMuonBundleClassifier);

MCTruthMuonBundleClassifier::MCTruthMuonBundleClassifier(const std::string& name)
    : ToolBase(name) {

}

MCTruthMuonBundleClassifier::~MCTruthMuonBundleClassifier() {

}

bool MCTruthMuonBundleClassifier::configure() {
    // initialize the classifier

    return true;
}

ClassificationType MCTruthMuonBundleClassifier::classify(JM::NavBuffer* buffer) {
    ClassificationType classified_type = ClassificationType::kUnknown;

    // check buffer
    if (not buffer) {
        LogError << "event buffer is empty. " << std::endl;
        return classified_type;
    }

    // check current event navigator
    auto nav = buffer->curEvt();

    if (not nav) {
        LogError << "Cannot load event navigator. " << std::endl;
        return classified_type;
    }

    // get the SimHeader
    auto simhdr = JM::getHeaderObject<JM::SimHeader>(nav);
    if (not simhdr) {
        LogError << "Cannot load SimHeader" << std::endl;
        return classified_type;
    }

    // get the SimEvent
    auto simevt = dynamic_cast<JM::SimEvt*>(simhdr->event());

    // get the Track
    const auto& tracks = simevt->getTracksVec();

    int muon_track_counter = 0;

    for (const auto& track: tracks) {
        auto pdgid = track->getPDGID();

        // check the pdgcode in the SimTrack collection
        // if it is mu+/- (13 or -13), classify it to track like
        if (pdgid == 13 or pdgid == -13) {
            ++muon_track_counter;
        }

    }

    if (muon_track_counter == 1) {
        classified_type = ClassificationType::kMuonSingle;
    } else if (muon_track_counter > 1) {
        classified_type = ClassificationType::kMuonBundles;
    }

    return classified_type;
}
