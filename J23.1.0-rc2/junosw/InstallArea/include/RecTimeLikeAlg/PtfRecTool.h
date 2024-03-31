#ifndef PtfRecTool_h
#define PtfRecTool_h
#include "SniperKernel/ToolBase.h"
#include "RecTools/PMTTable.h"
#include "RecTools/IRecTool.h"
#include <TH1.h>
#include <TMath.h>
#include <TVector3.h>

class PtfRecTool : public ToolBase, public IRecTool{
	public:
		PtfRecTool(const std::string& name);
		virtual ~PtfRecTool();

		bool configure();
		void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);

	private:
		TVector3 calDeltaR_Combine(double ,double ,double );
		double CalculateTOF(double , double , double , int);
		double FOS_correction(double , int );

        std::vector<double> v_res_time;
        TH1D m_hit_time;
		HitPMTVector* m_hitPmtVector;
		bool m_isElec;
		float LS_R;
};
#endif /* ifndef PtfRecTool_h */
