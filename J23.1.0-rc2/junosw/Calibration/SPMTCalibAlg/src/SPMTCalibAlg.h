/*
SPMT Calib


Author: A.Meregaglia, C.Jollet (CENBG)
*/


#ifndef SPMTCalibAlg_h
#define SPMTCalibAlg_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Geometry/RecGeomSvc.h"
#include <string>
#include <algorithm>
#include "TTree.h"
#include "SpmtElecConfigSvc/SpmtElecConfigSvc.h"


//class NavBuffer;

class RecGeomSvc;
class CdGeom;

namespace JM {
class CalibHeader;
class CalibPMTChannel;
}

class SPMTCalibAlg: public AlgBase
{
    public:
	SPMTCalibAlg(const std::string& name);
	~SPMTCalibAlg();

	bool initialize();
	bool execute();
	bool finalize();

    private:

	// Sniper
	JM::NavBuffer* m_buf;
	SpmtElecConfigSvc* m_SpmtConfigSvc;
	
	int PC2ADC_THRES;      
	double PC2ADC_HG;     
	double PC2ADC_LG;     
	double Const_HG;      
	double Const_LG;
	double Q_Spe; 
	int StartIdSpmt;
	int TotalSpmt;
	unsigned int channel_index;
	unsigned int channel_mask;  
	unsigned int abc_index;  
	unsigned int abc_mask;  
	

	CdGeom* m_cdGeom;
	
	int EvtID;
	// user data definitions
	std::vector<float> m_charge;
	std::vector<float> m_time;
	int m_EvtId;
	int m_pmtId;
	float m_totalpe;
	TTree* m_calib;
};
#endif // SPMTCalibAlg.h
