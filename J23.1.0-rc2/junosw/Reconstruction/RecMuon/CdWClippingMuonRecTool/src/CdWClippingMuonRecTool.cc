#include "CdWClippingMuonRecTool.h"
#include "Recon_clipping.h"
#include "SniperKernel/ToolFactory.h"
#include "TMath.h"

// code review
DECLARE_TOOL(CdWClippingMuonRecTool); 

std::ostream& operator << (std::ostream& s, const TVector3& v){
   s << "(" << v.x() <<  "," << v.y() << "," << v.z() << ")";
   return s;
}
CdWClippingMuonRecTool::CdWClippingMuonRecTool(const std::string& name): ToolBase(name), m_evtid(0), m_oriptable(NULL){
   declProp("NPmtRequired_min", m_pmtUsedNumCut_min = 100);
   declProp("NPmtRequired_max", m_pmtUsedNumCut_max = 17200);
   //declProp("FhtCorrFile", m_corrfilename="");
   //declProp("FhtCorrMethod",m_corrmethod="nocorr");
   //declProp("NumBinsDist",m_nBinDist = 18);
   m_rec = new Recon();
}

CdWClippingMuonRecTool::~CdWClippingMuonRecTool(){
   delete m_rec;
}

bool CdWClippingMuonRecTool::configure(const Params* pars, const PmtTable* ptab){
   LogDebug<<" config reconstructing tool [CdWClippingMuonRecTool] "<<std::endl;
   m_oriptable = ptab;
   return true;
}

bool CdWClippingMuonRecTool::reconstruct(RecTrks* trks){
   LogDebug<<"Reconstructing tracks of CD water clipping muons by pmts ... "<< std::endl;
   m_evtid++;
   m_ptable.assign(m_oriptable->begin(),m_oriptable->end());

   m_rec->set_pmt_num(&m_ptable);
   m_rec->reconstruct();
   TVector3 start, end;
   double totalpe, chi2byndf;
   m_rec->get_rec_num(start, end, totalpe, chi2byndf);
   
   trks->addTrk(start, end, -1, -1, totalpe, chi2byndf, 0);

   LogDebug<<"\033[34m ------ End of the reconstruction of one event ------  \033[0m"<<std::endl;
   return true; 
}