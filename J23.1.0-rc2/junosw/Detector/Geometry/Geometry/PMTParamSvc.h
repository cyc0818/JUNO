#ifndef PMTParamSvc_h
#define PMTParamSvc_h

//
//  Description: allow user query PMT paramters from this service.
//               The parameters could be loaded from plain text or database.
//
//               It includes:
//               * PMT position. Note it's better to use RecGeomSvc, 
//                 because RecGeomSvc could load geometry from GDML/ROOT file.
//               * PMT type. 3inch or 20inch. Hamamatsu or NNVT or HZC
//               * TTS.
//
//               During query, user could use Identifier or PMTID used in detsim
//
//         Note: In ElecSim, there is a service called PmtParamSvc:
//                 Simulation/ElecSimV3/PmtParamSvc
//               We need to consider to unify these two services.
//
//               In the future, the Identifier could be also encoded with more info.
//
//       Author: Tao Lin <lintao@ihep.ac.cn> 2017-05-27
//


#include "SniperKernel/SvcBase.h"
#include "Geometry/IPMTParamSvc.h"
#include <map>
#include "Geometry/PMTParamData.h"
#include "Geometry/PMT.h"


class PMTParamSvc: public IPMTParamSvc, public SvcBase {
public:
  PMTParamSvc(const std::string& name);
  virtual ~PMTParamSvc();

  bool initialize();
  bool finalize();

  // User interface

  bool isCD(int pmtid) const ;
  bool isWP(int pmtid) const ;

  bool is20inch(int pmtid) const ;
  bool is3inch(int pmtid) const ;
  
  bool isNNVT(int pmtid) const ;
  bool isNormalNNVT(int pmtid) const ;
  bool isHamamatsu(int pmtid) const ;
  bool isHighQENNVT(int pmtid) const ;
  bool isHZC(int pmtid) const ;

public:
  // if not enabled, the geometry is still enabled, but the response disabled.
  bool isEnabled(int pmtid) const;

  // if not constructed, the geometry is also disabled.
  bool isConstructed(int pmtid) const;

public:
  // Updatas: new get functions --qianz 2021.06.03
  float getPMTX(int pmtid) const;      
  float getPMTY(int pmtid) const;        
  float getPMTZ(int pmtid) const;       
  float getPMTTheta(int pmtid) const;   
  float getPMTPhi(int pmtid) const;      
  float getPMTPsi(int pmtid) const;

  float getPMTX_ideal(int pmtid) const;
  float getPMTY_ideal(int pmtid) const;
  float getPMTZ_ideal(int pmtid) const;
  float getPMTTheta_ideal(int pmtid) const;
  float getPMTPhi_ideal(int pmtid) const;
  float getPMTPsi_ideal(int pmtid) const;

  float getPMTmeanR_CD_LPMT() const;
  float getPMTmeanR_CD_SPMT() const;

  float getPMTmeanR_CD_LPMT_ideal() const;
  float getPMTmeanR_CD_SPMT_ideal() const;

  int get_NTotal_CD_LPMT() const;
  int get_NTotal_CD_SPMT() const;
  int get_NTotal_WP_LPMT() const;

private:
  bool init_ranges();
  bool init_default();
  bool init_file();

private:
  // Updatas: new files to initialize the service --qianz 2021.06.03
  std::string getFilePath(const std::string &f);
  bool init_file_PMTPos(const std::string &f);
  bool init_file_PMTType(const std::string &f);
  bool init_file_PMTPos_Shift(const std::string &f);
  bool init_PMT_R();

private:
  PMTParamData data ; 

private:
  std::string m_db_type; // * File * ROOT * MySQL ...

private:
  std::map<int, int>& m_pmt_categories; // 

  // new files
  std::string m_file_cd_lpmt_pos_path;
  std::string m_file_cd_lpmt_type_path;
  std::string m_file_cd_spmt_pos_path;
  std::string m_file_cd_spmt_type_path;
  std::string m_file_wp_lpmt_pos_path;
  std::string m_file_wp_lpmt_type_path;
  
  std::string m_file_cd_lpmt_pos_shift_path;
  std::string m_file_cd_spmt_pos_shift_path;
  std::string m_file_wp_lpmt_pos_shift_path;

  // pmt position
  std::map<int, float> m_pmt_x;
  std::map<int, float> m_pmt_y;
  std::map<int, float> m_pmt_z;
  std::map<int, float> m_pmt_theta; // pmt direction rotation along y axis
  std::map<int, float> m_pmt_phi;   // pmt direction rotation along z axis
  std::map<int, float> m_pmt_psi;

  std::vector<float> m_pmt_r_lpmt;
  std::vector<float> m_pmt_r_spmt;
  float m_pmt_meanR_CD_lpmt;
  float m_pmt_meanR_CD_spmt;

  // pmt position original unshifted
  std::map<int, float> m_pmt_x_ideal;
  std::map<int, float> m_pmt_y_ideal;
  std::map<int, float> m_pmt_z_ideal;
  std::map<int, float> m_pmt_theta_ideal; // pmt direction rotation along y axis
  std::map<int, float> m_pmt_phi_ideal;   // pmt direction rotation along z axis
  std::map<int, float> m_pmt_psi_ideal;

  std::vector<float> m_pmt_r_lpmt_ideal;
  std::vector<float> m_pmt_r_spmt_ideal;
  float m_pmt_meanR_CD_lpmt_ideal;
  float m_pmt_meanR_CD_spmt_ideal;

  // nums of pmt
  int m_nums_cd_lpmt;
  int m_nums_cd_spmt;
  int m_nums_wp_lpmt;

  // Control the PMT enabled/constructed
  // The range [start, stop)
  // The ranges: [start1, stop1, start2, stop2, ...]
  //              <--range 1-->  <--range 2-->
  //
  // If not set, then always enabled.

  // helper methods: 
  // * check_ranges: during initialization, check the input correct
  // * is_in_range: check if PMT in the ranges.
  bool helper_check_ranges(const std::vector<int>& ranges) const;
  bool helper_is_in_range(const std::vector<int>& ranges, int pmtid) const;

  // PMT enabled or not
  std::vector<int> m_pmt_enabled_ranges;
  
  // PMT constructed or not
  std::vector<int> m_pmt_constructed_ranges;
private: 
  static PMTParamSvc* fInstance ;  
 
public:
  static PMTParamSvc* Instance(); 
  int           getPMTCategory(int pmtID) const ; 
  const std::map<int, int>& getPMTCategoryMap() const { return m_pmt_categories ; } 

  PMTParamData* getPMTParamData() { return &data ; } ; 
};


#endif
