#include "SniperKernel/ToolFactory.h"
#include "RecTimeLikeAlg/PtfRecTool.h"

DECLARE_TOOL(PtfRecTool);

PtfRecTool::PtfRecTool(const std::string& name):ToolBase(name),
                       m_hit_time("","tmp_hit_time",5000,-20,2000)
{
	declProp("Pdf_Value", m_isElec=0);
	declProp("LS_R", LS_R=17.7);

}

PtfRecTool::~PtfRecTool(){
}

bool PtfRecTool::configure(){
	return true;
}

void PtfRecTool::rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo)
{
	m_hitPmtVector = &hitpmtvector;
    v_res_time.reserve(pmttable.size());

	double tmp_pos_x = recinfo.vtx.X();
	double tmp_pos_y = recinfo.vtx.Y();
	double tmp_pos_z = recinfo.vtx.Z();

	TVector3 delta_r;

	bool flag = false;
	const int maxloop = 100;
	double limit_delta = 1;

	for (int i=0;i<maxloop;++i){
		double r = TMath::Sqrt(tmp_pos_x*tmp_pos_x+tmp_pos_y*tmp_pos_y+tmp_pos_z*tmp_pos_z);

		if (r>LS_R*1000){
			LogDebug << "invalid vertex. " << std::endl;
			if (!flag)
			{
				i = 0;
				flag = true;
				tmp_pos_x *= (LS_R*1000-10)/r ;
				tmp_pos_y *= (LS_R*1000-10)/r ;
				tmp_pos_z *= (LS_R*1000-10)/r ;
			}
			else
			{
				tmp_pos_x *= (LS_R*1000)/r ;
				tmp_pos_y *= (LS_R*1000)/r ;
				tmp_pos_z *= (LS_R*1000)/r ;
				break;
			}
		}

		delta_r = calDeltaR_Combine(tmp_pos_x,tmp_pos_y,tmp_pos_z);

		tmp_pos_x += delta_r.X();
		tmp_pos_y += delta_r.Y();
		tmp_pos_z += delta_r.Z();
		if (delta_r.Mag() < limit_delta){
			LogDebug << "iteration " << i << std::endl;
			break;
		}
		if (i == maxloop - 1){
			LogDebug << "iteration " << i << std::endl;
		}
	}
	LogDebug << "delta_r = " << delta_r.Mag() << std::endl;

	double PTFRec_x = tmp_pos_x;
	double PTFRec_y = tmp_pos_y;
	double PTFRec_z = tmp_pos_z;

	TVector3 m_vtx_ptf(PTFRec_x, PTFRec_y, PTFRec_z);
	//LogInfo << "PTFRec_x = " << PTFRec_x << ", PTFRec_y = " << PTFRec_y << ", PTFRec_z = " << PTFRec_z << std::endl;

	recinfo.vtx = m_vtx_ptf;
}

TVector3 PtfRecTool::calDeltaR_Combine(double x,double y,double z)
{
    m_hit_time.Reset("ICE");
	//TH1D* m_hit_time = new TH1D("","tmp_hit_time",5000,-20,2000);
	const int n_pe = 50;
	unsigned int nFiredPMTs = m_hitPmtVector->size();
	auto& hitPmtVector = *m_hitPmtVector;
	// find the peak time of t_res
	v_res_time.clear();
	for (int i=0; i<nFiredPMTs; ++i){
		v_res_time.push_back(1e9);

		auto* thisPMT = hitPmtVector[i];

		int pe_num = int(thisPMT->totq+0.5);
		if (pe_num < 1 || pe_num > n_pe) continue;
		//if (thisPMT->isdn) continue;

		double timeflight = CalculateTOF(x,y,z,i);
		double tres = thisPMT->fht-timeflight;

		//Apply FOS time correction
		tres = tres - FOS_correction(pe_num, i);

		m_hit_time.Fill(tres);

		v_res_time.at(i) = tres;
	}
	int max_bin_id_pe;
	double peak_time_pe;

	max_bin_id_pe = m_hit_time.GetMaximumBin();
	peak_time_pe = m_hit_time.GetBinCenter(max_bin_id_pe);

	double low = 10;
	double up = 5;
	// use t_res in [t_peak-low, t_peak+up]
	double mean_res_time = 0;
	double fired_pmt = 0;

    for (int i=0; i<nFiredPMTs; ++i){
		int pe_num = int(hitPmtVector[i]->totq+0.5);
		if (pe_num < 1 || pe_num > n_pe) continue;

		if (v_res_time[i]<peak_time_pe-low || v_res_time[i]>peak_time_pe+up) continue;
		mean_res_time += v_res_time[i];
		fired_pmt ++;
	}
	mean_res_time /= fired_pmt;

	double delta_x = 0.0;
	double delta_y = 0.0;
	double delta_z = 0.0;

	for (int i=0; i<nFiredPMTs; ++i){
		auto* thisPMT = hitPmtVector[i];
		int pe_num = int(thisPMT->totq+0.5);
		if (pe_num < 1 || pe_num > n_pe) continue;

		double timeflight = thisPMT->fht - v_res_time[i];

		if (v_res_time[i]<peak_time_pe-low || v_res_time[i]>peak_time_pe+up) continue;

		auto& pos = thisPMT->pos;
		auto denom = timeflight*fired_pmt;
		auto time = v_res_time[i];
		delta_x += (time-mean_res_time)*(x-pos.X())/denom;
		delta_y += (time-mean_res_time)*(y-pos.Y())/denom;
		delta_z += (time-mean_res_time)*(z-pos.Z())/denom;
	}

	return TVector3(delta_x, delta_y, delta_z);
}

double PtfRecTool::CalculateTOF(double source_x, double source_y, double source_z, int i)
{
	double pmt_pos_x, pmt_pos_y, pmt_pos_z;
	auto& pos = (*m_hitPmtVector)[i]->pos;

	double dx = (source_x - pos.X())/1000;
	double dy = (source_y - pos.Y())/1000;
	double dz = (source_z - pos.Z())/1000;

	double dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);

	static const double lightspeed = 299792458/1e9;
	//optimized for PTF algorithm base on ToyMC
	double m_neff = 1.538;
	if ( m_isElec ){
		m_neff = 1.542;
	}

	return dist*m_neff/lightspeed;//currently
}


//Apply first-order-stastic time correction
double PtfRecTool::FOS_correction(double npe, int i)
{
	static const double par_ideal[]  = {6.07993, 0.467088, -2.40442};
	static const double par_ham[]  = {9.61728, -0.311931, -4.82104};
	static const double par_mcp[]  = {29.367, -7.6345, -14.495};
	const double *par;
	if ( !m_isElec ){
		par = par_ideal;
	}
	else {
		if ((*m_hitPmtVector)[i]->type == LPMT_DYNODE){
			par = par_ham;
		}
		else {
			par = par_mcp;
		}
	}

	double shift = par[0]/TMath::Sqrt(npe)+par[1]+par[2]/npe;

	return shift;
}

