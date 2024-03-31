#ifndef ClassificationType_h
#define ClassificationType_h

/*
 * Description:
 *   The classified type. 
 *
 * Author:
 *   Tao Lin <lintao@ihep.ac.cn>
 *   May 30, 2022
 */

#include <ostream>

enum class ClassificationType {
    kUnknown = 0,
    // Sub-detector: CD, WP, TT
    kCentralDetector,
    kWaterPool,
    kTopTracker,
    // Point or Track
    kPointLike,
    kTrackLike,
    // Muon related
    kMuonSingle,
    kMuonBundles,
    kMuonStop,
    // Prompt and delayed signal
    kPromptSignal,
    kDelayedSignal,
    // energy and position
    kInEnergyRange,
    kInFiducialVolume,
    kAfterPulse,
    kSingle,
    // Size of types
    kSize // Please keep this as the last element
};

std::ostream& operator<<(std::ostream& os, ClassificationType classified_type);

#endif
