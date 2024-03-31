#include "OECProcessorAlg.h"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/DataMemSvc.h"

#include "EvtNavigator/EvtNavHelper.h"
#include "Event/OecHeader.h"
#include "Event/CdWaveformHeader.h"
#include "Event/WpWaveformHeader.h"
#include "Event/GenHeader.h"
#include "Event/SimHeader.h"
#include "Event/CdLpmtElecTruthHeader.h"
#include "Event/CdSpmtElecTruthHeader.h"
#include "Event/WpElecTruthHeader.h"
#include "Event/TtElecTruthHeader.h"
#include "Event/TrackElecTruthHeader.h"
#include "Event/CdTriggerHeader.h"
#include "Event/WpTriggerHeader.h"
#include "Event/TtTriggerHeader.h"

#include "OECOutputHelper.hh"

DECLARE_ALGORITHM(OECProcessorAlg);
OECProcessorAlg::OECProcessorAlg(const std::string &name):AlgBase(name){
    declProp("TimeWindow",v_bufRange);
}

OECProcessorAlg::~OECProcessorAlg(){
    //if (m_buf!=NULL) delete m_buf;
}

bool OECProcessorAlg::initialize(){
    LogInfo<<"initializing OECProcessorAlg!"<<std::endl;
    if (v_bufRange.size()<2){
        v_bufRange.resize(2);
        v_bufRange[0]=0.0;
        v_bufRange[1]=0.0;
    }

    SniperPtr<DataMemSvc> msvc(*getParent(),"DataMemSvc");
    msvc->regist("/Event",new OECBuffer(v_bufRange[0],v_bufRange[1]));

    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = dynamic_cast<OECBuffer*>(navBuf.data());

    m_navbuf_input = NULL;
    m_memmgr_lec = NULL; 

    SniperPtr<OECTagSvc> tagsvc(getParent(),"OECTagSvc");
    if (tagsvc.invalid()){
        LogError << "Unable to locate OECTagSvc" << std::endl;
        return false;
    }
    m_tagsvc = tagsvc.data();

    m_output_helper = new OECOutputHelper(m_tagsvc);

#ifdef HXDEBUG
    //timer service
    SniperPtr<IJunoTimerSvc>  junotimersvc(getParent(), "JunoTimerSvc");
    if ( junotimersvc.invalid() ) {
        LogError << "cannot get the junotimer service." << std::endl;
        return false;
    }
    m_junotimersvc = junotimersvc.data();
    for (int i=0;i<2;i++){
        m_timerevent[i]=m_junotimersvc->get(Form("OECEVT/timerevt%d",i));
    }
    //root writer
    SniperPtr<RootWriter> rwsvc(getParent(),"RootWriter");
    if ( rwsvc.invalid() ) {
        LogError << "cannot get the rootwriter service." << std::endl;
        return false;
    }
    m_OECProcessor[0]=rwsvc->bookTree(*m_par, "OECProcessorLEC","tree to store input and LEC time");
    m_OECProcessor[1]=rwsvc->bookTree(*m_par, "OECProcessorHEC","tree to store HEC and output time and memory");
    m_OECProcessor[0]->Branch("inputtime",&f_time[1],"inputtime/f");
    m_OECProcessor[0]->Branch("LECtime",&f_time[2],"LECtime/f");
    m_OECProcessor[0]->Branch("memoryLEC",&f_memory2,"LECmemory/f");

    m_OECProcessor[1]->Branch("memory",&f_memory,"memory/f");
    N_mem = 2;
    m_OECProcessor[1]->Branch("N_mem", &N_mem, "N_mem/I");                   
    m_OECProcessor[1]->Branch("memoryHEC",&f_memory3,"memoryHEC[N_mem]/f");    
    m_OECProcessor[1]->Branch("totaltime",&f_time[0],"totaltime/f");
    m_OECProcessor[1]->Branch("HECtime",&f_time[3],"HECtime/f");
    m_OECProcessor[1]->Branch("outputtime",&f_time[4],"outputtime/f");
    rwsvc->attach("USER_OUTPUT", m_OECProcessor[0]);
    rwsvc->attach("USER_OUTPUT", m_OECProcessor[1]);    
#endif


    return true;
}

bool OECProcessorAlg::finalize(){
    LogInfo<<"finalizing OECProcessorAlg!"<<std::endl;
    delete m_output_helper;
    //delete m_buf;
    return true;
}

bool OECProcessorAlg::execute(){
#ifdef HXDEBUG
    ProcInfo_t info;
    gSystem->GetProcInfo(&info);
    f_memory = info.fMemResident/1024;
    m_timerevent[0]->start();
#endif
    LogDebug<<"check if there is enough events in OECBuffer."<<std::endl;
    while (m_buf->t2Ceiling(1)<v_bufRange[1]){
#ifdef HXDEBUG
        m_timerevent[1]->start();
        ProcInfo_t info2;
        gSystem->GetProcInfo(&info2);
        f_memory2 = info2.fMemResident/1024;
#endif
        if(!fireTask("input_Task")) break;

        //store the event read by input_Task in the OECBuffer
        if (m_navbuf_input==NULL){
            SniperDataPtr<JM::NavBuffer> navbuf(dynamic_cast<Task*>(getParent()->find("input_Task")),"/Event");
            if (navbuf.invalid()){
                LogError<<"can't get the NavBuffer in input_Task"<<std::endl;
                return false;
            }
            m_navbuf_input = navbuf.data();
        }
        JM::NavBuffer::ElementPtr nav=*m_navbuf_input->current();
        LogDebug<<"get an event:"<<nav->TimeStamp()<<std::endl;
        //create OECEvent
        JM::OecEvt *thisEvent = new JM::OecEvt();     //init event
        JM::OecHeader *thisHeader = new JM::OecHeader();  //init header 
        thisEvent->setTime(nav->TimeStamp());
        thisHeader->setEvent(thisEvent);
        JM::addHeaderObject(nav.get(), thisHeader);
        m_buf->adopt(nav);

        //store the event read by input_Task in the bufmemmgr service in LEC_Task
        if (m_memmgr_lec==NULL){
            SniperPtr<IDataMemMgr> mMgr(dynamic_cast<Task*>(getRoot()->find("LEC_Task")), "BufferMemMgr");
            if ( mMgr.invalid() ) {
                LogError << "cannot get the BufferMemMgr." << std::endl;
                return false;
            }
            m_memmgr_lec = mMgr.data(); 
        }
        JM::EvtNavigator *nav_lec = new JM::EvtNavigator();
        std::vector<std::string>& newnavpath=nav->getPath();
        for (std::vector<std::string>::const_iterator it=newnavpath.begin();it!=newnavpath.end();++it){
            nav_lec->addHeader(*it,dynamic_cast<JM::HeaderObject*>(nav->getHeader(*it)));
        }
        nav_lec->setTimeStamp(nav->TimeStamp());
        nav_lec->setEventID(nav->EventID());
        nav_lec->setRunID(nav->RunID());
        m_memmgr_lec->adopt(nav_lec,"/Event");

#ifdef HXDEBUG
        m_timerevent[1]->stop();
        f_time[1]=m_timerevent[1]->elapsed();
        m_timerevent[1]->start();
#endif
        if(!fireTask("LEC_Task")) return false;
#ifdef HXDEBUG
        m_timerevent[1]->stop();
        f_time[2]=m_timerevent[1]->elapsed();
        m_OECProcessor[0]->Fill();
        ProcInfo_t info3;
        gSystem->GetProcInfo(&info3);
        f_memory3[0] = info3.fMemResident/1024;            
#endif

        //TODO
        //Get the CalibEvt in LEC_Task (if any) and store it in the nav
        JM::CdLpmtCalibHeader* calib_cdltq = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav_lec);
        if (calib_cdltq!=NULL) JM::addHeaderObject(nav.get(), calib_cdltq);
        JM::CdSpmtCalibHeader* calib_cdstq = JM::getHeaderObject<JM::CdSpmtCalibHeader>(nav_lec);
        if (calib_cdstq!=NULL) JM::addHeaderObject(nav.get(), calib_cdstq);
        JM::WpCalibHeader* calib_wptq = JM::getHeaderObject<JM::WpCalibHeader>(nav_lec);
        if (calib_wptq!=NULL) JM::addHeaderObject(nav.get(), calib_wptq);

        //break;
    }
    bool nextStat=m_buf->next();
    if (!nextStat){
        LogInfo<<"poping all!"<<std::endl;
        m_buf->popAll();
        if (!writeOutput()) return false;
        dynamic_cast<Task*>(getParent())->stop(Sniper::StopRun::Peacefully);
        return true;
    }
#ifdef HXDEBUG
    m_timerevent[1]->start();
#endif
    if(!fireTask("HEC_Task")) return false;
#ifdef HXDEBUG
    m_timerevent[1]->stop();
    f_time[3]=m_timerevent[1]->elapsed();
    m_timerevent[1]->start();
    ProcInfo_t info4;
    gSystem->GetProcInfo(&info4);
    f_memory3[1] = info4.fMemResident/1024;    
#endif

    if (!writeOutput()) return false;
#ifdef HXDEBUG
    m_timerevent[1]->stop();
    f_time[4]=m_timerevent[1]->elapsed();
    m_timerevent[0]->stop();
    f_time[0]=m_timerevent[0]->elapsed();
    m_OECProcessor[1]->Fill();
#endif

    return true;
}

bool OECProcessorAlg::fireTask(std::string subtask){
    try{
        bool ft=Incident::fire(*getRoot(),subtask);
        if (!ft){
            LogError<<"fail to fire subTask:"<<subtask<<std::endl;
            return false;
        }
        SniperPtr<Task> ftask(getRoot(), subtask); 
        if (ftask->Snoopy().state()==Sniper::RunState::Stopped) {
            return false;
        }
    }
    catch (StopRunProcess& e){
        LogInfo<<"Catch stop!"<<std::endl;
        return false;
    }

    return true;
}

bool OECProcessorAlg::writeOutput(){
    //store waveform or T/Q according to OECTag
    std::shared_ptr<JM::EvtNavigator> pevtnav=m_buf->realPop();
    while (pevtnav){
        //JM::EvtNavigator *oecnav=m_buf->curEvt();
        JM::EvtNavigator *oecnav=pevtnav.get();
        JM::EvtNavigator *outnav=new JM::EvtNavigator();
        m_output_helper->copy(oecnav, outnav);
        

        SniperPtr<IDataMemMgr> mMgr(dynamic_cast<Task*>(getParent()->find("output_Task")), "BufferMemMgr");
        if ( mMgr.invalid() ) {
            LogError << "cannot get the BufferMemMgr." << std::endl;
            return false;
        }
        IDataMemMgr * m_mMgr=mMgr.data(); 
        m_mMgr->adopt(outnav,"/Event");
        if(!fireTask("output_Task")) return false;
        pevtnav=m_buf->realPop();
    }
    return true;
}

