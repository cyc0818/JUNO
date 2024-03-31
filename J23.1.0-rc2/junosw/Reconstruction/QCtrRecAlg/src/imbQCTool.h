//Written by Iwan Morton-Blake
//contact iblake@tcd.ie

#ifndef IMBQCTOOL_H
#define IMBQCTOOL_H
#include "RecTools/IRecTool.h"
#include "SniperKernel/ToolBase.h"


class imbQCTool: public ToolBase, public IRecTool{
	public:
		imbQCTool(const std::string &);
		virtual ~imbQCTool();

		double pmtcentre_r_to_rpos_fit_edep(double pmtcentre_r, double charge);

		void find_2d_bins(double radius_xval, double charge_yval, int &binx_slopexyz, int &biny_slopexyz);

		double correct_z_bias_rhofrac(double rhofrac);

		bool configure();
		void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &);

		private:
		//options
		bool b_usePerfectVtx;

		// Weighting parameters
		double weight_min_t;
		double weight_exp_coeff;
		double weight_dynode_v_mcp;

		const double AV_radius = 17700;

		long unsigned int n_times_for_median = 300;

		// correction factor due to difference in PMT positions compared to those saved in EDM files and those in PMTVector
		const float pmt_radius_correction = 1.01324; 

		std::vector <double> pmt_hittimes_for_t0;

		// first estimate of event radius, needed for lookup tables, PMTCentre -> event radius, 
		// conversion described by pol2, where each pol2 parameter is a function of total charge, also described using a pol2
		static const int n_pars_rpmt_to_rpos = 3;
		static const int n_pars_rpmt_to_rpos_vs_energy = 3;

		double rpmt_to_rpos_params [n_pars_rpmt_to_rpos] [n_pars_rpmt_to_rpos_vs_energy] 
		{ 
			{-0.0109589, 2.99052e-6, -7.95889e-11},\
		{0.49099, -2.36901e-5, 5.62404e-10},\
		{0.648839, 5.809e-7, 8.16156e-11} 
		};

		// 100 x 100 look-up table (radius vs charge) for slopes, converting from PMTCentre_x,y,z -> event position x,y,z
		size_t slope_xyz_radius_size;
		size_t slope_xyz_charge_size;

		static const int max_arr_size = 500;

		double slope_xyz_radius_bin_edges[max_arr_size];
		double slope_xyz_charge_bin_edges[max_arr_size];

		// 100 x 100 grid for x conversions
		double slope_x_bin_contents [max_arr_size-1][max_arr_size-1];

		double slope_y_bin_contents [max_arr_size-1][max_arr_size-1];

		double slope_z_bin_contents [max_arr_size-1][max_arr_size-1];

};
#endif
