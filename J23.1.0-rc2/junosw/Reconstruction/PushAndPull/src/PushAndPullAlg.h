/*
Vertex Reconstruction: Push and Pull(learn from KamLand)
Ennergy Reconstruction: 
1.Calculate the total expect charge by a simple model
2.e_rec = experiment_total/expect_total
expect_total: the total expect charge per 1MeV
experiment_total: the total charge of the PMTs
3.correct the residual non-uniformity by calibration

Author: xzh
*/

#ifndef PushAndPullAlg_h
#define PushAndPullAlg_h

#include "SniperKernel/AlgBase.h"
#include "TVector3.h"
#include "TF1.h"
#include <string>

class NavBuffer;
class CdGeom;

class PushAndPullAlg: public AlgBase
{
    public:
	PushAndPullAlg(const std::string& name);
	~PushAndPullAlg();

	bool initialize();
	bool execute();
	bool finalize();

    private:
  // private function
	bool searchVer(double,double,double);
	void chargeCenter();
	void searchInput();
	TVector3 calDeltaR(double x,double y,double z);
	double calTOF(double x,double y,double z,int i);

	void calExpectPe(double,double,double);
	bool searchEne();
  // variable
	// geometry
	std::string m_det_type;
	int m_pmt_num; 		// number of PMT
	double m_pmt_center_r; 	// radius of PMT center in the steel ball	
	double m_pmt_r;  	// radius of PMT
        double m_pmt_front_r;    // the minimum distance between PMT and center of steel ball
	double m_ls_r;
	std::vector<TVector3> m_pmt_pos;  	// position of PMT

	// calibration function
	TF1* m_ene_cor1;
	TF1* m_ene_cor2;
	TF1* m_ene_nonl;
        	
	// input data
	std::vector<double> m_pmt_hit;
        std::vector<double> m_first_time;
	int fired_pmt_pe[4];	

	//
	std::vector<double> m_pmt_expect;
	double m_expect_total;
	double m_experiment_total;

	// output data
	double x_rec;
	double y_rec;
	double z_rec;
	double r_rec;
	double x_ccm;
	double y_ccm;
	double z_ccm;
	double r_ccm;
	double x_edep;
	double y_edep;
	double z_edep;
	double r_edep;
	

	double ene_rec;
	double ene_gamma;
	
	// parameters 
	double m_eff_att_length;      // effective attenuation length 
	double m_ccm_ratio;
	double low;
	double up;
	double SPEED_LS;
	double SPEED_BUFFER;
	// Sniper
	JM::NavBuffer* m_buf;
	CdGeom*  m_cdGeom;	
	int m_evt_id;

	
};
#endif // PushAndPullAlg.h
