#ifndef IPMTSimParamSvc_h
#define IPMTSimParamSvc_h

// Description:  query PMT parameters for simulation in this service.
//               Both DetSim and ElecSim will use this service.
//               The parameters could be loaded from plain text or database.
//          
//               It includes:
//               PMT index, SN, Type, PDE, DCR, gain, resolution, TTS, timeoffset,
//               prePulseProb, afterPulseProb...
//
//               Author: miaoyu@ihep.ac.cn 2020.09




#include "SniperKernel/SvcBase.h"
#include "G4MaterialPropertyVector.hh"
class TGraph;

struct PMTSimParamData ; 

class IPMTSimParamSvc {

  public:
    virtual char* get_SN(int pmtId) const = 0;
    virtual double get_gain(int pmtId) const = 0;
    virtual double get_sigmaGain(int pmtId) const = 0;
    virtual double get_pde(int pmtId) const = 0;
    virtual double get_dcr(int pmtId) const = 0;
    virtual double get_afterPulseProb(int pmtId) const = 0;
    virtual double get_prePulseProb(int pmtId) const = 0;
    virtual double get_tts(int pmtId) const  = 0;
    virtual double get_timeOffset(int pmtId) const = 0;

    // new interface for SPMT --qianz 2021.06.10
  public: 
    virtual double get_efficiency(int pmtId) const = 0;  // SPMT
    virtual double get_QE(int pmtId) const = 0;          // SPMT
    
    
  public: 
    virtual TGraph* get_MCPTT() = 0;
    virtual TGraph* get_MCPAmp() = 0;
   // virtual G4DataInterpolation* get_MCPTT() = 0;
   // virtual G4DataInterpolation* get_MCPAmp() = 0;

    // after pulse
    virtual int dynodeAP1_amp() = 0;
    virtual double dynodeAP1_time() = 0;
    virtual int dynodeAP2_amp() = 0;
    virtual double dynodeAP2_time() = 0;
    virtual int mcpAP1_amp() = 0;
    virtual double mcpAP1_time() = 0;
    virtual int mcpAP2_amp() = 0;
    virtual double mcpAP2_time() = 0;
    virtual int mcpAP3_amp() = 0;
    virtual double mcpAP3_time() = 0;
    //
     virtual double get_pmtid_ce(int pmtid,double theta) const = 0;
     virtual double get_pmtcat_ce(int pmtcat,double theta) const = 0;
     virtual double get_pmtid_qe(int pmtid, double energy) const = 0;
     virtual double get_pmtcat_qe(int pmtcat, double energy) const = 0;
     virtual double get_pmt_qe_scale(int pmtid) const = 0; 
     virtual double get_shape_qe_at420nm(int pmtid) const = 0;
     virtual double get_real_qe_at420nm(int pmtid) const = 0;
     //
     virtual double get_pmtcat_prop(int pmtcat, const std::string& prop_name, double val) const = 0;
     virtual double get_pmtcat_const_prop(int pmtcat, const std::string& prop_name) const = 0;
     virtual G4MaterialPropertyVector*  get_pmtcat_qe_vs_energy(int pmtcat) const = 0;

     virtual int get_pmtContiguousIndex(int pmtID) const = 0 ;
     virtual int get_pmtId_from_ContiguousIndex(int idx) const = 0 ;
     virtual void getQEData(std::vector<double>& qe_data, double en0, double en1, unsigned num_edep  ) const = 0 ;
     virtual void getCEData(std::vector<double>& ce_data, double theta0, double theta1, unsigned num_theta) const = 0 ;
     virtual const std::vector<int>&  get_all_pmtID() const = 0; 
     virtual PMTSimParamData* getPMTSimParamData() = 0 ; 

};

#endif
