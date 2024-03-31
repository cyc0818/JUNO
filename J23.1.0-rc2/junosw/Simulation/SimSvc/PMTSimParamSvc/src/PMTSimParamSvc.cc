#include <boost/python.hpp>
#include "PMTSimParamSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SharedElemFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "Geometry/IPMTParamSvc.h"
#include "Identifier/CdID.h"
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TRandom.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>
#include <iostream> 
#include <string>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>


///// definition for Service
DECLARE_SERVICE(PMTSimParamSvc);
DECLARE_CAN_BE_SHARED(PMTSimParamSvc);

PMTSimParamSvc::PMTSimParamSvc(const std::string& name) 
    : 
    SvcBase(name),
    m_all_pmtID(m_data.m_all_pmtID),
    m_all_pmtID_qe_scale(m_data.m_all_pmtID_qe_scale),
    m_db_path_SPMT(),
    m_PmtTotal_SPMT(m_data.m_PmtTotal_SPMT),
    pd_map_SPMT(m_data.pd_map_SPMT),
    m_map_pmt_category(m_data.m_map_pmt_category),
    m_all_pmt_category(m_data.m_all_pmt_category),
    m_all_pmt_catvec(m_data.m_all_pmt_catvec),
    m_db_type("File"), 
    m_db_path_CD_LPMT(),
    m_db_path_WP_LPMT(),
    m_insertVersion(),
    m_PmtTotal(m_data.m_PmtTotal), // _LPMT
    pd_vector(m_data.pd_vector),
    m_PmtTotal_ALL(m_data.m_PmtTotal_ALL),
    m_PmtTotal_WP(m_data.m_PmtTotal_WP),
    m_pde_scale_Hamamatsu(0.),
    m_pde_scale_NNVT(0.),
    m_QEshape_HZC(m_data.m_QEshape_HZC),
    m_QEshape_NNVT(m_data.m_QEshape_NNVT),
    m_QEshape_NNVT_HiQE(m_data.m_QEshape_NNVT_HiQE),
    m_QEshape_R12860(m_data.m_QEshape_R12860),
    m_QEshape_WP_PMT(m_data.m_QEshape_WP_PMT),
    m_PMT_MPT(m_data.m_PMT_MPT),
    m_PMT_CONST(m_data.m_PMT_CONST)
{
    declProp("DBType", m_db_type);
    declProp("Path", m_db_path_CD_LPMT);
    declProp("Path_WP", m_db_path_WP_LPMT);
    declProp("Path_SPMT", m_db_path_SPMT);
    declProp("InsertVersion", m_insertVersion="v1");
    declProp("PmtTotal", m_PmtTotal=17612); 
    // scale pde to potting pmts
    declProp("PDEScaleHamamatsu", m_pde_scale_Hamamatsu=1);
    declProp("PDEScaleNNVT", m_pde_scale_NNVT=1);
    declProp("DynodeAP1_Prob", dynodeAP1_prob = 0.591*6.57/100);
    declProp("DynodeAP1_Time", dynodeAP1_mu = 4.03e3);
    declProp("DynodeAP1_Sigma", dynodeAP1_sigma = 1.48e3);
    declProp("DynodeAP2_Prob", dynodeAP2_prob = 0.276*1.99/100);
    declProp("DynodeAP2_Time", dynodeAP2_mu = 15.54e3);
    declProp("DynodeAP2_Sigma", dynodeAP2_sigma = 30.1e3);
    declProp("MCPAP1_Prob", MCPAP1_prob = 0.152*15.5/100);
    declProp("MCPAP1_Time", MCPAP1_mu = 0.898e3);
    declProp("MCPAP1_Sigma", MCPAP1_sigma = 0.076e3);
    declProp("MCPAP2_Prob", MCPAP2_prob = 0.145*11.4/100);
    declProp("MCPAP2_Time", MCPAP2_mu = 3.116e3);
    declProp("MCPAP2_Sigma", MCPAP2_sigma = 0.279e3);
    declProp("MCPAP3_Prob", MCPAP3_prob = 0.135*9.1/100);
    declProp("MCPAP3_Time", MCPAP3_mu = 4.543e3);
    declProp("MCPAP3_Sigma", MCPAP3_sigma = 0.390e3);
    declProp("CEFlatValue", m_ce_flat_value=0.9);
}



PMTSimParamSvc::~PMTSimParamSvc()
{
}

bool PMTSimParamSvc::initialize()
{
  LogInfo << "PMTSimParamSvc is initializing!" << std::endl;
  LogInfo << "db_type: " << m_db_type << std::endl;
  bool st;

  // init global info
  st = init_default();
  if (!st) { return st; }

  // Initialize variables From PMTParamSvc. 
  // So initializing PMTParamSvc before PMTSimParamSvc is required.
  st = init_PMTParamSvc();
  if (!st) { return st; }


  // FILE MODE
  if (m_db_type == "File") {
      // CD-LPMT
      st = init_file_CD_LPMT();
      if (!st) { return st; }
      // CD-SPMT
      st = init_file_CD_SPMT();
      if (!st) { return st; }
      // WP-LPMT
      st = init_file_WP_LPMT();
      if (!st) { return st; }


  } else {
      LogError << "Unsupported DB type " << m_db_type << std::endl;
      return false;
  }


  init_all_pmtID_qe_scale(); 


  double tt_angle[9] = {0, 14, 30, 42.5, 55, 67, 77.5, 85, 90};
  double tt_ratio[9] = {0., 0., 0.2142, 4.5757, 6.2484, 9.1953, 8.9885, 7.87506618, 7.1328014};
  gTT_MCP = new TGraph(9, tt_angle, tt_ratio);

  double amp_angle[9] = {-80, -60, -40, -20, 0, 20, 40, 60, 80};
  double amp_ratio[9] = {0.177,0.0479,0.04835,0.0445,0.0433,0.0552,0.0403,0.0372,0.1849};
  gAmp_MCP = new TGraph(9, amp_angle, amp_ratio );

  /*
  double ce_angle_dynode[9] = {0, 13, 28, 41, 55, 66, 79, 85, 90};
  double ce_dynode[9] = {0.873, 0.873, 0.888, 0.896, 0.881, 0.9, 0.881, 0.627, 0.262};
  gCE_Dynode = new TGraph(9, ce_angle_dynode, ce_dynode);

  double ce_angle_mcp[9] = {0, 14, 30, 42.5, 55, 67, 77.5, 85, 90};
  double ce_mcp[9]    = {0.900, 0.900, 0.845, 0.801, 0.775, 0.802, 0.802, 0.771, 0.660};
  gCE_MCP = new TGraph(9, ce_angle_mcp, ce_mcp);

  double ce_R12860[9] = {0.911, 0.911, 0.9222, 0.9294, 0.9235, 0.93, 0.9095, 0.6261, 0.2733};
  gCE_R12860 = new TGraph(9, ce_angle_dynode, ce_R12860);

  double ce_NNVTMCP[9] = {1.0, 1.0, 0.9453, 0.9105, 0.8931, 0.9255, 0.9274, 0.8841, 0.734};
  gCE_NNVTMCP = new TGraph(9, ce_angle_mcp, ce_NNVTMCP);

  double ce_NNVTMCP_HiQE[9] = {1.0, 1.0, 0.9772, 0.9723, 0.9699, 0.9697, 0.9452, 0.9103, 0.734};
  gCE_NNVTMCP_HiQE = new TGraph(9, ce_angle_mcp, ce_NNVTMCP_HiQE);
*/


  m_ce_func = new TF1("ce_func", "0.9*[0]/(1+[1]*exp(-[2]*x))", 0, 90);
  m_ce_func->SetParameter(0, 0.9194);
  m_ce_func->SetParameter(1, 0.5040);
  m_ce_func->SetParameter(2, 0.08067);

  return st;
}


void PMTSimParamSvc::init_all_pmtID_qe_scale()
{
    m_all_pmtID.clear();

    // pd_vector will contain both CD-LPMT and WP-LPMT
    assert( m_PmtTotal+m_PmtTotal_WP == int(pd_vector.size()) ); 

    if (m_PmtTotal+m_PmtTotal_WP != int(pd_vector.size())) {
        LogError << "Mismatch m_PmtTotal+m_PmtTotal_WP != int(pd_vector.size())." << std::endl;
        LogError << " m_PmtTotal: " << m_PmtTotal << std::endl;
        LogError << " m_PmtTotal_WP: " << m_PmtTotal_WP << std::endl;
        LogError << " int(pd_vector.size()): " << int(pd_vector.size()) << std::endl;
    }

    for(int idx = 0 ; idx < m_PmtTotal ; idx++ )
    {
        int id = pd_vector[idx].pmtId() ; 
        assert( id == idx ); 
        m_all_pmtID.push_back(id);
    }



    m_all_pmt_category = m_PMTParamSvc->getPMTCategoryMap() ;  // this excludes WP 
    for(int idx = 0 ; idx < m_PmtTotal_WP ; idx++ )
    {
        int idx_pd_vector = m_PmtTotal + idx; // index in pd_vector
        int id = pd_vector[idx_pd_vector].pmtId(); // PMT ID from pd_vector
        int pmtid = kOFFSET_WP_PMT + idx ;

        if (id != pmtid) {
            LogError << "Mismatch WP PMTID: "
                     << " idx_pd_vector (m_PmtTotal + idx): " << idx_pd_vector
                     << " id: " << id
                     << " pmtid (kOFFSET_WP_PMT + idx): " << pmtid
                     << std::endl;
        }
 
        m_all_pmtID.push_back(pmtid);
        m_all_pmt_category[pmtid] = kPMT_NNVT ;  // that means WP 
    }




    assert( m_PmtTotal_SPMT == int(pd_map_SPMT.size()) ); 

    for( auto &kv : pd_map_SPMT )
    {
        int id = kv.second.pmtId() ; 
        assert( id == kv.first ); 
        m_all_pmtID.push_back(id);
    }

    m_PmtTotal_ALL = int(m_all_pmtID.size()) ; 

    assert( m_PmtTotal_ALL == m_PmtTotal + m_PmtTotal_WP + m_PmtTotal_SPMT ) ; 


    for(int i=0 ; i < m_PmtTotal_ALL ; i++)
    {
        int pmtid = m_all_pmtID[i] ; 
        double qe_scale = get_pmt_qe_scale(pmtid) ; 
        m_all_pmtID_qe_scale.push_back(qe_scale); 
    }



    typedef std::map<int,int>  II ; 
    typedef II::const_iterator IIT ; 
    const II& ii = m_all_pmt_category ;  
    for(IIT it = ii.begin(); it != ii.end() ; it++)
    {
        m_all_pmt_catvec.push_back(it->second);
    }

    std::cout << descTotal() << std::endl ; 
}


std::string PMTSimParamSvc::descTotal() const 
{
    std::stringstream ss ; 
    ss << "PMTSimParamSvc::descTotal" 
       << std::endl
       << std::setw(30) << " m_PmtTotal        " << m_PmtTotal << std::endl
       << std::setw(30) << " pd_vector.size    " << pd_vector.size() << std::endl
       << std::setw(30) << " m_PmtTotal_SPMT   " << m_PmtTotal_SPMT << std::endl 
       << std::setw(30) << " pd_map_SPMT.size  " << pd_map_SPMT.size() << std::endl
       << std::setw(30) << " m_PmtTotal_WP     " << m_PmtTotal_WP << std::endl 
       << std::setw(30) << " m_PmtTotal_ALL    " << m_PmtTotal_ALL << std::endl 
       << std::setw(30) << " m_all_pmtID.size  " << m_all_pmtID.size() << std::endl
       << std::setw(30) << " pd_vector[0].pmtId() " << pd_vector[0].pmtId() << std::endl 
       << std::setw(30) << " m_all_pmt_catvec.size() " << m_all_pmt_catvec.size() << std::endl
       << std::endl
       ;

    std::string str = ss.str(); 
    return str ; 
}


bool PMTSimParamSvc::helper_pmt_mpt(G4MaterialPropertyVector*& vec, IMCParamsSvc* params, const std::string& name){
    IMCParamsSvc::vec_d2d props;
    bool st = params->Get(name, props);
    if (!st) {
        LogError << "can't find material property: " << name << std::endl;
        return false;
    }
    vec = new G4MaterialPropertyVector(0,0,0);

    int N = props.size();
    if (!N) {
        LogError << "empty material property: " << name << std::endl;
        return false;
    }
    for (int i = 0; i < N; ++i) {
        vec->InsertValues(props[i].get<0>(), props[i].get<1>());
    }
    
    
    LogDebug<<"******** name = "<< name<<" ***********" <<std::endl;
    for (int i = 0; i < N; i++){
          LogDebug<<"energy = " << vec->Energy(i) <<" value = " << (*vec)[i] <<std::endl;   
    }
    
  
   return true;
} 


bool PMTSimParamSvc::finalize()
{
  LogInfo << "PMTSimParamSvc is finalizing!" << std::endl;

  return true;
}


bool PMTSimParamSvc::init_default() {

    SniperPtr<IPMTParamSvc> svc(*getParent(), "PMTParamSvc");
    if (svc.invalid()) {
        LogError << "Can't get PMTParamSvc. We can't initialize PMT." << std::endl;
        assert(0);
        return false;
    } else {
        LogInfo << "Retrieve PMTParamSvc successfully." << std::endl;
        m_PMTParamSvc = svc.data();
    }

    SniperPtr<IMCParamsSvc> mcgt(getParent(), "MCParamsSvc");
    if (mcgt.invalid()) {
        std::cout << "Can't find MCParamsSvc." << std::endl;
        assert(0);
        return false;
   }

    // helper_pmt_ce(m_TT_MCP,mcgt.data(),"PMTProperty.TT_MCP.CE");
    // helper_pmt_ce(m_Amp_MCP,mcgt.data(),"PMTProperty.Amp_MCP.CE");
    helper_pmt_mpt(m_CE_Dynode,mcgt.data(),"PMTProperty.Dynode.CE");
    helper_pmt_mpt(m_CE_MCP,mcgt.data(),"PMTProperty.MCP.CE");
    helper_pmt_mpt(m_CE_R12860,mcgt.data(),"PMTProperty.R12860.CE");
    helper_pmt_mpt(m_CE_NNVTMCP,mcgt.data(),"PMTProperty.NNVTMCP.CE");
    helper_pmt_mpt(m_CE_NNVTMCP_HiQE,mcgt.data(),"PMTProperty.NNVTMCP_HiQE.CE");
    helper_pmt_mpt(m_CE_WP_PMT,mcgt.data(),"PMTProperty.WP_PMT.CE"); 
    //assert(m_Amp_MCP&&m_TT_MCP);

    helper_pmt_mpt(m_QEshape_HZC, mcgt.data(),"PMTProperty.HZC_3inch.QE_shape");
    helper_pmt_mpt(m_QEshape_NNVT, mcgt.data(),"PMTProperty.NNVTMCP.QE_shape");
    helper_pmt_mpt(m_QEshape_NNVT_HiQE, mcgt.data(),"PMTProperty.NNVTMCP_HiQE.QE_shape");
    helper_pmt_mpt(m_QEshape_R12860, mcgt.data(),"PMTProperty.R12860.QE_shape");
    helper_pmt_mpt(m_QEshape_WP_PMT, mcgt.data(),"PMTProperty.WP_PMT.QE_shape");
    assert(m_QEshape_HZC);

    helper_pmt_mpt(m_PMT_MPT[kPMT_Hamamatsu]["ARC_RINDEX"], mcgt.data(), "PMTProperty.R12860.ARC_RINDEX");
    helper_pmt_mpt(m_PMT_MPT[kPMT_Hamamatsu]["ARC_KINDEX"], mcgt.data(), "PMTProperty.R12860.ARC_KINDEX");
    helper_pmt_mpt(m_PMT_MPT[kPMT_Hamamatsu]["PHC_RINDEX"], mcgt.data(), "PMTProperty.R12860.PHC_RINDEX");
    helper_pmt_mpt(m_PMT_MPT[kPMT_Hamamatsu]["PHC_KINDEX"], mcgt.data(), "PMTProperty.R12860.PHC_KINDEX");

    helper_pmt_mpt(m_PMT_MPT[kPMT_NNVT]["ARC_RINDEX"], mcgt.data(), "PMTProperty.NNVTMCP.ARC_RINDEX");
    helper_pmt_mpt(m_PMT_MPT[kPMT_NNVT]["ARC_KINDEX"], mcgt.data(), "PMTProperty.NNVTMCP.ARC_KINDEX");
    helper_pmt_mpt(m_PMT_MPT[kPMT_NNVT]["PHC_RINDEX"], mcgt.data(), "PMTProperty.NNVTMCP.PHC_RINDEX");
    helper_pmt_mpt(m_PMT_MPT[kPMT_NNVT]["PHC_KINDEX"], mcgt.data(), "PMTProperty.NNVTMCP.PHC_KINDEX");
  
    helper_pmt_mpt(m_PMT_MPT[kPMT_NNVT_HighQE]["ARC_RINDEX"], mcgt.data(), "PMTProperty.NNVTMCP_HiQE.ARC_RINDEX");
    helper_pmt_mpt(m_PMT_MPT[kPMT_NNVT_HighQE]["ARC_KINDEX"], mcgt.data(), "PMTProperty.NNVTMCP_HiQE.ARC_KINDEX");
    helper_pmt_mpt(m_PMT_MPT[kPMT_NNVT_HighQE]["PHC_RINDEX"], mcgt.data(), "PMTProperty.NNVTMCP_HiQE.PHC_RINDEX");
    helper_pmt_mpt(m_PMT_MPT[kPMT_NNVT_HighQE]["PHC_KINDEX"], mcgt.data(), "PMTProperty.NNVTMCP_HiQE.PHC_KINDEX");

    IMCParamsSvc::map_s2d ham_thick_map;
    bool st = mcgt->Get("PMTProperty.R12860.THICKNESS", ham_thick_map);
    if(st){
        m_PMT_CONST[kPMT_Hamamatsu]["ARC_THICKNESS"] = ham_thick_map["ARC_THICKNESS"];
        m_PMT_CONST[kPMT_Hamamatsu]["PHC_THICKNESS"] = ham_thick_map["PHC_THICKNESS"];
    }

    IMCParamsSvc::map_s2d nnvt_thick_map;
    st = mcgt->Get("PMTProperty.NNVTMCP.THICKNESS", nnvt_thick_map);
    if(st){
        m_PMT_CONST[kPMT_NNVT]["ARC_THICKNESS"] = nnvt_thick_map["ARC_THICKNESS"];
        m_PMT_CONST[kPMT_NNVT]["PHC_THICKNESS"] = nnvt_thick_map["PHC_THICKNESS"];
    }

    IMCParamsSvc::map_s2d nnvt_HiQE_thick_map;
    st = mcgt->Get("PMTProperty.NNVTMCP_HiQE.THICKNESS", nnvt_HiQE_thick_map);
    if(st){
        m_PMT_CONST[kPMT_NNVT_HighQE]["ARC_THICKNESS"] = nnvt_HiQE_thick_map["ARC_THICKNESS"];
        m_PMT_CONST[kPMT_NNVT_HighQE]["PHC_THICKNESS"] = nnvt_HiQE_thick_map["PHC_THICKNESS"];
    }

    IMCParamsSvc::map_s2d ham_scale_map;
    st = mcgt->Get("PMTProperty.R12860.scale", ham_scale_map);
    if(st){
        m_eff_ce_Hamamatsu = ham_scale_map["eff_ce_Hamamatsu"];
        m_qeshape_at420nm_Hamamatsu = ham_scale_map["qe_shape_at_420nm"];
    }
   
    IMCParamsSvc::map_s2d nnvt_scale_map;
    st = mcgt->Get("PMTProperty.NNVTMCP.scale", nnvt_scale_map);
    if(st){
        m_eff_ce_NNVT_Normal = nnvt_scale_map["eff_ce_NNVT_Normal"];
        m_qeshape_at420nm_NNVT_Normal = nnvt_scale_map["qe_shape_at_420nm"];   
    }

    IMCParamsSvc::map_s2d nnvt_HiQE_scale_map;
    st = mcgt->Get("PMTProperty.NNVTMCP_HiQE.scale", nnvt_HiQE_scale_map);
    if(st){
        m_eff_ce_NNVT_HiQE = nnvt_HiQE_scale_map["eff_ce_NNVT_HiQE"];
        m_qeshape_at420nm_NNVT_HiQE = nnvt_HiQE_scale_map["qe_shape_at_420nm"];  
    }

    IMCParamsSvc::map_s2d hzc_scale_map;
    st = mcgt->Get("PMTProperty.HZC_3inch.scale", hzc_scale_map);
    if(st){
        m_qeshape_at420nm_HZC = hzc_scale_map["qe_shape_at_420nm"];
    }
 
    IMCParamsSvc::map_s2d wp_scale_map;
    st = mcgt->Get("PMTProperty.WP_PMT.scale", wp_scale_map);
    if(st){
        m_qeshape_at420nm_WP = wp_scale_map["qe_shape_at_420nm"];
        m_qereal_at420nm_WP = wp_scale_map["qe_real_at_420nm"];
    }
          


    LogDebug <<" m_eff_ce_NNVT_HiQE = "<<m_eff_ce_NNVT_HiQE 
             <<"\n m_eff_ce_NNVT_Normal = "<<m_eff_ce_NNVT_Normal
             <<"\n m_eff_ce_Hamamatsu "<<m_eff_ce_Hamamatsu 
             << std::endl;
    LogDebug <<" m_qeshape_at420nm_Hamamatsu = "<<m_qeshape_at420nm_Hamamatsu 
             <<"\n m_qeshape_at420nm_NNVT_Normal = "<<m_qeshape_at420nm_NNVT_Normal
             <<"\n m_qeshape_at420nm_NNVT_HiQE = "<<m_qeshape_at420nm_NNVT_HiQE
             <<"\n m_qeshape_at420nm_HZC = "<<m_qeshape_at420nm_HZC
             <<"\n m_qeshape_at420nm_WP = "<< m_qeshape_at420nm_WP
             <<"\n m_qereal_at420nm_WP = "<<m_qereal_at420nm_WP
             << std::endl;

    return true;
}

bool PMTSimParamSvc::init_file_CD_LPMT() {
    bool st = true;

    pd_vector.clear();

    init_file_common_LPMT(pd_vector,
                          m_PmtTotal, // CD-LPMT
                          kOFFSET_CD_LPMT,
                          m_db_path_CD_LPMT,
                          "/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root");

    return st;
}

bool PMTSimParamSvc::init_file_WP_LPMT() {
    bool st = true;

    // append the WP PMT info into pd_vector
    init_file_common_LPMT(pd_vector,
                          m_PmtTotal_WP, // WP-LPMT
                          kOFFSET_WP_PMT,
                          m_db_path_WP_LPMT,
                          "/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_WP_LPMT.root");


    return st;
}

//
// Description: 
//     This method is a common method to load the LPMT related parameters.
//     Both CD-LPMT and WP-LPMT could use this. 
bool PMTSimParamSvc::init_file_common_LPMT(std::vector<PmtSimData_LPMT>& pmtdata_vector,
                                           int N,
                                           int pmtid_offset,
                                           const std::string& custom_path,
                                           const std::string& default_path) {
    bool st = true;

    // file path
    std::string p = custom_path;
    if (custom_path.empty()) {
        const char* env_vars[2] = {"JUNOTOP", "WORKTOP"};
        for (int i = 0; i < 2; ++i) {
            if (getenv(env_vars[i]) == NULL) {
                continue;
            }
            std::string path = getenv(env_vars[i]);
            path += default_path;
            if (boost::filesystem::exists(path)) {
                p = path ;
            }
        }
    }
    
    if (!p.empty() && boost::filesystem::exists(p)) {
        LogInfo << "Loading parameters from file: " << p << std::endl;
    } else {
        LogError << "Can't find PMT parameters file '" << p << "'." << std::endl;
        st = false;
        return st;
    }

    // load data
    auto infile = TFile::Open(p.c_str());
    auto t1 = (TTree*)infile->Get("data");
    int _pmtId;
    double _pde;
    double _gain;
    double _sigmaGain;
    double _afterPulseProb;
    double _prePulseProb;
    double _darkRate;
    double _timeSpread;
    double _timeOffset;
    t1 ->SetBranchAddress("pmtID",&_pmtId);
    t1 ->SetBranchAddress("PDE",&_pde);
    t1 ->SetBranchAddress("Gain",&_gain);
    t1 ->SetBranchAddress("Resolution",&_sigmaGain);
    t1 ->SetBranchAddress("afterPulseProb",&_afterPulseProb);
    t1 ->SetBranchAddress("prePulseProb",&_prePulseProb);
    t1 ->SetBranchAddress("DCR",&_darkRate);
    t1 ->SetBranchAddress("TTS_SS",&_timeSpread);
    t1 ->SetBranchAddress("timeOffset",&_timeOffset);
    for(int i=0; i<N; i++){
        t1->GetEntry(i);
        PmtSimData_LPMT pd(_pmtId + pmtid_offset, // convert to copy number with offset
                _pde,
                _gain,
                _sigmaGain,
                _afterPulseProb,
                _prePulseProb,
                _darkRate,
                _timeSpread,
                _timeOffset
                );

        pmtdata_vector.push_back(pd);
    }


    delete t1;
    delete infile;

    return true;
}


bool PMTSimParamSvc::init_file_CD_SPMT() {
    bool st = true;
    pd_map_SPMT.clear();
    
    if (getenv("JUNOTOP")) {
        std::string path = getenv("JUNOTOP");
        path += "/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root";
        if (boost::filesystem::exists(path)) {
            m_db_path_SPMT = path ;
        }
    }
    if (getenv("WORKTOP")) {
        std::string path = getenv("WORKTOP");
        path += "/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root";
        if (boost::filesystem::exists(path)) {
            m_db_path_SPMT = path ;
        }
    }

    
    if (!m_db_path_SPMT.empty() && boost::filesystem::exists(m_db_path_SPMT)) {
        LogInfo << "Loading parameters from file: " << m_db_path_SPMT << std::endl;
    } else {
        LogError << "Can't find PMT parameters file '" << m_db_path_SPMT << "'." << std::endl;
        st = false;
        return st;
    }

    auto infile = TFile::Open(m_db_path_SPMT.c_str());
    auto t1 = (TTree*)infile->Get("data");
    int _pmtId;
    double _QE;
    double _efficiency;
    double _gain;
    double _sigmaGain;
    double _afterPulseProb;
    double _prePulseProb;
    double _darkRate;
    double _timeSpread;
    double _timeOffset;
    t1 ->SetBranchAddress("pmtID",&_pmtId);
    t1 ->SetBranchAddress("QE", &_QE);
    t1 ->SetBranchAddress("efficiency",&_efficiency);
    t1 ->SetBranchAddress("Gain",&_gain);
    t1 ->SetBranchAddress("sigmaGain",&_sigmaGain);
    t1 ->SetBranchAddress("afterPulseProb",&_afterPulseProb);
    t1 ->SetBranchAddress("prePulseProb",&_prePulseProb);
    t1 ->SetBranchAddress("darkRate",&_darkRate);
    t1 ->SetBranchAddress("timeSpread",&_timeSpread);
    t1 ->SetBranchAddress("timeOffset",&_timeOffset);

    m_PmtTotal_SPMT = t1->GetEntries();
    for(int i=0; i<m_PmtTotal_SPMT; i++){
        t1->GetEntry(i);
        PmtSimData_SPMT pd(_pmtId,
                _QE,
                _efficiency,
                _gain,
                _sigmaGain,
                _afterPulseProb,
                _prePulseProb,
                _darkRate,
                _timeSpread,
                _timeOffset
                );

        pd_map_SPMT.insert(std::pair<int, PmtSimData_SPMT>(_pmtId, pd));
    }
    
    delete t1;
    delete infile;

    return true;
}

bool PMTSimParamSvc::init_PMTParamSvc()
{
    int num_lpmt = m_PMTParamSvc->get_NTotal_CD_LPMT() ; 
    for(int i = 0 ; i < num_lpmt ; i++)
    {
        int pmtid = kOFFSET_CD_LPMT + i ; 
        int cat = m_PMTParamSvc->getPMTCategory(pmtid) ; 
        m_map_pmt_category[i] = cat ; 
    }

    m_PmtTotal_WP = m_PMTParamSvc->get_NTotal_WP_LPMT() ; 


   return true;

}


char* PMTSimParamSvc::get_SN(int /*pmtId*/) const 
{
    //return pd_vector[pmtId].SN();
    return nullptr ;
}


double PMTSimParamSvc::get_gain(int pmtId) const
{   
    double gain = 0;
    if(PMT::Is20inch(pmtId))
    {
        int pmtIdx = m_data.get_pmtContiguousIndex(pmtId);

        if (pmtIdx >= int(pd_vector.size())) {
            std::cerr << "ERROR: pmtIdx >= pd_vector.size(): " 
                      << " pmtId: " << pmtId
                      << " pmtIdx (cont): " << pmtIdx
                      << std::endl;
        }

        gain = pd_vector.at(pmtIdx).gain() / 10000000;
    }else
    {
        if(pd_map_SPMT.count(pmtId)>0)
        {
            gain = pd_map_SPMT.at(pmtId).gain();
        }
    }

    return gain;
}

double PMTSimParamSvc::get_sigmaGain(int pmtId) const 
{   
    double sigmaGain = 0;
    if(PMT::Is20inch(pmtId))
    {
        int pmtIdx = m_data.get_pmtContiguousIndex(pmtId);
        if (pmtIdx >= int(pd_vector.size())) {
            std::cerr << "ERROR: pmtIdx >= pd_vector.size(): " 
                      << " pmtId: " << pmtId
                      << " pmtIdx (cont): " << pmtIdx
                      << std::endl;
        }
        sigmaGain = pd_vector.at(pmtIdx).sigmaGain();
    }
    if(pd_map_SPMT.count(pmtId)>0)
    {
        sigmaGain = pd_map_SPMT.at(pmtId).sigmaGain();
    }
    
    return sigmaGain;
}

double PMTSimParamSvc::get_efficiency(int pmtId) const // SPMT
{   
    if (not PMT::Is3inch(pmtId)) {
        std::cerr << "ERROR: "
                  << "pmtId: " << pmtId << " is not SPMT. "
                  << "Currently PMTSimParamSvc::get_efficiency is used for SPMT only. " 
                  << std::endl;
        return 0;
    }

    return pd_map_SPMT.at(pmtId).efficiency();
}

double PMTSimParamSvc::get_QE(int pmtId) const  // SPMT 
{   
    if (not PMT::Is3inch(pmtId)) {
        std::cerr << "ERROR: "
                  << "pmtId: " << pmtId << " is not SPMT. "
                  << "Currently PMTSimParamSvc::get_QE is used for SPMT only. " 
                  << std::endl;
        return 0;
    }

    return pd_map_SPMT.at(pmtId).QE();
}

double PMTSimParamSvc::get_pde(int pmtId) const 
{
    if (not PMT::Is20inch(pmtId)) {
        std::cerr << "ERROR: "
                  << "pmtId: " << pmtId << " is not CD-LPMT or WP-LPMT. "
                  << "Currently PMTSimParamSvc::get_pde is used for LPMT only. " 
                  << std::endl;
        return 0;
    }

    int cat = kPMT_Unknown;

    if (m_map_pmt_category.count(pmtId)) {
        cat = m_map_pmt_category.at(pmtId); 
    } else if (m_all_pmt_category.count(pmtId)) {
        cat = m_all_pmt_category.at(pmtId);
    } else {
        std::cerr << "ERROR: " 
                  << " Failed to find PMT Category for pmtId "
                  << pmtId
                  << std::endl;
        return 0;
    }

    // FIXME: potting scale factor, should it be placed here?
    double scale_factor = 1.0;
    if (cat == kPMT_Hamamatsu ) 
    {  
        scale_factor = m_pde_scale_Hamamatsu;
    } 
    else if (cat == kPMT_NNVT or cat == kPMT_NNVT_HighQE) 
    {
        scale_factor = m_pde_scale_NNVT;
    }

    int pmtIdx = m_data.get_pmtContiguousIndex(pmtId);
    if (pmtIdx >= int(pd_vector.size())) {
        std::cerr << "ERROR: pmtIdx >= pd_vector.size(): " 
                  << " pmtId: " << pmtId
                  << " pmtIdx (cont): " << pmtIdx
                  << std::endl;
    }

    return pd_vector.at(pmtIdx).pde()*scale_factor /100.;
}

double PMTSimParamSvc::get_dcr(int pmtId) const
{
    double darkRate = 0;
    if(PMT::Is20inch(pmtId))
    {
        int pmtIdx = m_data.get_pmtContiguousIndex(pmtId);
        if (pmtIdx >= int(pd_vector.size())) {
            std::cerr << "ERROR: pmtIdx >= pd_vector.size(): " 
                      << " pmtId: " << pmtId
                      << " pmtIdx (cont): " << pmtIdx
                      << std::endl;
        }
        darkRate = pd_vector.at(pmtIdx).darkRate();
    }else
    {
        if(pd_map_SPMT.count(pmtId)>0)
        {
            darkRate = pd_map_SPMT.at(pmtId).darkRate();
        }
    }
    return darkRate;
}


double PMTSimParamSvc::get_afterPulseProb(int pmtId) const
{
    double afterPulseProb = 0;
    if(PMT::Is20inch(pmtId))
    {
        int pmtIdx = m_data.get_pmtContiguousIndex(pmtId);
        if (pmtIdx >= int(pd_vector.size())) {
            std::cerr << "ERROR: pmtIdx >= pd_vector.size(): " 
                      << " pmtId: " << pmtId
                      << " pmtIdx (cont): " << pmtIdx
                      << std::endl;
        }
        afterPulseProb = pd_vector.at(pmtIdx).afterPulseProb();
    }else
    {
        if(pd_map_SPMT.count(pmtId)>0)
        {
            afterPulseProb = pd_map_SPMT.at(pmtId).afterPulseProb();
        }
    }

    return afterPulseProb;
}

double PMTSimParamSvc::get_prePulseProb(int pmtId) const
{
    double prePulseProb = 0;
    if(PMT::Is20inch(pmtId))
    {
        int pmtIdx = m_data.get_pmtContiguousIndex(pmtId);
        if (pmtIdx >= int(pd_vector.size())) {
            std::cerr << "ERROR: pmtIdx >= pd_vector.size(): " 
                      << " pmtId: " << pmtId
                      << " pmtIdx (cont): " << pmtIdx
                      << std::endl;
        }
        prePulseProb = pd_vector.at(pmtIdx).prePulseProb();
    }else
    {
        if(pd_map_SPMT.count(pmtId)>0)
        {
            prePulseProb = pd_map_SPMT.at(pmtId).prePulseProb();
        }
    }

    return prePulseProb;
}

double PMTSimParamSvc::get_tts(int pmtId) const
{
    double timeSpread = 0;
    if(PMT::Is20inch(pmtId))
    {
        int pmtIdx = m_data.get_pmtContiguousIndex(pmtId);
        if (pmtIdx >= int(pd_vector.size())) {
            std::cerr << "ERROR: pmtIdx >= pd_vector.size(): " 
                      << " pmtId: " << pmtId
                      << " pmtIdx (cont): " << pmtIdx
                      << std::endl;
        }
        timeSpread = pd_vector.at(pmtIdx).timeSpread();
    }else
    {
        if(pd_map_SPMT.count(pmtId)>0)
        {
            timeSpread = pd_map_SPMT.at(pmtId).timeSpread();
        }
    }

    return timeSpread;
}

double PMTSimParamSvc::get_timeOffset(int pmtId) const
{
    double timeOffset = 0;
    if(PMT::Is20inch(pmtId))
    {
        int pmtIdx = m_data.get_pmtContiguousIndex(pmtId);
        if (pmtIdx >= int(pd_vector.size())) {
            std::cerr << "ERROR: pmtIdx >= pd_vector.size(): " 
                      << " pmtId: " << pmtId
                      << " pmtIdx (cont): " << pmtIdx
                      << std::endl;
        }
        timeOffset = pd_vector.at(pmtIdx).timeOffset();
    }else
    {
        if(pd_map_SPMT.count(pmtId)>0)
        {
            timeOffset = pd_map_SPMT.at(pmtId).timeOffset();
        }
    }

    return timeOffset;
}

int PMTSimParamSvc::dynodeAP1_amp()
{
    return gRandom->Poisson(dynodeAP1_prob);
}

double PMTSimParamSvc::dynodeAP1_time()
{
    double tt = -1;
    while (tt<=0 || fabs(dynodeAP1_mu - tt) > 5*dynodeAP1_sigma) {
        tt = gRandom->Gaus(dynodeAP1_mu, dynodeAP1_sigma);
    }
    return tt;
}

int PMTSimParamSvc::dynodeAP2_amp()
{
    return gRandom->Poisson(dynodeAP2_prob);
}

double PMTSimParamSvc::dynodeAP2_time()
{
    double tt = -1;
    while (tt<=0 || fabs(tt - dynodeAP2_mu) > 5*dynodeAP2_sigma) {
        tt = gRandom->Gaus(dynodeAP2_mu, dynodeAP2_sigma);
    }
    return tt;
}

int PMTSimParamSvc::mcpAP1_amp()
{
    return gRandom->Poisson(MCPAP1_prob);
}

double PMTSimParamSvc::mcpAP1_time()
{
    double tt = -1;
    while (tt<=0 || fabs(tt - MCPAP1_mu) > 5*MCPAP1_sigma) {
        tt = gRandom->Gaus(MCPAP1_mu, MCPAP1_sigma);
    }
    return tt;
}

int PMTSimParamSvc::mcpAP2_amp()
{
    return gRandom->Poisson(MCPAP2_prob);
}

double PMTSimParamSvc::mcpAP2_time()
{
    double tt = -1;
    while (tt<=0 || fabs(tt - MCPAP2_mu) > 5*MCPAP2_sigma) {
        tt = gRandom->Gaus(MCPAP2_mu, MCPAP2_sigma);
    }
    return tt;
}

int PMTSimParamSvc::mcpAP3_amp()
{
    return gRandom->Poisson(MCPAP3_prob);
}

double PMTSimParamSvc::mcpAP3_time()
{
    double tt = -1;
    while (tt<=0 || fabs(tt - MCPAP3_mu) > 5*MCPAP3_sigma) {
        tt = gRandom->Gaus(MCPAP3_mu, MCPAP3_sigma);
    }
    return tt;
}

const std::vector<int>&  PMTSimParamSvc::get_all_pmtID() const
{
    return m_all_pmtID ;
}

int  PMTSimParamSvc::get_pmtContiguousIndex(int pmtID) const
{   
    const std::vector<int>& a = m_all_pmtID ; 
    size_t idx = std::distance( a.begin(), std::find( a.begin(), a.end(), pmtID )) ;
    return idx < a.size() ? int(idx) : -1 ;
}

int PMTSimParamSvc::get_pmtId_from_ContiguousIndex(int idx) const
{   
    const std::vector<int>& a = m_all_pmtID ;  
    return idx > -1 && idx < int(a.size()) ? a[idx] : -1 ;
}

void PMTSimParamSvc::getCEData(std::vector<double>& ce_data, double theta0, double theta1, unsigned num_theta) const 
{

    const std::vector<int>& a = m_all_pmtID ;

    int pmtId_first = a.front() ;
    int pmtId_last  = a.back() ;
    assert( pmtId_first == 0 );

    int idx_first = get_pmtContiguousIndex(pmtId_first) ;
    int idx_last  = get_pmtContiguousIndex(pmtId_last) ;
    int num_idx = idx_last - idx_first + 1 ;


    ce_data.clear();
    ce_data.resize( num_idx*num_theta );


    double* ce_data_ = ce_data.data() ;


    for(int idx=0 ; idx < int(a.size()) ; idx++ )
    {
       int pmtID = a[idx] ;
       int idx_check = get_pmtContiguousIndex(pmtID);
       assert( idx == idx_check );

       for(unsigned j=0 ; j < num_theta ; j++)
         {
           double tfrac = double(j)/double(num_theta-1) ;
           double theta = theta0 + (theta1 - theta0)*tfrac ;
           double ce = get_pmtid_ce(pmtID,theta);

           ce_data_[ idx*num_theta + j ] = ce ;
         }
    }
      

}


void PMTSimParamSvc::getQEData(std::vector<double>& qe_data, double en0, double en1, unsigned num_edep  ) const 
{   
    const std::vector<int>& a = m_all_pmtID ;

    int pmtId_first = a.front() ; 
    int pmtId_last  = a.back() ; 
    assert( pmtId_first == 0 ); 
    
    int idx_first = get_pmtContiguousIndex(pmtId_first) ; 
    int idx_last  = get_pmtContiguousIndex(pmtId_last) ;     
    int num_idx = idx_last - idx_first + 1 ;


    qe_data.clear();
    qe_data.resize( num_idx*num_edep );
    

    double* qe_data_ = qe_data.data() ;
    

    for(int idx=0 ; idx < int(a.size()) ; idx++ )
    {
       int pmtID = a[idx] ;
       int idx_check = get_pmtContiguousIndex(pmtID);
       assert( idx == idx_check );

       for(unsigned j=0 ; j < num_edep ; j++)
       {
           double efrac = double(j)/double(num_edep -1 ) ;
           double edep = en0 + (en1 - en0)*efrac ;
           double qe = get_pmtid_qe(pmtID,edep);

           qe_data_[ idx*num_edep + j ] = qe ;
       }
    }
}





double PMTSimParamSvc::get_pmtcat_qe(int pmtcat, double energy) const
{
    double qe = -1 ;
    G4MaterialPropertyVector* vec = get_pmtcat_qe_vs_energy(pmtcat);
    assert(vec);
    qe = vec->Value(energy);
    return qe;   
}

double PMTSimParamSvc::get_pmtid_qe(int pmtid, double energy) const
{
 
  int pmtcat = m_PMTParamSvc->getPMTCategory(pmtid) ;
  assert( pmtcat >= (int)kPMT_Unknown && pmtcat <= (int)kPMT_NNVT_HighQE && pmtcat + 1 >= 0 ); 
  double qe = get_pmtcat_qe(pmtcat, energy);
  double qe_scale = get_pmt_qe_scale(pmtid);
  qe = qe*qe_scale;
  assert(qe > 0 && qe < 1);
  return qe;
}

double PMTSimParamSvc::get_pmt_qe_scale(int pmtid) const
{
      return get_real_qe_at420nm(pmtid)/get_shape_qe_at420nm(pmtid);
}

double PMTSimParamSvc::get_shape_qe_at420nm(int pmtid) const
{
    int pmtcat = m_PMTParamSvc->getPMTCategory(pmtid) ;
    assert( pmtcat >= (int)kPMT_Unknown && pmtcat <= (int)kPMT_NNVT_HighQE && pmtcat + 1 >= 0 );
    double qe_shape_at420nm = -1 ;
    //FIXME:KPMT_Unknown represent WP pmt,which use normal NNVTMCP ?
    switch(pmtcat)
    {   
        case kPMT_Unknown:     qe_shape_at420nm  = m_qeshape_at420nm_WP          ; break ;
        case kPMT_NNVT:        qe_shape_at420nm =  m_qeshape_at420nm_NNVT_Normal ; break ;
        case kPMT_Hamamatsu:   qe_shape_at420nm  = m_qeshape_at420nm_Hamamatsu   ; break ;
        case kPMT_HZC:         qe_shape_at420nm  = m_qeshape_at420nm_HZC         ; break ;
        case kPMT_NNVT_HighQE: qe_shape_at420nm  = m_qeshape_at420nm_NNVT_HiQE   ; break ;
    }
    assert(qe_shape_at420nm  >= 0);
    return qe_shape_at420nm ;
}

double PMTSimParamSvc::get_real_qe_at420nm(int pmtid) const
{
    int pmtcat = m_PMTParamSvc->getPMTCategory(pmtid) ;
    assert( pmtcat >= (int)kPMT_Unknown && pmtcat <= (int)kPMT_NNVT_HighQE && pmtcat + 1 >= 0 );
    double qe_real_at420nm =-1 ;
    //FIXME:KPMT_Unknown represent WP pmt,which use normal NNVTMCP ?
    switch(pmtcat)
    {  
        case kPMT_Unknown:     qe_real_at420nm = m_qereal_at420nm_WP                 ; break ;
        case kPMT_HZC:         qe_real_at420nm = pd_map_SPMT[pmtid].QE()             ; break ;
        case kPMT_NNVT:        qe_real_at420nm = get_pde(pmtid)/m_eff_ce_NNVT_Normal ; break ;
        case kPMT_Hamamatsu:   qe_real_at420nm = get_pde(pmtid)/m_eff_ce_Hamamatsu   ; break ;
        case kPMT_NNVT_HighQE: qe_real_at420nm = get_pde(pmtid)/m_eff_ce_NNVT_HiQE   ; break ;
   }
   assert(qe_real_at420nm>0);
   return qe_real_at420nm;
}


 
G4MaterialPropertyVector*  PMTSimParamSvc::get_pmtcat_qe_vs_energy(int pmtcat) const
{
    assert( pmtcat >= (int)kPMT_Unknown && pmtcat <= (int)kPMT_NNVT_HighQE && pmtcat + 1 >= 0 );
    G4MaterialPropertyVector * vec = 0 ;
    switch(pmtcat)
    {   //FIXME:KPMT_Unknown represent WP pmt,which use normal NNVTMCP ?
        case kPMT_Unknown:     vec = m_QEshape_WP_PMT    ; break ;
        case kPMT_NNVT:        vec = m_QEshape_NNVT      ; break ;
        case kPMT_Hamamatsu:   vec = m_QEshape_R12860    ; break ;
        case kPMT_HZC:         vec = m_QEshape_HZC       ; break ;
        case kPMT_NNVT_HighQE: vec = m_QEshape_NNVT_HiQE ; break ;    
    } 
    assert(vec);
    return vec;
}

double PMTSimParamSvc::get_pmtid_ce(int pmtid , double theta) const
{   
     int pmtcat = m_PMTParamSvc->getPMTCategory(pmtid);
     double ce = get_pmtcat_ce(pmtcat,theta);
     assert( ce > 0 );
     return  ce ;

}

double PMTSimParamSvc::get_pmtcat_ce(int pmtcat, double theta) const 
{
  
  assert( pmtcat >= (int)kPMT_Unknown && pmtcat <= (int)kPMT_NNVT_HighQE && pmtcat + 1 >= 0 );
  double ce = -1;
  G4MaterialPropertyVector * vec = 0 ;
  switch(pmtcat)
    {   //FIXME:KPMT_Unknown represent WP pmt,which are use normal NNVTMCP ?
        case kPMT_Unknown:     { vec = m_CE_WP_PMT ; } ; break ;   
        case kPMT_NNVT:        { vec = m_CE_NNVTMCP ; } ; break ; 
        case kPMT_Hamamatsu:   { vec = m_CE_R12860 ; } ; break ;  
        case kPMT_HZC:         { ce = 1.0 ; } ; return ce ; 
        case kPMT_NNVT_HighQE: { vec = m_CE_NNVTMCP_HiQE ; } ; break ;  
    }
  
  if(vec){ 
        //vec -> SetSpline(false);  that is default 
        ce = vec->Value(theta);
  }
    
  //double CE = ce >= 1 ? 1 : ce ;  
  assert(ce > 0 && ce <= 1 );
  return ce;
}

double PMTSimParamSvc::get_pmtcat_prop(int pmtcat, const std::string& prop_name, double val) const
{
    assert( pmtcat >= (int)kPMT_Unknown && pmtcat <= (int)kPMT_NNVT_HighQE && pmtcat + 1 >= 0 );

    auto iter1 = m_PMT_MPT.find(pmtcat);
    assert(iter1 != m_PMT_MPT.end());
    auto iter2 = iter1->second.find(prop_name);
    assert(iter2 != iter1->second.end());

    return iter2->second->Value(val);
}

double PMTSimParamSvc::get_pmtcat_const_prop(int pmtcat, const std::string& prop_name) const
{
    bool check = ( pmtcat >= (int)kPMT_Unknown && pmtcat <= (int)kPMT_NNVT_HighQE && pmtcat + 1 >= 0 );
    if (!check) {
        std::cerr << "Failed to find the PMTCAT Const Prop with "
                 << " pmtcat: " << pmtcat
                 << " prop_name: " << prop_name
                 << std::endl;
        return 0.0;
    }

    auto iter1 = m_PMT_CONST.find(pmtcat);
    if (iter1 != m_PMT_CONST.end()) {
        auto iter2 = iter1->second.find(prop_name);
        if (iter2 != iter1->second.end()) {
            return iter2->second;
        }
    }

    std::cerr << "Failed to find the PMTCAT Const Prop with "
             << " pmtcat: " << pmtcat
             << " prop_name: " << prop_name
             << std::endl;

    return 0.0;
}

