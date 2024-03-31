#include <boost/python.hpp>
#include "ClassificationAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/SniperLog.h"

DECLARE_ALGORITHM(ClassificationAlg);

ClassificationAlg::ClassificationAlg(const std::string& name)
    : AlgBase(name) {
    declProp("ClassifierNames", m_classifierNames);
}

ClassificationAlg::~ClassificationAlg() {

}

bool ClassificationAlg::initialize() {

    // the event buffer
    SniperDataPtr<JM::NavBuffer> navbuf(getParent(), "/Event");
    if (navbuf.invalid()){
        LogError<<"can not get navbuffer!!!"<<std::endl;
        return false;
    }
    m_buffer=navbuf.data();

    // the classifers 
    for (const auto& name: m_classifierNames) {
        IClassificationTool* ct = tool<IClassificationTool>(name);
        if (not ct) {
            LogError << "Failed to find the ClassifierTool: " << name << std::endl;
            return false;
        }
        // initialize eash classifier
        ct->configure();

        m_classifiers.push_back(ct);
    }

    return true;
}

bool ClassificationAlg::execute() {
    // check if there is any event in the buffer
    if (not m_buffer->size()) {
        LogError << "There is no event in the data buffer. " << std::endl;
        return false;
    }

    // store all the types which are not kUnknown
    std::vector<ClassificationType> classified_types;

    for (auto& classifier: m_classifiers) {
        auto ct = classifier->classify(m_buffer);

        // find the type, break the loop
        if (ct != ClassificationType::kUnknown) {
            classified_types.push_back(ct);
        }
    }

    // set flags in event navigator
    auto nav = m_buffer->curEvt();

    LogInfo << "Start to classifiy an Event: " << std::endl;
    for (auto classified_type: classified_types) {
        LogInfo << "Classify event to " << classified_type << std::endl;

        if (classified_type == ClassificationType::kUnknown) {
            continue;
        } else if (classified_type == ClassificationType::kCentralDetector) {
            nav->setDetectorType(JM::EvtNavigator::DetectorType::CD);
        } else if (classified_type == ClassificationType::kWaterPool) {
            nav->setDetectorType(JM::EvtNavigator::DetectorType::WP);
        } else if (classified_type == ClassificationType::kTopTracker) {
            nav->setDetectorType(JM::EvtNavigator::DetectorType::TT);
        } else if (classified_type == ClassificationType::kTrackLike) {
            nav->setTrackLike();
        } else if (classified_type == ClassificationType::kPointLike) {
            nav->setPointLike();
        } else if (classified_type == ClassificationType::kMuonSingle) {
            nav->setMuonType(JM::EvtNavigator::MuonType::single);
        } else if (classified_type == ClassificationType::kMuonBundles) {
            nav->setMuonType(JM::EvtNavigator::MuonType::bundle);
        }

    }

    return true;
}

bool ClassificationAlg::finalize() {
    return true;
}
