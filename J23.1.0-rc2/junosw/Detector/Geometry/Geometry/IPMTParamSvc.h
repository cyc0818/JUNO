#ifndef IPMTParamSvc_h
#define IPMTParamSvc_h

//
//  Description: allow user query PMT paramters from this service.
//               The parameters could be loaded from plain text or database.
//
//               It includes:
//               * PMT position. Note it's better to use RecGeomSvc, 
//                 because RecGeomSvc could load geometry from GDML/ROOT file.
//               * PMT type. 3inch or 20inch. Hamamatsu or NNVT or HZC
//               * PMT is whether enabled or disabled. 
//                 * If disabled, the geometry is still constructed, but no response.
//               * PMT is whether constructed or not.
//                 * This is useful for the detector simulation
//
//               During query, user could use Identifier or PMTID used in detsim
//
//
//       Author: Tao Lin <lintao@ihep.ac.cn> 2017-05-27
//


#include "SniperKernel/SvcBase.h"
#include <map>

// SCB: enumerations repositioned into below no-dependency headers for global access
#include "Geometry/PMT.h"
#include "Geometry/PMTCategory.h"
struct PMTParamData ; 

class IPMTParamSvc{
public:
  // User interface
  virtual bool isCD(int pmtid) const = 0 ;
  virtual bool isWP(int pmtid) const = 0 ;

  virtual bool is20inch(int pmtid) const = 0 ;
  virtual bool is3inch(int pmtid) const = 0 ;
  
  virtual bool isNormalNNVT(int pmtid) const = 0 ;
  virtual bool isHamamatsu(int pmtid) const = 0 ;
  virtual bool isHighQENNVT(int pmtid) const = 0 ;
  virtual bool isHZC(int pmtid) const = 0 ;

public:
  // if not enabled, the geometry is still enabled, but the response disabled.
  virtual bool isEnabled(int pmtid) const = 0;

  // if not constructed, the geometry is also disabled.
  virtual bool isConstructed(int pmtid) const = 0;

public:
  // Updatas: new get functions --qianz 2021.06.03
  virtual float getPMTX(int pmtid) const = 0;      
  virtual float getPMTY(int pmtid) const = 0;        
  virtual float getPMTZ(int pmtid) const = 0;       
  virtual float getPMTTheta(int pmtid) const = 0;   
  virtual float getPMTPhi(int pmtid) const = 0;      
  virtual float getPMTPsi(int pmtid) const = 0;

  virtual float getPMTmeanR_CD_LPMT() const = 0;
  virtual float getPMTmeanR_CD_SPMT() const = 0;

  virtual float getPMTX_ideal(int pmtid) const = 0;      
  virtual float getPMTY_ideal(int pmtid) const = 0;        
  virtual float getPMTZ_ideal(int pmtid) const = 0;       
  virtual float getPMTTheta_ideal(int pmtid) const = 0;   
  virtual float getPMTPhi_ideal(int pmtid) const = 0;      
  virtual float getPMTPsi_ideal(int pmtid) const = 0;

  virtual float getPMTmeanR_CD_LPMT_ideal() const = 0;
  virtual float getPMTmeanR_CD_SPMT_ideal() const = 0;


  virtual int get_NTotal_CD_LPMT() const = 0;
  virtual int get_NTotal_CD_SPMT() const = 0;
  virtual int get_NTotal_WP_LPMT() const = 0;


public:
  // virtual IPMTParamSvc* Instance() = 0 ; 
  // virtual const char*  PMTCategoryName(int pmtcat) = 0 ;  
  // virtual std::string  PMTCategoryList() = 0;  
  virtual int getPMTCategory(int pmtID) const = 0; 
  virtual const std::map<int, int>& getPMTCategoryMap() const = 0 ;   

  virtual PMTParamData* getPMTParamData() = 0 ; 

};


#endif
