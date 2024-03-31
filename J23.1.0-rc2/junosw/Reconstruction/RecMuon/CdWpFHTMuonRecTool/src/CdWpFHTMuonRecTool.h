#ifndef CdWpFHTMuonRecTool_h
#define CdWpFHTMuonRecTool_h
#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/ToolBase.h"
#include "Math/Minimizer.h"

using namespace std;

class Chi2TimeSimple;
class IFhtCorrection;
class CdWpFHTMuonRecTool : public IRecMuonTool, public ToolBase
{
	public:
		CdWpFHTMuonRecTool(const std::string& name);
		virtual ~CdWpFHTMuonRecTool();

		bool reconstruct(RecTrks*);
		bool configure(const Params*,const PmtTable*);

	private:
		void iniargs(TVector3& ,double&,TVector3&,double&);
		int pmtCut();
		void corrpmtinfo(TVector3&,TVector3&);

	private:

		int m_evtid;
		double m_LSRadius;
		double m_clight;
		double m_nLSlight;
		double m_vmuon;
		int m_nBinDist;
		double cdfirstHit;
		double wpfirstHit;

		const PmtTable* m_oriptable;
		PmtTable m_ptable;
		std::string m_corrfilename;
		std::string m_corrmethod;

		ROOT::Math::Minimizer* m_minuit;
		Chi2TimeSimple* m_chi2func;
		IFhtCorrection* m_fhtcorrtool;

		double m_relativetimecut;
		double m_qcut;
		double m_pmtUsedNumCut;
};
#endif /* ifndef CdWpFHTMuonRecTool_h */
