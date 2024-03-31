#include "DetFactorySvc.hh"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"

#include "LSExpDetectorConstruction.hh"
#include "LSExpPhysicsList.hh"
#include "LSExpRunAction.hh"
#include "LSExpSteppingAction.hh"
#include "LSExpTrackingAction.hh"

#include "LSExpMTEventAction.hh"
#include "LSExpMTPrimaryGeneratorAction.hh"
#include "LSExpMTActionInitialization.hh"

#include "Randomize.hh"
#include "DetSimAlg/MgrOfAnaElem.h"
#include "DetSimAlg/DetSimAlg.h"
#include "DetSimAlg/IAnalysisElement.h"

#include "G4PhysListFactory.hh"

#ifdef WITH_G4OPTICKS
#include "OPTICKS_LOG.hh"
#include "G4Opticks.hh"
#endif


DECLARE_SERVICE(DetFactorySvc);

DetFactorySvc::DetFactorySvc(const std::string& name)
    : SvcBase(name)
{
    declProp("AnaMgrList", m_ana_list);
    declProp("CDName", m_cd_name="DetSim1");
    declProp("PMTName", m_pmt_name="PMTMask");
    declProp("LPMTExtra", m_extra_lpmt="ONE"); // ONE or TWO
    declProp("3inchPMTName", m_3inchpmt_name="Tub3inchV3");
    // NOTE: to do the elec simulation, please scale the QE
    declProp("ElecSimPMTQEScale", m_pmt_qe_scale_for_elec=1.0);
    declProp("StrutName", m_strut_name="StrutAcrylic");
    declProp("FastenerName", m_fastener_name="FastenerAcrylic");
    declProp("PMTMother", m_pmt_mother="lWaterPool");
    declProp("StrutMother", m_strut_mother="lWaterPool");
    declProp("FastenerMother", m_fastener_mother="lWaterPool");
    declProp("PMTPosMode", m_pmt_pos_mode="FileMode");
    declProp("PMTPosFile", m_pmt_pos_file);
    declProp("3inchPMTPosFile", m_3inch_pmt_pos_file);
    declProp("3inchPMTPosOffset", m_3inch_pmt_pos_offset=-50.0);
    declProp("StrutPosFile", m_strut_pos_file);
    declProp("FastenerPosFile", m_fastener_pos_file);
    declProp("XJanchorPosFile", m_xjanchor_pos_file);
    declProp("SJCLSanchorPosFile", m_sjclsanchor_pos_file);
    declProp("SJReceiverPosFile", m_sjreceiver_pos_file);
    declProp("SJFixturePosFile", m_sjfixture_pos_file);

    declProp("VetoPMTName", m_veto_pmt_name="R12860OnlyFrontMask");
    declProp("VetoPMTPosMode", m_veto_pmt_pos_mode="CalMode");

    declProp("TTName", m_tt_name="TT_OPERA");
    declProp("CalibUnitEnable", m_cu_enable=true); // enable calib unit by default
    declProp("CalibUnitName", m_cu_name="Calib_GuideTube");  // so Guide Tube is enabled
    declProp("CalibUnitExtras", m_cu_extras);

    declProp("TopChinmeyEnable", m_top_chimney_enable=true);
    declProp("TopChinmeyName", m_chimney_top_name="TopChimney");

    declProp("LowerChinmeyEnable", m_lower_chimney_enable=true);
    declProp("LowerChinmeyName", m_chimney_lower_name="LowerChimney");

    declProp("PhysicsList", m_physics_lists_name="JUNO");

    // flags to enable specific components
    declProp("CDEnabled", m_cd_enabled=true);
    declProp("WPEnabled", m_wp_enabled=true);
    declProp("TTEnabled", m_tt_enabled=true);

    declProp("OpticksMode", m_opticksMode=0);
}

DetFactorySvc::~DetFactorySvc()
{

}

bool
DetFactorySvc::initialize()
{
    return true;
}



bool
DetFactorySvc::finalize()
{
    return true;
}

G4VUserDetectorConstruction* 
DetFactorySvc::createDetectorConstruction()
{
    LSExpDetectorConstruction* dc = new LSExpDetectorConstruction;
    dc->setMTmode(true); // MT
    dc->setScope(getParent());
    dc->setCDEnabled(m_cd_enabled);
    dc->setCDName(m_cd_name);
    dc->setPMTName(m_pmt_name);
    dc->setLPMTExtra(m_extra_lpmt);
    dc->setPMTQEScale(m_pmt_qe_scale_for_elec);
    dc->setPMTMother(m_pmt_mother);
    dc->setPMTPosMode(m_pmt_pos_mode);
    dc->setPMTPosFile(m_pmt_pos_file);
    dc->set3inchPMTName(m_3inchpmt_name);
    dc->set3inchPMTPosFile(m_3inch_pmt_pos_file);
    dc->set3inchPMTPosOffset(m_3inch_pmt_pos_offset);
 
    dc->setStrutName(m_strut_name); 
    dc->setStrutMother(m_strut_mother);
    dc->setStrutPosFile(m_strut_pos_file);

    dc->setFastenerName(m_fastener_name);
    dc->setFastenerMother(m_fastener_mother);
    dc->setFastenerPosFile(m_fastener_pos_file);


    dc->setXJanchorPosFile(m_xjanchor_pos_file);
    dc->setXJfixturePosFile(m_xjanchor_pos_file);
    dc->setSJCLSanchorPosFile(m_sjclsanchor_pos_file);
    dc->setSJReceiverPosFile(m_sjreceiver_pos_file);
    dc->setSJFixturePosFile(m_sjfixture_pos_file);

    dc->setWPEnabled(m_wp_enabled);
    dc->setVetoPMTName(m_veto_pmt_name);
    dc->setVetoPMTPosMode(m_veto_pmt_pos_mode);

    dc->setTTEnabled(m_tt_enabled);
    dc->setTTName(m_tt_name);

    // Calib Unit Related
    if (m_cu_enable) {
        dc->setCalibUnitName(m_cu_name);
        dc->setCalibUnitExtras(m_cu_extras);
    }

    // Chimney Related
    if (m_top_chimney_enable) {
        dc->setTopChimneyName(m_chimney_top_name);
    }
    if (m_lower_chimney_enable) {
        dc->setLowerChimneyName(m_chimney_lower_name);
    }
    dc->setOpticksMode(m_opticksMode);
    return dc;
}

G4VUserPhysicsList* 
DetFactorySvc::createPhysicsList()
{
    if (m_physics_lists_name=="JUNO") {
        LSExpPhysicsList* phylist = new LSExpPhysicsList;
        phylist->setScope(getParent());
        return phylist;
    }
    G4PhysListFactory *physListFactory = new G4PhysListFactory();
    G4VUserPhysicsList *physicsList =
        physListFactory->GetReferencePhysList(m_physics_lists_name);
    assert(physicsList);
    return physicsList;
}

G4VUserPrimaryGeneratorAction* 
DetFactorySvc::createPrimaryGenerator()
{
    LSExpMTPrimaryGeneratorAction* pga = new LSExpMTPrimaryGeneratorAction;
    // pga->setScope(getParent());
    pga->setMTmode(true);
    return pga;
}

G4UserRunAction*  
DetFactorySvc::createRunAction() 
{
    return new LSExpRunAction;
}

G4UserEventAction*  
DetFactorySvc::createEventAction() 
{
    return new LSExpMTEventAction;
}

G4UserStackingAction*  
DetFactorySvc::createStackingAction() 
{
    return NULL;
} 

G4UserTrackingAction*  
DetFactorySvc::createTrackingAction() 
{
    return new LSExpTrackingAction;
} 

G4UserSteppingAction*  
DetFactorySvc::createSteppingAction() 
{
    return new LSExpSteppingAction;
} 

G4VUserActionInitialization*
DetFactorySvc::createUserActionInitialization()
{
    return new LSExpMTActionInitialization();
}


 
bool DetFactorySvc::initializeOpticks()
{
    dumpOpticks("DetFactorySvc::initializeOpticks"); 
    assert( m_opticksMode > 0); 

#ifdef WITH_G4OPTICKS
    OPTICKS_ELOG("DetFactorySvc"); 
#else
    LogError << " FATAL : non-zero opticksMode **NOT** WITH_G4OPTICKS " << std::endl ; 
    assert(0); 
#endif
    return true ; 
}

bool DetFactorySvc::finalizeOpticks()
{
    dumpOpticks("DetFactorySvc::finalizeOpticks"); 
    assert( m_opticksMode > 0); 

#ifdef WITH_G4OPTICKS
    G4Opticks::Finalize(); 
#else
    LogError << " FATAL : non-zero opticksMode **NOT** WITH_G4OPTICKS " << std::endl ; 
    assert(0); 
#endif
    return true;
}

void DetFactorySvc::dumpOpticks(const char* msg) 
{
    LogInfo
       << msg 
       << " m_opticksMode " << m_opticksMode
#ifdef WITH_G4OPTICKS
       << " WITH_G4OPTICKS "
#else
       << " **NOT** WITH_G4OPTICKS " 
#endif
       << std::endl 
       ;
    assert( m_opticksMode > 0); 
}
