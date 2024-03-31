#ifndef OECOutputHelper_hh
#define OECOutputHelper_hh

/*
 * Description:
 *     This helper class is used to store the waveform data or t/q data.
 *     The original implementation could be found in OECProcessorAlg by Xin Huang. 
 *
 *     OECTagSvc is responsible to tell the helper waveform or t/q format will be used.
 *
 * Author:
 *     Tao Lin <lintao AT ihep.ac.cn>
 *
 */


#include "OECTagSvc/OECTagSvc.h"

// data model
#include "EvtNavigator/EvtNavHelper.h"

#include "Event/CdWaveformHeader.h"
#include "Event/WpWaveformHeader.h"
#include "Event/GenHeader.h"
#include "Event/SimHeader.h"
#include "Event/CdLpmtElecTruthHeader.h"
#include "Event/CdSpmtElecTruthHeader.h"
#include "Event/WpElecTruthHeader.h"
#include "Event/TtElecTruthHeader.h"
#include "Event/TrackElecTruthHeader.h"
#include "Event/CdTriggerHeader.h"
#include "Event/WpTriggerHeader.h"
#include "Event/TtTriggerHeader.h"
#include "Event/CdLpmtElecHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtElecHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "Event/WpElecHeader.h"
#include "Event/WpCalibHeader.h"
#include "Event/TtElecHeader.h"
#include "Event/TtCalibHeader.h"

#include "Event/OecHeader.h"

#include "TQConvertor/TQConvertor.h"


struct OECOutputHelper {

    OECOutputHelper(OECTagSvc*);
    ~OECOutputHelper();

    // copy contents from src to dst. save t/q or waveform will be decided
    // by OECTagSvc
    bool copy(JM::EvtNavigator* src, JM::EvtNavigator* dst);

private:
    OECTagSvc* m_tagsvc;

    // converter from calib to elec
    TQConvertor* m_tqconvertor;
};

#endif
