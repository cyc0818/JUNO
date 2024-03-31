#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "Event/WpWaveformHeader.h"
#include "Event/WpElecHeader.h"
#include "Event/WpCalibHeader.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "WaterPoolClassifier.h"

DECLARE_TOOL(WaterPoolClassifier);

WaterPoolClassifier::WaterPoolClassifier(const std::string& name)
    : ToolBase(name) {

}

WaterPoolClassifier::~WaterPoolClassifier() {

}

bool WaterPoolClassifier::configure() {

    return true;
}

ClassificationType WaterPoolClassifier::classify(JM::NavBuffer* buffer) {
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
    if (JM::getHeaderObject<JM::WpWaveformHeader>(nav)) {
        classified_type = ClassificationType::kWaterPool;
    } else if (JM::getHeaderObject<JM::WpElecHeader>(nav)) {
        classified_type = ClassificationType::kWaterPool;
    } else if (JM::getHeaderObject<JM::WpCalibHeader>(nav)) {
        classified_type = ClassificationType::kWaterPool;
    } 

    return classified_type;
}
