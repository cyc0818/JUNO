#include "HECAlg/HECDefaultTool.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperDataPtr.h"
#include "TMath.h"

DECLARE_TOOL(HECDefaultTool);

HECDefaultTool::HECDefaultTool(const std::string &name):ToolBase(name){
    m_latestMuon=TTimeStamp(-100000, 0);
    declProp("maxN", i_maxN=2);
    declProp("dT", i_dT);
    declProp("TpreMuonVeto", d_preMuonVetoTime=1000);
    declProp("TpostMuonVeto", d_postMuonVetoTime=1500000);
}

HECDefaultTool::~HECDefaultTool(){
}

bool HECDefaultTool::initialize(){
    LogInfo<<"initializing classification tool..."<<std::endl;
    //get the event buffer
    SniperDataPtr<JM::NavBuffer> navbuf(getRoot(), "/Event");
    if (navbuf.invalid()){
        LogError<<"can not get navbuffer!!!"<<std::endl;
        return false;
    }
    m_buf=navbuf.data();
    return true;
}

bool HECDefaultTool::finalize(){
    return true;
}

bool HECDefaultTool::isVetoed(JM::NavBuffer::Iterator navit){
    ////By default, do not perform muon veto
    //return false;
    //Get this OECEvent
    JM::OecHeader* tHeaderOEC = JM::getHeaderObject<JM::OecHeader>(navit->get());
    JM::OecEvt *tEventOEC = dynamic_cast<JM::OecEvt*>(tHeaderOEC->event("JM::OecEvt"));
    const TTimeStamp& ttime=tEventOEC->getTime();

    //Update the latest muon
    if (tEventOEC->isMuon() && m_latestMuon<ttime){
        m_latestMuon = ttime;
        return true;
    }

    //post muon veto
    double dt=(ttime-m_latestMuon)*1e9;
    if (dt<d_postMuonVetoTime) return true;

    //pre muon veto
    for (JM::NavBuffer::Iterator tmpit=++navit;tmpit!=m_buf->end();++tmpit){
        JM::OecHeader* aHeaderOEC = JM::getHeaderObject<JM::OecHeader>(tmpit->get());
        JM::OecEvt *aEventOEC=dynamic_cast<JM::OecEvt*>(aHeaderOEC->event("JM::OecEvt"));
        const TTimeStamp& afterTime=aEventOEC->getTime();
        dt=(afterTime-ttime)*1e9;
        if (dt>d_preMuonVetoTime) break;
        if (aEventOEC->isMuon()){//dt<d_preMuonVetoTime && aEventOEC is muon
            return true;
        }
    }

    return false;
}

bool HECDefaultTool::findCorrelation(JM::NavBuffer::Iterator navit, std::vector<JM::OecEvt*>& corEvts){
    if (corEvts.size()!=1){
        LogError<<"Current event is not pushed"<<std::endl;
        return false;
    }
    const TTimeStamp& thisTime=corEvts.at(0)->getTime();
    for (JM::NavBuffer::Iterator tmpit=++navit;tmpit!=m_buf->end();++tmpit){
        JM::OecHeader* aHeaderOEC = JM::getHeaderObject<JM::OecHeader>(tmpit->get());
        JM::OecEvt *aEventOEC = dynamic_cast<JM::OecEvt*>(aHeaderOEC->event("JM::OecEvt"));
        const TTimeStamp& afterTime=aEventOEC->getTime();
        double dt=(afterTime-thisTime)*1e9;
        if (dt>i_dT) break;
        if (this->isVetoed(tmpit)) continue;
        corEvts.push_back(aEventOEC);
    }
    int totEvts = corEvts.size();
    if (totEvts>i_maxN){
        LogDebug<<"Multi tags:"<<totEvts<<std::endl;
        return false;
    }
    if (totEvts==1){
        LogDebug<<"Single tag:0"<<std::endl;
        return false;
    }
    return true;
}

bool HECDefaultTool::calClassifyQuantity(const std::vector<JM::OecEvt*>& corEvents){
    int totEvts = corEvents.size();
    if (totEvts<2){
        LogDebug<<"Cannot calculate the quantities for classification:number of correlated events less than 2!"<<std::endl;
        return false;
    }
    m_eventPair.clear();
    m_classQuantity.clear();

    classifyQuantity thecondition = classifyQuantity({"penergy", "denergy", "distance", "deltaT"});
    JM::OecEvt *tEvent = corEvents.at(0);
    double tvtxX=tEvent->getVertexX();
    double tvtxY=tEvent->getVertexY();
    double tvtxZ=tEvent->getVertexZ();
    for (int ii=1;ii<totEvts;ii++){
        JM::OecEvt *aEvent = corEvents.at(ii);
        double dvtxX=aEvent->getVertexX();
        double dvtxY=aEvent->getVertexY();
        double dvtxZ=aEvent->getVertexZ();
        double dx=tvtxX-dvtxX;
        double dy=tvtxY-dvtxY;
        double dz=tvtxZ-dvtxZ;
        float dist=TMath::Sqrt(dx*dx+dy*dy+dz*dz);
        float dT=1e9*(aEvent->getTime()-tEvent->getTime());
        float penergy=tEvent->getEnergy();
        float denergy=aEvent->getEnergy();
        thecondition.setValue("penergy", penergy);
        thecondition.setValue("denergy", denergy);
        thecondition.setValue("distance", dist);
        thecondition.setValue("deltaT", dT);
        m_eventPair.push_back(std::make_pair(tEvent, aEvent));
        m_classQuantity.push_back(thecondition);
    }
    return true;
}

bool HECDefaultTool::userFunc(JM::EvtNavigator* thisnav, const EventPair& thispair){
    return true;
}
