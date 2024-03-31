#ifndef CdWClippingMuonRecTool_h
#define CdWClippingMuonRecTool_h
#include "RecTools/IRecMuonTool.h"
#include "RecTools/PmtProp.h"
#include "RecTools/Params.h"
#include "SniperKernel/ToolBase.h"
#include "Event/CdTrackRecHeader.h"
#include "Math/Minimizer.h"
#include "Recon_clipping.h"

class CdWClippingMuonRecTool : public IRecMuonTool, public ToolBase{ // : public TObject of Root is inherit 

public:
   CdWClippingMuonRecTool(const std::string& name);
   virtual ~CdWClippingMuonRecTool();
   //public fuctions
   bool reconstruct(RecTrks*);
   bool configure(const Params*,const PmtTable*);

private:
   void iniargs(TVector3&, double&, TVector3&, double&);
   int pmtCut();
private:
   Recon* m_rec; //input tree for rec
   int m_evtid;

   //double PMTRadius;
   double m_pmtUsedNumCut_min, m_pmtUsedNumCut_max;
   
   const PmtTable* m_oriptable;
   PmtTable m_ptable;
};
#endif
