#ifndef QcRecTool_h
#define QcRecTool_h
#include "SniperKernel/ToolBase.h"
#include "RecTools/PMTTable.h"
#include "RecTools/IRecTool.h"
#include <TH1.h>
#include <TMath.h>
#include <TVector3.h>

class QcRecTool : public ToolBase, public IRecTool{
	public:
		QcRecTool(const std::string& name);
		virtual ~QcRecTool();

		bool configure();
		void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);

	private:

		bool m_isElec;
		float LS_R;
};
#endif /* ifndef QcRecTool_h */
