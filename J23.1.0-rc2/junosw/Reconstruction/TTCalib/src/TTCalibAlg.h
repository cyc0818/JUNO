/*
TopTracker Calib


Author: A.Meregaglia, C.Jollet (IPHC)
Changes to CalibEvent v2 => J. P. A. M. de André <jpandre@iphc.cnrs.fr>
*/


#ifndef TTCalibAlg_h
#define TTCalibAlg_h

#include "SniperKernel/AlgBase.h"
#include <string>

//#include "DetSimAlg/IDetElement.h"

class NavBuffer;

class TTCalibAlg: public AlgBase
{
    public:
	TTCalibAlg(const std::string& name);
	~TTCalibAlg();

	bool initialize();
	bool execute();
	bool finalize();

    private:

	// Sniper
	JM::NavBuffer* m_buf;
	int m_evt_id;
	//IDetElement *de;
	double meangain;
};
#endif // TTCalibAlg.h
