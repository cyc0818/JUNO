#include "SniperKernel/AlgFactory.h"
#include "TSystem.h"

#include "TMath.h"
#include "Event/ElecHeader.h"

#include "HECMuonAlg.h"
DECLARE_ALGORITHM(HECMuonAlg);

HECMuonAlg::HECMuonAlg(const std::string &name):AlgBase(name){
    Mu_lowE = 20;
    Mu_highE = 100;
    d_distpEvt2tEvt=2500;
    d_muwindow=200;
    d_buffwindow=1e9;
}

HECMuonAlg::~HECMuonAlg(){

}

bool HECMuonAlg::initialize(){
    LogInfo<<"initializing HECMuonAlg..."<<std::endl;
    //get navbuffer
    SniperDataPtr<JM::NavBuffer> navbuf(getRoot(), "/Event");
    if (navbuf.invalid()){
        LogError<<"can not get navbuffer!!!"<<std::endl;
        return false;
    }
    m_buf=navbuf.data();
    //m_buf=dynamic_cast<OECBuffer*>(navbuf.data());

    //get OECConfig service
    SniperPtr<OECConfigSvc> oecconfigsvc(getParent(),"OECConfigSvc"); 
    if( oecconfigsvc.invalid()) {
        LogError << "Unable to locate OECConfigSvc" << std::endl;
        return false;
    }
    m_OECConfigSvc = oecconfigsvc.data();
    //get predefined tags
    m_tagList=m_OECConfigSvc->getOECTagVec();
    //print taglist information
    for (vector<OECTag*>::iterator it=m_tagList.begin();it!=m_tagList.end();it++){//iterate different predefined tag
        LogInfo<<"==>"<<(*it)->getTagName()<<std::endl;
    }
    LogInfo<<"Initializing tag list done."<<std::endl;

#ifdef HXDEBUG
    //timer service
    SniperPtr<IJunoTimerSvc>  junotimersvc(getRoot(), "JunoTimerSvc");
    if ( junotimersvc.invalid() ) {
        LogError << "cannot get the junotimer service." << std::endl;
        return false;
    }
    m_junotimersvc = junotimersvc.data();
    m_timerevent=m_junotimersvc->get("HECEVT/timerevt");
    //root writer
    SniperPtr<RootWriter> rwsvc(getRoot(),"RootWriter");
    if ( rwsvc.invalid() ) {
        LogError << "cannot get the rootwriter service." << std::endl;
        return false;
    }
    m_HECMuon=rwsvc->bookTree(*m_par, "HECMuon","tree to store input and HECMuon time");
    m_HECMuon->Branch("totaltime",&f_time,"totaltime/f");
    m_HECMuon->Branch("tag",&f_tag,"tag/I");
    rwsvc->attach("USER_OUTPUT", m_HECMuon);
#endif

    return true;
}

bool HECMuonAlg::finalize(){
    LogInfo<<"finalizing HECMuonAlg..."<<std::endl;

    return true;
}

bool HECMuonAlg::execute(){
#ifdef HXDEBUG
    m_timerevent->start();
#endif
    LogDebug<<"executing..."<<std::endl;
    //get this event
    JM::EvtNavigator* tnav=m_buf->curEvt();
    JM::OECHeader *tEventHeader=dynamic_cast<JM::OECHeader*>(tnav->getHeader("/Event/OEC"));
    m_tEvent=dynamic_cast<JM::OECEvent*>(tEventHeader->event("JM::OECEvent"));

    const TTimeStamp& ttime=m_tEvent->getTime();
    float m_tenergy = m_tEvent->getEnergy();
    
    LogDebug<<"Initial Tag = "<< hex << m_tEvent->getTag() <<"; energy = "<<m_tenergy<<endl;

    if(m_tenergy > Mu_lowE && m_tenergy < Mu_highE){
       if(relatedTTorWP()){
       m_tEvent->addTag("muCDLE");
       LogDebug<<"Tag (muCDLE) = "<< hex << m_tEvent->getTag() <<endl;
      }
    }
    double mag_MuTrack;
    double dist;
    if(m_tenergy > Mu_highE && m_tEvent->isMuon()){
    LogDebug<<"Tag = "<< hex << m_tEvent->getTag() <<"; energy = "<<m_tenergy<<endl;
    MuTrack.SetXYZ(m_tEvent->getMuInX()-m_tEvent->getMuOutX(),m_tEvent->getMuInY()-m_tEvent->getMuOutY(),m_tEvent->getMuInZ()-m_tEvent->getMuOutZ());
    mag_MuTrack = MuTrack.Mag();
    for (JM::NavBuffer::Iterator navit=++m_buf->current();navit!=m_buf->end();++navit){
        JM::OECHeader *aheader=dynamic_cast<JM::OECHeader*>((*navit)->getHeader("/Event/OEC"));
        JM::OECEvent *aevent=dynamic_cast<JM::OECEvent*>(aheader->event("JM::OECEvent"));
        const TTimeStamp& atime=aevent->getTime();
        TTimeStamp dt(atime.GetSec(),atime.GetNanoSec());
        dt.Add(TTimeStamp(-1*ttime.GetSec(),-1*ttime.GetNanoSec()));
        if (dt.AsDouble()*1e9> d_buffwindow) break;
        if(mag_MuTrack > 0) {
        EventPos.SetXYZ(aevent->getVertexX()-m_tEvent->getMuInX(),aevent->getVertexY()-m_tEvent->getMuInY(),aevent->getVertexZ()-m_tEvent->getMuInZ());
        cross_vector = EventPos.Cross(MuTrack);
        dist = cross_vector.Mag()/mag_MuTrack; 
        }
        else {
        double dx=m_tEvent->getVertexX()-aevent->getVertexX();
        double dy=m_tEvent->getVertexY()-aevent->getVertexY();
        double dz=m_tEvent->getVertexZ()-aevent->getVertexZ();         // distance is wrong
        dist=TMath::Sqrt(dx*dx+dy*dy+dz*dz);
        }
        if(dist>d_distpEvt2tEvt) continue;
        if(relatedTTorWP()) continue;
        double time = 1e9*(aevent->getTime()-m_tEvent->getTime()); 
        float tE=aevent->getEnergy();
        int mytag=getTag(tE,dist,time); 
        LogDebug<<"found tag:"<<mytag<<endl; 
        if (mytag!=0) m_tEvent->addTag(mytag);
    }
    }
#ifdef HXDEBUG
    m_timerevent->stop();
    f_time=m_timerevent->elapsed();
    m_HECMuon->Fill();
#endif

    return true;
}

bool HECMuonAlg::relatedTTorWP(){

    const TTimeStamp& ttime=m_tEvent->getTime();
    bool isrelatedTTorWP = false;
    LogDebug<<"----------related start----------"<<endl;
    for (JM::NavBuffer::Iterator navit=++m_buf->current();navit!=m_buf->end();++navit){
         JM::OECHeader *aheader=dynamic_cast<JM::OECHeader*>((*navit)->getHeader("/Event/OEC"));
         JM::OECEvent *aevent=dynamic_cast<JM::OECEvent*>(aheader->event("JM::OECEvent"));
         const TTimeStamp& atime=aevent->getTime();
         TTimeStamp dt(atime.GetSec(),atime.GetNanoSec()); 
         dt.Add(TTimeStamp(-1*ttime.GetSec(),-1*ttime.GetNanoSec()));
         if (dt.AsDouble()*1e9 > d_muwindow) break;
         if (aevent->isTTMuon() || aevent->isWPMuon()){ 
            isrelatedTTorWP = true;
         }
    }
    JM::NavBuffer::Iterator navit=m_buf->current();
    if(navit!=m_buf->begin()){
    for ( navit=--m_buf->current();navit!=m_buf->begin();--navit){
         JM::OECHeader *aheader=dynamic_cast<JM::OECHeader*>((*navit)->getHeader("/Event/OEC"));
         JM::OECEvent *aevent=dynamic_cast<JM::OECEvent*>(aheader->event("JM::OECEvent"));
         const TTimeStamp& atime=aevent->getTime();
          TTimeStamp dt(atime.GetSec(),atime.GetNanoSec());
         dt.Add(TTimeStamp(-1*ttime.GetSec(),-1*ttime.GetNanoSec()));
         if (dt.AsDouble()*-1*1e9 > d_muwindow) break;
         if (aevent->isTTMuon() || aevent->isWPMuon()){
             isrelatedTTorWP = true;
         }
    }
    }
    LogDebug<<"----------related end----------"<<endl;
    return isrelatedTTorWP;
}

int HECMuonAlg::getTag(float dE, float dist, float deltaT){
    LogDebug<<"dE:"<<dE<<" dist:"<<dist<<" dt:"<<deltaT<<endl;
    int temptag=0;//default value should be set at tagOther
    for (vector<OECTag*>::iterator it=m_tagList.begin();it!=m_tagList.end();it++){//iterate different predefined tag
        vector<OECTag::Condition> tempCondition=(*it)->getConditionVec();//get the tag condition of the current tag(it)
        unsigned int condcount=0;
        //iterate the conditions
        for (vector<OECTag::Condition>::iterator condit=tempCondition.begin();condit!=tempCondition.end();condit++){
            float tempvariable=0;
            //get the variable value
            if (!(*condit).name.compare("mu_denergy")){
                tempvariable=dE;
            }
            else if (!(*condit).name.compare("deltaD")){
                tempvariable=dist;
            }
            else if (!(*condit).name.compare("deltaT")){
                tempvariable=deltaT;
            }
            else {
                LogError<<"unreconginized variable!!!!"<<endl;
                tempvariable=-1;
            }

            //check if condition is satisfied or not
            if (tempvariable<(*condit).max && tempvariable>=(*condit).min){
                condcount++;
                continue;
            }
            else{
                break;
            }
        }

        //check if all conditions are satisfied
        LogDebug<<"conditions sat:"<<condcount<<endl;
        if (condcount==tempCondition.size()){
            temptag=(*it)->getTagValue();
            //LogInfo<<"tagName:"<<(*it)->getTagName()<<endl;
            LogDebug<<"tagName:"<<(*it)->getTagName()<<endl;
            break;
        }
    }

    return temptag;
}


