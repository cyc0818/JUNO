#include "ClassificationTool/ClassificationType.h"

#include <stdexcept>


std::ostream& operator<<(std::ostream& os, ClassificationType classified_type) {
#define CASE(T) case T: os << #T; break 
    switch (classified_type) {
        CASE(ClassificationType::kUnknown);
        CASE(ClassificationType::kCentralDetector);
        CASE(ClassificationType::kWaterPool);
        CASE(ClassificationType::kTopTracker);
        CASE(ClassificationType::kPointLike);
        CASE(ClassificationType::kTrackLike);
        CASE(ClassificationType::kMuonSingle);
        CASE(ClassificationType::kMuonBundles);
        CASE(ClassificationType::kMuonStop);
        CASE(ClassificationType::kPromptSignal);
        CASE(ClassificationType::kDelayedSignal);
        CASE(ClassificationType::kInEnergyRange);
        CASE(ClassificationType::kInFiducialVolume);
        CASE(ClassificationType::kSingle);
        CASE(ClassificationType::kAfterPulse);
    default:
        throw std::runtime_error("Unsupported ClassificationType in ostream");
        break;
    }
#undef CASE
    return os;
}
