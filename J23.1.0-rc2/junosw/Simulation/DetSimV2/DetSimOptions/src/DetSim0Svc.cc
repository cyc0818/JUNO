#include "DetSim0Svc.hh"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Task.h"

#include "LSExpDetectorConstruction.hh"
#include "LSExpPhysicsList.hh"
#include "LSExpEventAction.hh"
#include "LSExpRunAction.hh"
#include "LSExpSteppingAction.hh"
#include "LSExpTrackingAction.hh"
#include "LSExpPrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include "DetSimAlg/MgrOfAnaElem.h"
#include "DetSimAlg/DetSimAlg.h"
#include "DetSimAlg/IAnalysisElement.h"

#include "G4PhysListFactory.hh"

#ifdef WITH_G4CXOPTICKS
#include "OPTICKS_LOG.hh"
#include "G4CXOpticks.hh"
#include "PMTSim/OK_PMTSIM_LOG.hh"
#include "PhysiSim/OK_PHYSISIM_LOG.hh"
#endif

#include "DsWrapPhysConsOptical.h"

DECLARE_SERVICE(DetSim0Svc);

DetSim0Svc::DetSim0Svc(const std::string& name)
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

    declProp("3inchPMTPosFile", m_3inch_pmt_pos_file);
    declProp("3inchPMTPosOffset", m_3inch_pmt_pos_offset=-50.0);
    declProp("StrutPosFile", m_strut_pos_file);
    declProp("Strut2PosFile", m_strut2_pos_file);
    declProp("FastenerPosFile", m_fastener_pos_file);
    declProp("XJanchorPosFile", m_xjanchor_pos_file);
    declProp("SJCLSanchorPosFile", m_sjclsanchor_pos_file);
    declProp("SJReceiverPosFile", m_sjreceiver_pos_file);
    declProp("SJFixturePosFile", m_sjfixture_pos_file);

    declProp("VetoPMTName", m_veto_pmt_name="R12860OnlyFrontMask");
    declProp("VetoPMTPosMode", m_veto_pmt_pos_mode="FileMode");
    declProp("VetoPMTPosFile", m_veto_pmt_pos_file);
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
    
 //   declProp("GdLSAbsLengthMode", m_GdLSAbsLengthMode="old");
    declProp("UsePMTOpticalModel", m_pmt_optical_model = "old");
    declProp("UseLSOpticalModel", m_LS_optical_model = "old");

    declProp("CDInnerReflectorEnabled", m_isCDInnerReflectorEnabled = true);
    
    declProp("UsePmtSimSvc",m_use_pmtsimsvc=true);

}

DetSim0Svc::~DetSim0Svc()
{

}

bool
DetSim0Svc::initialize()
{
    if(m_opticksMode > 0)
    {
        initializeOpticks();
    }

    MgrOfAnaElem& mgr = MgrOfAnaElem::instance();
    // register
    if (m_ana_list.size() == 0) {
        LogWarn << "No Analysis Element is found." << std::endl;
        // m_ana_list.push_back("NormalAnaMgr");
    }
    // TODO
    // user can create the Ana Element more Early
    SniperPtr<DetSimAlg> detsimalg(*getParent(),"DetSimAlg");
    if (detsimalg.invalid()) {
        LogError << "Can't Load DetSimAlg" << std::endl;
        return false;
    }
    for (std::vector<std::string>::iterator it = m_ana_list.begin();
            it != m_ana_list.end();
            ++it) {
        ToolBase* t = 0;
        // find the tool first
        t = detsimalg->findTool(*it);
        //
        // create the tool if not exist
        if (not t) {
            t = detsimalg->createTool(*it);
        }
        //
        // register the tool into MgrOfAnaElem
        if (t) {
            IAnalysisElement* ae = dynamic_cast<IAnalysisElement*>(t);
            if (ae) {
                mgr.reg(*it, ae);
                LogInfo << "Register AnaMgr " << *it << std::endl;
                continue;
            }
        } 
        LogError << "Can't Load Tool " << *it << std::endl;
        return false;
    }

    //To provide option to the Geant4 referrence physics list, the following code will 
    //get the customized optical process of JUNO  at first.
    
    std::string toolname;
    toolname = "DsPhysConsOptical";
    optical_phy = detsimalg->findTool(toolname);
    if (not optical_phy) {
        optical_phy = detsimalg->createTool(toolname);
    }
    assert(optical_phy);
 
    return true;
}



bool
DetSim0Svc::finalize()
{
    if(m_opticksMode > 0 )
    {
        finalizeOpticks();
    }

    return true;
}

G4VUserDetectorConstruction* 
DetSim0Svc::createDetectorConstruction()
{
    LSExpDetectorConstruction* dc = new LSExpDetectorConstruction;
    dc->setScope(dynamic_cast<Task*>(getParent()));
    dc->setCDEnabled(m_cd_enabled);
    dc->setCDName(m_cd_name);
    dc->setPMTName(m_pmt_name);
    dc->setLPMTExtra(m_extra_lpmt);
    dc->setPMTQEScale(m_pmt_qe_scale_for_elec);
    dc->setPMTMother(m_pmt_mother);
    dc->setPMTPosMode(m_pmt_pos_mode);

    dc->set3inchPMTName(m_3inchpmt_name);
    dc->set3inchPMTPosFile(m_3inch_pmt_pos_file);
    dc->set3inchPMTPosOffset(m_3inch_pmt_pos_offset);
 
    dc->setStrutName(m_strut_name); 
    dc->setStrutMother(m_strut_mother);
    dc->setStrutPosFile(m_strut_pos_file);
    dc->setStrut2PosFile(m_strut2_pos_file);
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
    dc->setVetoPMTPosFile(m_veto_pmt_pos_file);
    dc->setTTEnabled(m_tt_enabled);
    dc->setTTName(m_tt_name);
    
    dc->setUsePmtSimSvc(m_use_pmtsimsvc);

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

    //dc->setGdLSAbsLengthMode(m_GdLSAbsLengthMode);
    dc->setPMTOpticalModel(m_pmt_optical_model);
    dc->setLSOpticalModel(m_LS_optical_model);

    dc->setCDInnerReflector(m_isCDInnerReflectorEnabled);

    return dc;
}

G4VUserPhysicsList* 
DetSim0Svc::createPhysicsList()
{
    if (m_physics_lists_name=="JUNO") {
        LSExpPhysicsList* phylist = new LSExpPhysicsList;
        phylist->setScope(dynamic_cast<Task*>(getParent()));
        return phylist;
    }
    G4VUserPhysicsList * physicslist = NULL;
    G4PhysListFactory *physListFactory = new G4PhysListFactory();
    modularPhysicsList = 
        physListFactory->GetReferencePhysList(m_physics_lists_name);
    assert(modularPhysicsList);
    // Register the optical process to the modularPhysicsList;
    // Since the DsPhysConsOptical is a sniper tool , we have to define a new wrapper class DsWrapPhysConsOptical
    // to complete the registration task. 
    DsWrapPhysConsOptical * wrap_op = new DsWrapPhysConsOptical;
    assert(wrap_op);
    wrap_op->SetOpticalProcess(dynamic_cast<G4VPhysicsConstructor*>(optical_phy));
    modularPhysicsList->RegisterPhysics(dynamic_cast<G4VPhysicsConstructor*>(wrap_op));
    LogInfo << " the replace physics list is : " << m_physics_lists_name << std::endl;
    physicslist = modularPhysicsList;
    return physicslist;
}

G4VUserPrimaryGeneratorAction* 
DetSim0Svc::createPrimaryGenerator()
{
    LSExpPrimaryGeneratorAction* pga = new LSExpPrimaryGeneratorAction;
    pga->setScope(dynamic_cast<Task*>(getParent()));
    return pga;
}

G4UserRunAction*  
DetSim0Svc::createRunAction() 
{
    return new LSExpRunAction;
}

G4UserEventAction*  
DetSim0Svc::createEventAction() 
{
   return new LSExpEventAction;
}

G4UserStackingAction*  
DetSim0Svc::createStackingAction() 
{
    return NULL;
} 

G4UserTrackingAction*  
DetSim0Svc::createTrackingAction() 
{
    return new LSExpTrackingAction;
} 

G4UserSteppingAction*  
DetSim0Svc::createSteppingAction() 
{
    return new LSExpSteppingAction;
} 




 
bool DetSim0Svc::initializeOpticks()
{
    dumpOpticks("DetSim0Svc::initializeOpticks"); 
    assert( m_opticksMode > 0); 

#ifdef WITH_G4CXOPTICKS
    // TODO: arrange logging setup earlier than this 
    OPTICKS_ELOG("DetSim0Svc_CXOK"); 
    OK_PMTSIM_LOG_(0) ;
    OK_PHYSISIM_LOG_(0) ;
#else
    LogError << " FATAL : non-zero opticksMode **NOT** WITH_G4CXOPTICKS " << std::endl ; 
    assert(0); 
#endif
    return true ; 
}

bool DetSim0Svc::finalizeOpticks()
{
    dumpOpticks("DetSim0Svc::finalizeOpticks"); 
    assert( m_opticksMode > 0); 

#ifdef WITH_G4CXOPTICKS
    G4CXOpticks::Finalize(); 
#else
    LogError << " FATAL : non-zero opticksMode **NOT** WITH_G4CXOPTICKS  " << std::endl ; 
    assert(0); 
#endif
    return true;
}

void DetSim0Svc::dumpOpticks(const char* msg) 
{
    LogInfo
       << msg 
       << " m_opticksMode " << m_opticksMode
#ifdef WITH_G4CXOPTICKS
       << " WITH_G4CXOPTICKS "
#else
       << " **NOT** WITH_G4CXOPTICKS " 
#endif
       << std::endl 
       ;
    assert( m_opticksMode > 0); 
}
