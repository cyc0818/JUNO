#include "HECAlg/HECSingleTool.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperPtr.h"
#include "OECTagSvc/OECTagSvc.h"
#include "TMath.h"

DECLARE_TOOL(HECSingleTool);

HECSingleTool::HECSingleTool(const std::string& name):ToolBase(name){
    m_latestMuon=TTimeStamp(-100000, 0);
    declProp("TpreMuonVeto", i_preMuonVetoTime=0);
    declProp("TpostMuonVeto", i_postMuonVetoTime=7000);
}

HECSingleTool::~HECSingleTool(){
}

bool HECSingleTool::initialize(){
    LogInfo<<"initializing classification tool..."<<std::endl;
    //get the event buffer
    SniperDataPtr<JM::NavBuffer> navbuf(getRoot(), "/Event");
    if (navbuf.invalid()){
        LogError<<"can not get navbuffer!!!"<<std::endl;
        return false;
    }
    m_buf=navbuf.data();
    //Get the muon tag for veto
    SniperPtr<OECTagSvc> oectagsvc(getParent(),"OECTagSvc"); 
    if( oectagsvc.invalid()) {
        LogError << "Unable to locate OECTagSvc" << std::endl;
        return false;
    }
    OECTagSvc* m_OECTagSvc = oectagsvc.data();
    std::string stagval = "";
    stagval = m_OECTagSvc->getpTag("CDMuon");
    i_muonTag = strtoll(stagval.c_str(), NULL, 16);
    stagval = m_OECTagSvc->getpTag("Single_ME");
    i_singleTag_ME = strtoll(stagval.c_str(), NULL, 16);
    stagval = m_OECTagSvc->getpTag("Single_LE");
    i_singleTag_LE = strtoll(stagval.c_str(), NULL, 16);
    stagval = m_OECTagSvc->getpTag("Single_HE");
    i_singleTag_HE = strtoll(stagval.c_str(), NULL, 16);
    return true;
}

bool HECSingleTool::finalize(){
    return true;
}

bool HECSingleTool::isVetoed(JM::NavBuffer::Iterator navit){
    //Get this OECEvent
    JM::OecHeader* tHeaderOEC = JM::getHeaderObject<JM::OecHeader>(navit->get());
    JM::OecEvt *tEventOEC = dynamic_cast<JM::OecEvt*>(tHeaderOEC->event("JM::OecEvt"));
    const TTimeStamp& ttime=tEventOEC->getTime();

    //Update the latest muon
    if (isCDMuon(tEventOEC->getTag())){//CD muon
        if (m_latestMuon<ttime) m_latestMuon = ttime;
        return true;
    }


    //post muon veto
    if (m_latestMuon<ttime){
        double dt=(ttime-m_latestMuon)*1e9;
        if (dt<i_postMuonVetoTime) return true;
    }

    //pre muon veto
    for (JM::NavBuffer::Iterator tmpit=++navit;tmpit!=m_buf->end();++tmpit){
        JM::OecHeader* aHeaderOEC = JM::getHeaderObject<JM::OecHeader>(tmpit->get());
        JM::OecEvt *aEventOEC=dynamic_cast<JM::OecEvt*>(aHeaderOEC->event("JM::OecEvt"));
        const TTimeStamp& afterTime=aEventOEC->getTime();
        double dt=(afterTime-ttime)*1e9;
        if (dt>i_preMuonVetoTime) break;
        if (isCDMuon(aEventOEC->getTag())) return true;//dt<i_preMuonVetoTime && aEventOEC is muon
    }

    return false;
}

bool HECSingleTool::findCorrelation(JM::NavBuffer::Iterator navit, std::vector<JM::OecEvt*>& corEvts){
    //For singles, do not need to find correlations
    if (corEvts.size()!=1){
        LogError<<"Current event is not pushed"<<std::endl;
        return false;
    }
    return true;
}

bool HECSingleTool::calClassifyQuantity(const std::vector<JM::OecEvt*>& corEvts){
    int totEvts = corEvts.size();
    if (totEvts!=1){
        LogDebug<<"Only 1 event is needed for singles!"<<std::endl;
        return false;
    }
    //Singles pair with itself
    m_eventPair.clear();
    m_classQuantity.clear();

    classifyQuantity thecondition = classifyQuantity({"R", "singleType"});
    JM::OecEvt *tEvent = corEvts.at(0);
    double tvtxX=tEvent->getVertexX();
    double tvtxY=tEvent->getVertexY();
    double tvtxZ=tEvent->getVertexZ();
    int ttag = tEvent->getTag();

    float r = TMath::Sqrt(tvtxX*tvtxX+tvtxY*tvtxY+tvtxZ*tvtxZ);
    float stype = -1;
    if ((ttag | ~i_singleTag_ME)==0xFFFFFFFF) stype = 0;
    if ((ttag | ~i_singleTag_LE)==0xFFFFFFFF) stype = 1;
    if ((ttag | ~i_singleTag_HE)==0xFFFFFFFF) stype = 2;
    thecondition.setValue("R", r);
    thecondition.setValue("singleType", stype);
    m_eventPair.push_back(std::make_pair(tEvent, tEvent));
    m_classQuantity.push_back(thecondition);
    return true;
}

bool HECSingleTool::userFunc(JM::EvtNavigator*, const EventPair&){
    return true;
}
