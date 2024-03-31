#include "HECAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "Event/OecHeader.h"

#include <vector>
DECLARE_ALGORITHM(HECAlg);

HECAlg::HECAlg(const std::string &name): AlgBase(name){
    i_nEvt = -1;
    declProp("toolName", v_toolName);
}

HECAlg::~HECAlg(){
}

bool HECAlg::initialize(){
    LogInfo<<"initializing HECAlg..."<<std::endl;

    //get the event buffer
    SniperDataPtr<JM::NavBuffer> navbuf(getRoot(), "/Event");
    if (navbuf.invalid()){
        LogError<<"can not get navbuffer!!!"<<std::endl;
        return false;
    }
    m_buf=navbuf.data();

    //get classification tool
    for (std::string s_toolName : v_toolName){
        IClassTool* thistool = tool<IClassTool>(s_toolName);
        if (!thistool){
            LogError<<"Can not find the classification tool!"<<std::endl;
            return false;
        }
        if (!dynamic_cast<ToolBase*>(thistool)->initialize()) return false;
        v_classTool.push_back(thistool);
    }

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
    for (vector<OECTag*>::iterator it=m_tagList.begin();it!=m_tagList.end();it++){
        LogInfo<<"==>"<<(*it)->getTagName()<<std::endl;
    }
    LogInfo<<"Initializing tag list done."<<std::endl;

#ifdef PERFORMANCE_CHECK    
    //timer service
    SniperPtr<IJunoTimerSvc> junotimersvc(getRoot(), "JunoTimerSvc");
    m_junotimersvc = junotimersvc.data();
    m_timerevent=m_junotimersvc->get("HECALGEVT/timerevt");

    // user data definitions
    SniperPtr<RootWriter> svc(getRoot(), "RootWriter");
    if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
             << "enalbe it in your job option file." << std::endl;
    return false;
    }
    m_hecalg = svc->bookTree(*m_par, "USER_OUTPUT/hecalg", "simple output of qctr rec data");    
    m_hecalg->Branch("time_total",&time_total,"time_total/F");
    N_mem = 2;   
    m_hecalg->Branch("N_mem", &N_mem, "N_mem/I");   
    m_hecalg->Branch("mem_resident",mem_resident,"mem_resident[N_mem]/F"); 

    // for IBD information
    m_ibd = svc->bookTree(*m_par, "USER_OUTPUT/ibd", "output of infomation used in IBD tagging");
    m_ibd_penergy = 0.0, m_ibd_denergy = 0.0, m_ibd_distance = 0.0, m_ibd_deltaT = 0.0;
    m_ibd->Branch("penergy", &m_ibd_penergy, "penergy/F");
    m_ibd->Branch("denergy", &m_ibd_denergy, "denergy/F");
    m_ibd->Branch("distance", &m_ibd_distance, "distance/F");
    m_ibd->Branch("deltaT", &m_ibd_deltaT, "deltaT/F");
#endif
    return true;
}

bool HECAlg::finalize(){
    LogInfo<<"finalizing HECAlg..."<<std::endl;
    for (IClassTool* thistool : v_classTool){
        if (!dynamic_cast<ToolBase*>(thistool)->finalize()) return false;
    }
    return true;
}

bool HECAlg::execute(){
#ifdef PERFORMANCE_CHECK
    // for resources test
    ProcInfo_t mem_info2;
    gSystem->GetProcInfo(&mem_info2);
    mem_resident[0] = mem_info2.fMemResident / 1024./1024.;
    m_timerevent->start();
#endif

    i_nEvt++;
    LogDebug<<"The "<<i_nEvt<<"th event"<<std::endl;

    //----------------Get the current event----------------
    JM::EvtNavigator* tnav=m_buf->curEvt();
    JM::OecHeader* tHeaderOEC = JM::getHeaderObject<JM::OecHeader>(tnav);
    JM::OecEvt *tEventOEC = dynamic_cast<JM::OecEvt*>(tHeaderOEC->event("JM::OecEvt"));
    const TTimeStamp& thisTime=tEventOEC->getTime();
    LogDebug<<thisTime<<std::endl;

    //Use all the tools to find correlations
    for (int ii=0;ii<v_classTool.size();ii++){
        IClassTool* m_classTool = v_classTool.at(ii);
        LogDebug<<"Using tool:"<<dynamic_cast<ToolBase*>(m_classTool)->tag()<<std::endl;

        //muon veto of this event
        if (m_classTool->isVetoed(m_buf->current())) continue;

        //----------------Find the events correlated in time-------------------
        std::vector<JM::OecEvt*> corEvts;
        corEvts.push_back(tEventOEC);//The first event
        if (!m_classTool->findCorrelation(m_buf->current(), corEvts)) continue;
        LogDebug<<"Number of correlation events:"<<corEvts.size()<<std::endl;

        //Calculate the quantities for classification
        if (m_classTool->calClassifyQuantity(corEvts)){
            //Get the event pair and its classification quantities
            const std::vector<IClassTool::EventPair> eventpair = m_classTool->getEventPair();
            const std::vector<classifyQuantity> classifyquantity = m_classTool->getClassQuantity();
            int npairs = eventpair.size();
            LogDebug<<"N pairs:"<<npairs<<std::endl;
            for (int ith=0;ith<npairs;ith++){
                bool isfound = findTag(eventpair.at(ith), classifyquantity.at(ith));
                if (isfound) m_classTool->userFunc(tnav, eventpair.at(ith));
            }
        }
    }

#ifdef PERFORMANCE_CHECK    
    // for resources test
    ProcInfo_t mem_info3;
    gSystem->GetProcInfo(&mem_info3);
    mem_resident[1] = mem_info3.fMemResident / 1024./1024.;
    m_timerevent->stop();
    time_total=m_timerevent->elapsed()/1000.;
    LogInfo << "Time consumes: " << time_total << " s"<<endl;  
    m_hecalg->Fill();
#endif   

    return true;
}

bool HECAlg::findTag(const IClassTool::EventPair &thispair, const classifyQuantity& thiscondition){
    int nquantities = thiscondition.getN();
    const std::vector<std::string>& thename = thiscondition.getName();
    std::vector<float> thevalue = thiscondition.getValue();

    for (vector<OECTag*>::iterator it=m_tagList.begin();it!=m_tagList.end();it++){//iterate different predefined tag
        vector<OECTag::Condition> tempCondition=(*it)->getConditionVec();//get the tag condition of the current tag(it)
        unsigned int condcount=0;
        for (vector<OECTag::Condition>::iterator condit=tempCondition.begin();condit!=tempCondition.end();condit++){//iterate the conditions
            float tmpvar = -1;
            bool hasvar = false;
            for (auto thisname : thename){//find the corresponding condition value
                if (!(*condit).name.compare(thisname)){
                    tmpvar = thiscondition.getValue(thisname);;
                    hasvar = true;
                }
            }
            if (!hasvar) break;
            //Check if this condition is satisfied or not
            if (tmpvar<(*condit).max && tmpvar>=(*condit).min){
                condcount++;
                continue;
            }
            else break;
        }

        //Check if all conditions are satisfied
        if (condcount==tempCondition.size()){
            int ptag=(*it)->getpTagValue();
            int dtag=(*it)->getdTagValue();
            std::string tmpTagName=(*it)->getTagName();
            LogInfo<<"Find a tag:"<<tmpTagName<<std::endl;
            //Add the tag value into OECEvent
            JM::OecEvt *pevent = thispair.first;
            JM::OecEvt *devent = thispair.second;
            pevent->addTag(ptag);
            devent->addTag(dtag);

#ifdef PERFORMANCE_CHECK
            //for IBD information
            if(tmpTagName=="Correlation"){
                m_ibd_penergy = thiscondition.getValue("penergy");
                m_ibd_denergy = thiscondition.getValue("denergy");
                m_ibd_distance = thiscondition.getValue("distance");
                m_ibd_deltaT = thiscondition.getValue("deltaT");
                m_ibd->Fill();
            }
#endif

            return true;
        }
    }
    return false;
}
