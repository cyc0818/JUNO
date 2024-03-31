//Written by Iwan Morton-Blake
//contact iblake@tcd.ie

#include "imbQCTool.h"
#include "SniperKernel/ToolFactory.h"
#include "Event/CalibPmtChannel.h"
#include "TFile.h"
#include "TTree.h"
#include <boost/filesystem.hpp>

DECLARE_TOOL(imbQCTool);

imbQCTool::imbQCTool(const std::string &name): ToolBase(name){
	declProp("UsePerfectVtx", b_usePerfectVtx=false);
}

imbQCTool::~imbQCTool(){
}

bool imbQCTool::configure(){

	weight_min_t = 20.;
	weight_exp_coeff = 0.015;
	weight_dynode_v_mcp = 3.;

	pmt_hittimes_for_t0.reserve(20000);
  
	// open root files to fill large tables with PMT Centre -> position slope values
	std::string table_filename;
	if (getenv("JUNOTOP")) {
		std::string s = getenv("JUNOTOP");
        s += "/data/Reconstruction/QCtrRecAlg/imbQCToolTables/ERSlopeTables/SlopeTables.root";
        if (boost::filesystem::exists(s)){
        	table_filename = s;
        }
    }
    if (getenv("WORKTOP")) {
    	std::string s = getenv("WORKTOP");
    	s += "/data/Reconstruction/QCtrRecAlg/imbQCToolTables/ERSlopeTables/SlopeTables.root";
    	if (boost::filesystem::exists(s)){
        	table_filename = s;
    	}
	}

	double rval, qval, xslope, yslope, zslope;
	TFile * f_in = TFile::Open(table_filename.c_str());
	if(!f_in) LogInfo<< table_filename  + "is not found!" << std::endl;
	TTree * tree_table_vals = (TTree*)f_in->Get("nt_table_vals");
	TTree * tree_slope_vals = (TTree*)f_in->Get("nt_slope_vals");

	tree_table_vals->SetBranchAddress("r_table_vals",&rval);
	tree_table_vals->SetBranchAddress("q_table_vals",&qval);
	
	tree_slope_vals->SetBranchAddress("x_slopes",&xslope);
	tree_slope_vals->SetBranchAddress("y_slopes",&yslope);
	tree_slope_vals->SetBranchAddress("z_slopes",&zslope);
	
	int entry_ij = 0;

	slope_xyz_radius_size = tree_table_vals->GetEntries() + 1;
	slope_xyz_charge_size = slope_xyz_radius_size;

	for (size_t i = 0 ; i < slope_xyz_radius_size - 1 ; i++){
		tree_table_vals->GetEntry(i);

		slope_xyz_radius_bin_edges[i] = rval;
		slope_xyz_charge_bin_edges[i] = qval;

		for (size_t j = 0 ; j < slope_xyz_charge_size - 1 ; j++){
			tree_slope_vals->GetEntry(entry_ij);

			slope_x_bin_contents [i][j] = xslope;
			slope_y_bin_contents [i][j] = yslope;
			slope_z_bin_contents [i][j] = zslope;

			entry_ij += 1;
		}
	}

	tree_table_vals->Delete();
	tree_slope_vals->Delete();

	f_in->Close();
    delete f_in;

	return true;
}

double imbQCTool::pmtcentre_r_to_rpos_fit_edep(double pmtcentre_r, double charge){

	double rpos = 0.;

	for (int i = 0 ; i < n_pars_rpmt_to_rpos_vs_energy ; i++){
		double par_p = 0.;
		for (int j = 0 ; j < n_pars_rpmt_to_rpos_vs_energy ; j++){
			par_p += rpmt_to_rpos_params[i][j]*pow(charge,j);
		}
		rpos += par_p*pow(pmtcentre_r,i);
	}
				
	return rpos;
}

void imbQCTool::find_2d_bins(double radius_xval, double charge_yval, int &binx_slopexyz, int &biny_slopexyz) {
  

	auto it_x = std::lower_bound(slope_xyz_radius_bin_edges, slope_xyz_radius_bin_edges + slope_xyz_radius_size, radius_xval);
	if (it_x == slope_xyz_radius_bin_edges + slope_xyz_radius_size) {
		binx_slopexyz = slope_xyz_radius_size - 1;
	} else if (it_x == slope_xyz_radius_bin_edges) {
	} else {
		// target is between two values
		auto left_x = it_x - 1;
		auto right_x = it_x;
		if (std::fabs(*left_x - radius_xval) < std::fabs(*right_x - radius_xval)) {
			binx_slopexyz = left_x - slope_xyz_radius_bin_edges;
		} else {
			binx_slopexyz = right_x - slope_xyz_radius_bin_edges;
		}
	}

	auto it_y = std::lower_bound(slope_xyz_charge_bin_edges, slope_xyz_charge_bin_edges + slope_xyz_charge_size, charge_yval);
	if (it_y == slope_xyz_charge_bin_edges + slope_xyz_charge_size) {
		biny_slopexyz = slope_xyz_charge_size - 1;
  	} else if (it_y == slope_xyz_charge_bin_edges) {
	} else {
		// target is between two values
		auto left_y = it_y - 1;
		auto right_y = it_y;
		if (std::fabs(*left_y - charge_yval) < std::fabs(*right_y - charge_yval)) {
			biny_slopexyz = left_y - slope_xyz_charge_bin_edges;
		} else {
			biny_slopexyz = right_y - slope_xyz_charge_bin_edges;
		}
	}

}

double imbQCTool::correct_z_bias_rhofrac(double rhofrac){

	double correcfrac = -0.0150442 + (0.0411421)*rhofrac + (-0.0207867)*rhofrac*rhofrac;
	return correcfrac;

}

void imbQCTool::rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo){

	/////////
	// hit time loop #1
	// Calculate median t0 time , needed to remove hits earlier than some minimum time (weight_min_t)
	pmt_hittimes_for_t0.clear();

	for (auto* thisPMT: hitpmtvector){
		const auto& thishittime = thisPMT->pmtcalib->time();
	for (size_t i = 0 ; i < thishittime.size() ; i++)
		pmt_hittimes_for_t0.push_back(thishittime[i]);
	}
	// sort hit times to calculate a median, representing t0
	std::sort(pmt_hittimes_for_t0.begin(), pmt_hittimes_for_t0.end());
	double median_trigger_time = 300.;
	if (pmt_hittimes_for_t0.size() < n_times_for_median){
		LogDebug<<"imbQCTool::Number of pmt hits < number of hits needed for t0 definition"<< std::endl;
		median_trigger_time = pmt_hittimes_for_t0[pmt_hittimes_for_t0.size()-1];
	}else{
		median_trigger_time = pmt_hittimes_for_t0[int(n_times_for_median/2)-1];
	}

	double pmt_weighted_xpos = 0.;
	double pmt_weighted_ypos = 0.;
	double pmt_weighted_zpos = 0.;
	double pmt_total_weight = 0.;

	// hit time loop #2
	// apply weightings, calculate PMT centre
	double ev_total_charge = 0.; // estimator for energy, needed for radius estimate and final conversion of PMT centre to event position.
	for (auto* thisPMT: hitpmtvector){
		const auto& thishittime = thisPMT->pmtcalib->time();
		const auto& thishitcharge = thisPMT->pmtcalib->charge();

		for (size_t i = 0 ; i < thishittime.size() ; i++){

			double weight = 1.;
			///  hamamatsu vs mcp /////
			if (thisPMT->type == PMTTYPE::LPMT_DYNODE){
				weight *= weight_dynode_v_mcp;
			}else if (thisPMT->type == PMTTYPE::LPMT_MCP || \
				thisPMT->type == PMTTYPE::LPMT_MCP_HIGHQE){
			}else
				LogDebug<<" PMT type is neither Dynode nor MCP!" << std::endl;

			ev_total_charge += thishitcharge[i];

			// ignore hit times that are too early, defined by t0
			if (thishittime[i] < (median_trigger_time-weight_min_t)) continue;

			weight *= thishitcharge[i]*TMath::Exp(-1.*weight_exp_coeff*thishittime[i]); // exponential time weight, charge

			pmt_weighted_xpos += weight*thisPMT->pos.X()*pmt_radius_correction;
			pmt_weighted_ypos += weight*thisPMT->pos.Y()*pmt_radius_correction;
			pmt_weighted_zpos += weight*thisPMT->pos.Z()*pmt_radius_correction;
			pmt_total_weight += weight;

		} // looping hits per pmt
	
	}// looping all hit pmts

	// normalise by weights
	pmt_weighted_xpos = pmt_weighted_xpos/pmt_total_weight;
	pmt_weighted_ypos = pmt_weighted_ypos/pmt_total_weight;
	pmt_weighted_zpos = pmt_weighted_zpos/pmt_total_weight;
	Double_t pmt_weighted_centre = sqrt(pmt_weighted_xpos*pmt_weighted_xpos +\
	pmt_weighted_ypos*pmt_weighted_ypos + pmt_weighted_zpos*pmt_weighted_zpos);


	// first estimate for event radius:
	double rpmtcentre_to_rpos = pmtcentre_r_to_rpos_fit_edep(pow(pmt_weighted_centre/AV_radius,3),ev_total_charge);

	// look up table with estimate radius and event charge
	int binx_slopexyz, biny_slopexyz = 0;
	find_2d_bins(rpmtcentre_to_rpos,ev_total_charge,binx_slopexyz,biny_slopexyz);

	double xpos_recon_func = pmt_weighted_xpos*slope_x_bin_contents[binx_slopexyz][biny_slopexyz];
	double ypos_recon_func = pmt_weighted_ypos*slope_y_bin_contents[binx_slopexyz][biny_slopexyz];
	double zpos_recon_func = pmt_weighted_zpos*slope_z_bin_contents[binx_slopexyz][biny_slopexyz];

	double rpos_recon_func = sqrt(xpos_recon_func*xpos_recon_func + \
	ypos_recon_func*ypos_recon_func + zpos_recon_func*zpos_recon_func);

	double rhofrac = sqrt(xpos_recon_func*xpos_recon_func+ypos_recon_func*ypos_recon_func)/rpos_recon_func;
	double zpos_recon_corr_frac = correct_z_bias_rhofrac(rhofrac);
	double zpos_recon_func_corr = zpos_recon_func*(1. - zpos_recon_corr_frac);

	TVector3 qtCtr(xpos_recon_func,ypos_recon_func,zpos_recon_func_corr);

	recinfo.vtx = qtCtr;

	LogDebug<<"X:" <<recinfo.vtx.X()<<"  Y: "<<recinfo.vtx.Y()<<"  Z: "<<recinfo.vtx.Z()<< std::endl;
}
