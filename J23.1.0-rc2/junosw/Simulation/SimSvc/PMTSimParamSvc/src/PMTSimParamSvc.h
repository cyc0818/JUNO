#ifndef PMTSimParamSvc_h
#define PMTSimParamSvc_h

// Description:  query PMT parameters for simulation in this service.
//               Both DetSim and ElecSim will use this service.
//               The parameters could be loaded from plain text or database.
//
//               It includes:
//               PMT index, SN, Type, PDE, DCR, gain, sigmaGain, TTS, timeoffset,
//               prePulseProb, afterPulseProb...
//
//               Author: miaoyu@ihep.ac.cn 2020.09
//               Modified by: Zhen Qian
//                            Simon Blyth
//                            Tao Lin

#include "SniperKernel/SvcBase.h"
#include "IPMTSimParamSvc/IPMTSimParamSvc.h"
#include "Geometry/IPMTParamSvc.h"
#include "Geometry/PMT.h"
#include <string>
#include <map>
#include "TGraph.h"
#include "TF1.h"
#include <MCParamsSvc/IMCParamsSvc.hh>
//#include "G4MaterialPropertyVector.hh"

#include "PMTSimParamSvc/PMTSimParamData.h"


class TH1D;
class TFile;
class TTree;


class PMTSimParamSvc : public IPMTSimParamSvc, public SvcBase
{

public:
  PMTSimParamSvc(const std::string &name);
  ~PMTSimParamSvc();

public:
  bool initialize();
  bool finalize();

private:
  void init_all_pmtID_qe_scale(); 

public:
  char *get_SN(int pmtId) const ;
  double get_gain(int pmtId) const ;
  double get_sigmaGain(int pmtId) const ;
  double get_pde(int pmtId) const ;
  double get_dcr(int pmtId) const ;
  double get_afterPulseProb(int pmtId) const ;
  double get_prePulseProb(int pmtId) const ;
  double get_tts(int pmtId) const ;
  double get_timeOffset(int pmtId) const ;

  TGraph *get_MCPTT() { return gTT_MCP; }
  TGraph *get_MCPAmp() { return gAmp_MCP; }

  //G4MaterialPropertyVector *get_MCPTT() { return m_TT_MCP; }
  //G4MaterialPropertyVector *get_MCPAmp() { return m_Amp_MCP; }


  // after pulse
  int dynodeAP1_amp();
  double dynodeAP1_time();
  int dynodeAP2_amp();
  double dynodeAP2_time();
  int mcpAP1_amp();
  double mcpAP1_time();
  int mcpAP2_amp();
  double mcpAP2_time();
  int mcpAP3_amp();
  double mcpAP3_time();

  double get_pmtcat_ce(int pmtcat,double theta) const;
  /*this function will return ce value if you give a theta(rad) and pmt catagory*/


  double get_pmtid_ce(int pmtid ,   double theta) const;
  /*this function will return ce value if you give a theta(rad) and pmt id*/
  
  //bool helper_pmt_ce(G4MaterialPropertyVector*& di, IMCParamsSvc* params, const std::string& name );
  bool helper_pmt_mpt(G4MaterialPropertyVector*& vec, IMCParamsSvc* params, const std::string& name); 
  /*the above two function will initialize the ce curve and qe shape curve */


  double get_pmtid_qe(int pmtid, double energy) const ;
  /*this function will return a real qe value for a specific pmt ID and photo energy*/

  double get_pmtcat_qe(int pmtcat, double energy) const ;

  double get_pmt_qe_scale(int pmtid) const ;
  /*Since we have the qe shape curve and the qe real curve, these two curve are connected by a scale factor.
   
   the equation is : scale = qe_real/qe_shape
   Thus if you give a pmt ID , then this function will return the scale factor of corresponding pmt.

  */

  double get_shape_qe_at420nm(int pmtid) const ;
  /*it will return a number which represent the value at 420 nm of qe shape for a given pmt ID*/

  double get_real_qe_at420nm(int pmtid) const ;
  /*it will return a number which represent the value at 420 nm of real qe curve for a given pmt ID */

 
  G4MaterialPropertyVector* get_pmtcat_qe_vs_energy(int pmtcat) const ;

  double get_pmtcat_prop(int pmtcat, const std::string& prop_name, double val) const ;
  double get_pmtcat_const_prop(int pmtcat, const std::string& prop_name) const ;

public:
  const std::vector<int>& get_all_pmtID() const;
  /**
   get_pmtContiguousIndex
  ------------------------
 
   Returns the index of the pmtID within the sorted list of all 
   pmtID providing a strictly contiguous(no gaps) index.  
   If the pmtID argument is not in the list of all pmtIDs 
   then -1 is returned. 
 
   **/
   int get_pmtContiguousIndex(int pmtID) const;

   
   /**
   get_pmtId_from_ContiguousIndex
   -------------------------------
 
    Returns the pmtID corresponding to the index provided.
    If the index is out of range of the list of all pmtID then -1 is returned. 
 
    **/
   int get_pmtId_from_ContiguousIndex(int idx) const;

   void getQEData(std::vector<double>& qe_data, double en0, double en1, unsigned num_edep  ) const ;

   void getCEData(std::vector<double>& ce_data, double theta0, double theta1, unsigned num_theta) const ;







private:
   PMTSimParamData m_data ;  // the underlying data

private:
  std::vector<int>&      m_all_pmtID;
  std::vector<double>&   m_all_pmtID_qe_scale ; 

private:
  bool init_default();
  bool init_PMTParamSvc();

public:
  // file mode initialization
  bool init_file_CD_LPMT();
  bool init_file_CD_SPMT();
  bool init_file_WP_LPMT();

  // following method will be reused by both init_file_CD_LPMT and init_file_WP_LPMT.
  bool init_file_common_LPMT(std::vector<PmtSimData_LPMT>& pmtdata_vector,
                             int N, // user can control the number to be loaded
                             int pmtid_offset, // the offset of the PMT
                             const std::string& custom_path, // user customized
                             const std::string& default_path);      // file mode initialization

private:
  std::string m_db_path_SPMT;
  int& m_PmtTotal_SPMT;
  std::map<int, PmtSimData_SPMT>& pd_map_SPMT;


public:
  // SPMT
  double get_efficiency(int pmtId) const ;
  double get_QE(int pmtId) const ;

private:
  std::map<int, int>& m_map_pmt_category;
  std::map<int, int>& m_all_pmt_category ;  
  std::vector<int>& m_all_pmt_catvec ;  

private:
  std::string m_db_type;
  std::string m_db_path_CD_LPMT; // path to load CD-LPMT
  std::string m_db_path_WP_LPMT; // path to load WP-LPMT
  std::string m_insertVersion;

private:

  int& m_PmtTotal ;   // _LPMT
  std::vector<PmtSimData_LPMT>& pd_vector;

  int& m_PmtTotal_ALL ;  
  int& m_PmtTotal_WP ;  

private:
  double m_pde_scale_Hamamatsu;
  double m_pde_scale_NNVT;

private:
  TGraph *gTT_MCP;
  TGraph *gAmp_MCP;
  // previous mode used in codes, not default
// real ce curve of pmt.
private:
 // G4MaterialPropertyVector* m_TT_MCP;
 // G4MaterialPropertyVector* m_Amp_MCP;
  G4MaterialPropertyVector* m_CE_Dynode;
  G4MaterialPropertyVector* m_CE_MCP;
  G4MaterialPropertyVector* m_CE_R12860;
  G4MaterialPropertyVector* m_CE_NNVTMCP;//means normal nnvt
  G4MaterialPropertyVector* m_CE_NNVTMCP_HiQE;
  G4MaterialPropertyVector* m_CE_WP_PMT;

//the shape of qe curve of PMT
private:  
  G4MaterialPropertyVector*& m_QEshape_HZC;
  G4MaterialPropertyVector*& m_QEshape_NNVT;//means normal
  G4MaterialPropertyVector*& m_QEshape_NNVT_HiQE;
  G4MaterialPropertyVector*& m_QEshape_R12860;
  G4MaterialPropertyVector*& m_QEshape_WP_PMT;

  std::map<int, std::map<std::string, G4MaterialPropertyVector*>>& m_PMT_MPT;
  std::map<int, std::map<std::string, G4double>>& m_PMT_CONST;


  double dynodeAP1_prob;
  double dynodeAP1_mu;
  double dynodeAP1_sigma;
  double dynodeAP2_prob;
  double dynodeAP2_mu;
  double dynodeAP2_sigma;
  double MCPAP1_prob;
  double MCPAP1_mu;
  double MCPAP1_sigma;
  double MCPAP2_prob;
  double MCPAP2_mu;
  double MCPAP2_sigma;
  double MCPAP3_prob;
  double MCPAP3_mu;
  double MCPAP3_sigma;

private:
  double m_ce_flat_value;
  double Ham20inch_m_ce_flat_value = 0.95;
  double Ham20inch_m_EAR_value = 0.93;
  double MCP20inch_m_ce_flat_value = 0.85;
  double MCP20inch_m_EAR_value = 1.;
  double Ham8inch_m_ce_flat_value = 0.7;
  double HZC9inch_m_ce_flat_value = 0.67;
  double MCP8inch_m_ce_flat_value = 0.85;
  double MCP8inch_m_EAR_value = 1.;
  double Ham8inch_m_EAR_value = 0.88;
  double HZC9inch_m_EAR_value = 0.92;

  TF1 *m_ce_func;
private:
  double m_eff_ce_Hamamatsu;
  double m_eff_ce_NNVT_Normal;
  double m_eff_ce_NNVT_HiQE;
  //
  double m_qeshape_at420nm_Hamamatsu;
  double m_qeshape_at420nm_NNVT_Normal;
  double m_qeshape_at420nm_NNVT_HiQE;
  double m_qereal_at420nm_WP;
  double m_qeshape_at420nm_WP;
  double m_qeshape_at420nm_HZC;//3inch
  



  private:
    IPMTParamSvc* m_PMTParamSvc;

  public:
    PMTSimParamData* getPMTSimParamData() { return &m_data ; } ; 
    std::string descTotal() const  ; 
    
};

#endif
