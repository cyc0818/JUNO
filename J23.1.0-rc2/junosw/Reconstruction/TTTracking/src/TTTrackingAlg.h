/*
TotTracker Tracking


Author: A.Meregaglia, C.Jollet (IPHC)
*/


#ifndef TTTrackingAlg_h
#define TTTrackingAlg_h

#include "TtMake3D.h"

#include "SniperKernel/AlgBase.h"
#include "TTree.h"
#include <string>

class NavBuffer;

class TTTrackingAlg: public AlgBase
{
    public:
	TTTrackingAlg(const std::string& name);
	~TTTrackingAlg();

	bool initialize();
	bool execute();
	bool finalize();

    private:

	// variable
	float m_time_thresh;
	bool m_do_xtalk_filter;
	// geometry
	double m_wp_height;
	double m_tt_height;
	double m_z_global_shift;

	TtMake3D * m_tt_make3d;

	// Sniper
	JM::NavBuffer* m_buf;
	int m_evt_id;

	// TREE
	TTree* m_evt_treeTTReco;
	int m_eventID;
	int m_NTracks;
	int m_NTotPoints;
	int m_NPoints[100];
	float m_PointX[200];
	float m_PointY[200];
	float m_PointZ[200];
	double m_Coeff0[20];
	double m_Coeff1[20];
	double m_Coeff2[20];
	double m_Coeff3[20];
	double m_Coeff4[20];
	double m_Coeff5[20];
	double m_Chi2[20];

};
#endif // TTTrackingAlg.h
