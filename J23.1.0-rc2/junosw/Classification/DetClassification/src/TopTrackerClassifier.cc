#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "Event/TtElecHeader.h"
#include "Event/TtCalibHeader.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "TopTrackerClassifier.h"

DECLARE_TOOL(TopTrackerClassifier);

TopTrackerClassifier::TopTrackerClassifier(const std::string& name)
    : ToolBase(name) {

}

TopTrackerClassifier::~TopTrackerClassifier() {

}

bool TopTrackerClassifier::configure() {

    return true;
}

ClassificationType TopTrackerClassifier::classify(JM::NavBuffer* buffer) {
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

    // check cd waveform
    if (JM::getHeaderObject<JM::TtElecHeader>(nav)) {
        classified_type = ClassificationType::kTopTracker;
    } else if (JM::getHeaderObject<JM::TtCalibHeader>(nav)) {
        classified_type = ClassificationType::kTopTracker;
    } 

    return classified_type;
}
