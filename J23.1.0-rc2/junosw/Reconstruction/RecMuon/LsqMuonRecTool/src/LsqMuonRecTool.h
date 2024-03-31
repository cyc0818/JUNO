#ifndef LsqMuonRecTool_h
#define LsqMuonRecTool_h
#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/ToolBase.h"
#include "Math/Minimizer.h"

#include "TH1F.h"
#include <memory>

class Chi2TimeSimple;
class IFhtCorrection;
class LsqMuonRecTool : public IRecMuonTool, public ToolBase
{
  public:
    LsqMuonRecTool(const std::string& name);
    virtual ~LsqMuonRecTool();

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

    std::unique_ptr<TH1F> fhts;
};
#endif /* ifndef LsqMuonRecTool_h */
