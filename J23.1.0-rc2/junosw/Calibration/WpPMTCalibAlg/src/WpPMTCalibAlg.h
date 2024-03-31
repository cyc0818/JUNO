/*
Wp PMT Calib


Author: Haoqi (IHEP)
*/


#ifndef WpPMTCalibAlg_h
#define WpPMTCalibAlg_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Geometry/RecGeomSvc.h"
#include <string>
#include <algorithm>
#include "TTree.h"


//class NavBuffer;

class RecGeomSvc;
class CdGeom;

namespace JM {
class CalibHeader;
class CalibPMTChannel;
}

class WpPMTCalibAlg: public AlgBase
{
    public:
	WpPMTCalibAlg(const std::string& name);
	~WpPMTCalibAlg();

	bool initialize();
	bool execute();
	bool finalize();

    private:

	// Sniper
	JM::NavBuffer* m_buf;
	
	double Q_Spe;
        double Q_factor; 
        int StartIdWppmt;
	int TotalWppmt;
	

	CdGeom* m_cdGeom;
	
	int EvtID;
	// user data definitions
	std::vector<float> m_charge;
	std::vector<float> m_time;
        int m_EvtId;
	int m_pmtId;
        int m_npmt;
	float m_totalpe;
	TTree* m_calib;
};
#endif // WpPMTCalibAlg.h
