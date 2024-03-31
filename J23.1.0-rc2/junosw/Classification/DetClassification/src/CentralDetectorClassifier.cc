#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtElecHeader.h"
#include "Event/CdSpmtElecHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "CentralDetectorClassifier.h"

DECLARE_TOOL(CentralDetectorClassifier);

CentralDetectorClassifier::CentralDetectorClassifier(const std::string& name)
    : ToolBase(name) {

}

CentralDetectorClassifier::~CentralDetectorClassifier() {

}

bool CentralDetectorClassifier::configure() {

    return true;
}

ClassificationType CentralDetectorClassifier::classify(JM::NavBuffer* buffer) {
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
    if (JM::getHeaderObject<JM::CdWaveformHeader>(nav)) {
        classified_type = ClassificationType::kCentralDetector;
    } else if (JM::getHeaderObject<JM::CdLpmtElecHeader>(nav)) {
        classified_type = ClassificationType::kCentralDetector;
    } else if (JM::getHeaderObject<JM::CdSpmtElecHeader>(nav)) {
        classified_type = ClassificationType::kCentralDetector;
    } else if (JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav)) {
        classified_type = ClassificationType::kCentralDetector;
    } else if (JM::getHeaderObject<JM::CdSpmtCalibHeader>(nav)) {
        classified_type = ClassificationType::kCentralDetector;
    }

    return classified_type;
}
