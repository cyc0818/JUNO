#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "Event/SimHeader.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "MCTruthPointLikeClassifier.h"

DECLARE_TOOL(MCTruthPointLikeClassifier);

MCTruthPointLikeClassifier::MCTruthPointLikeClassifier(const std::string& name)
    : ToolBase(name) {

}

MCTruthPointLikeClassifier::~MCTruthPointLikeClassifier() {

}

bool MCTruthPointLikeClassifier::configure() {
    // initialize the classifier

    return true;
}

ClassificationType MCTruthPointLikeClassifier::classify(JM::NavBuffer* buffer) {
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

    // fallback to point like
    classified_type = ClassificationType::kPointLike;

    for (const auto& track: tracks) {
        auto pdgid = track->getPDGID();

        // todo: should define what is point like

        // if it is a muon, set it as track like
        if (pdgid == 13 or pdgid == -13) {
            classified_type = ClassificationType::kTrackLike;
            break;
        }
        
    }

    return classified_type;
}
