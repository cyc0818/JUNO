#include "EvtSteering/StepHandler.h"
#include "EvtSteering/StepSequencer.h"
#include "EvtSteering/StepDecision.h"
#include "SniperKernel/AlgFactory.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/OecHeader.h"
#include <algorithm>

DECLARE_ALGORITHM(StepHandler);

StepHandler::StepHandler(const std::string &name): AlgBase(name){
}

StepHandler::~StepHandler(){
}

bool StepHandler::initialize(){
    m_sequencer = tool<StepSequencer>("StepSequencer");
    if (!m_sequencer->initialize()) return false;

    m_decision = tool<StepDecision>("StepDecision");
    if (!m_decision->initialize()) return false;

    //check the sequence_id and signature_id
    m_seqID = m_sequencer->getSeqIDs();
    std::vector<std::string> vsigid = m_decision->getSigIDs();
    for (std::string sid : m_seqID){
        if (find(vsigid.begin(), vsigid.end(), sid)==vsigid.end()){
            LogInfo<< "No matched Signature to Sequence(" <<sid<< ")" << std::endl;
            return false;
        }
    }

#ifdef HXDEBUG
    //timer service
    SniperPtr<IJunoTimerSvc>  junotimersvc(getRoot(), "JunoTimerSvc");
    if ( junotimersvc.invalid() ) {
        LogError << "cannot get the junotimer service." << std::endl;
        return false;
    }
    m_junotimersvc = junotimersvc.data();
    for (int i=0;i<5;i++){
        m_timerevent[i]=m_junotimersvc->get(Form("LECEVT/timerevt%d",i));
    }
    //root writer
    SniperPtr<RootWriter> rwsvc(getRoot(),"RootWriter");
    if ( rwsvc.invalid() ) {
        LogError << "cannot get the rootwriter service." << std::endl;
        return false;
    }
    m_LEC=rwsvc->bookTree(*m_par, "LEC","tree to store LEC execute time");
    m_LEC->Branch("totaltime",&f_time[0],"totaltime/f");
    for (int i=1;i<5;i++){
        m_LEC->Branch(Form("time%d",i),&f_time[i],Form("time%d/f",i));
    }
    rwsvc->attach("USER_OUTPUT", m_LEC);
#endif

    return true;
}

bool StepHandler::execute(){
    LogDebug << "*** Executing StepHandler *** " << std::endl;

#ifdef HXDEBUG
    m_timerevent[0]->start();
#endif
    //m_timer[0]->start();

    bool StepSeq = true;
    int  StepDec = -999;


    std::string answer, result, evtMark, tag;
    std::string::size_type separator;

    //SniperPtr<EvtResultSvc> hltResultSvc(getParent(),"EvtResultSvc");

    std::vector<std::string>::const_iterator id = m_seqID.begin();
    int n_timer=0;
    while (id != m_seqID.end()) {
        LogDebug << "In sequencerAlg  step " << (*id) << std::endl;
#ifdef HXDEBUG
        n_timer++;
        m_timerevent[n_timer]->start();
#endif
        //n_timer++;
        //m_timer[n_timer]->start();    

        StepSeq = m_sequencer->execSequence(*id);
        if (!StepSeq) {
            LogDebug << " Step Sequencer failed" << std::endl;
            id++;
            continue;
        } 
#ifdef HXDEBUG
        m_timerevent[n_timer]->stop();
        f_time[n_timer]=m_timerevent[n_timer]->elapsed();
        n_timer++;
        m_timerevent[n_timer]->start();
#endif
        //m_timer[n_timer]->stop();


        //n_timer++;
        //m_timer[n_timer]->start();
        LogDebug << "In decisionAlg step " << (*id) << std::endl;
        StepDec = m_decision->execDecision(*id);

        //perform=='continue'
        std::string midAnswer = m_decision->getContinue();
        if (midAnswer!="") {
            //hltResultSvc->setMidAnswer(midAnswer);
            LogDebug << "StepDec: "<<StepDec<<" ,midAnswer:"<<midAnswer << std::endl;
        }
        if (StepDec>-1){
            //hltResultSvc->push_back(StepDec);
        }
        else {
            LogDebug << " Execute next step" << std::endl;
            //hltResultSvc->push_back(0);
            id++;
            continue;
        }
#ifdef HXDEBUG
        m_timerevent[n_timer]->stop();
        f_time[n_timer]=m_timerevent[n_timer]->elapsed();
#endif
        //m_timer[n_timer]->stop();

        answer = m_decision->getBehaviour();
        LogDebug << " Matched Criteria, Behaviour = " << answer << std::endl;

        separator = answer.find('_');
        if (separator == std::string::npos) {
            LogError << " Unknown decision result: "<< answer << std::endl;
            return false;
        }
        std::string perform = answer.substr(0, separator);

        if (perform == "reject") {
            result = "Rejected";
            break;
        }
        else if (perform == "accept") {
            result = "Accepted";
            break;
        }
        else if (perform == "jump") {
            std::string nextId = answer.substr(separator+1);
            do {
                ++id;
            }	while ((id != m_seqID.end()) && ((*id) != nextId));
            if (id == m_seqID.end()) {
                LogError << " Unknown jump target: "<< nextId << std::endl;
                return false;
            }
        }
        else {
            LogError << " Unknown decision result: "<< answer << std::endl;
            return false;
        }
    }

    if (result == "Accepted") {
        std::string tempstr=answer.substr(separator+1);
        std::size_t pos=tempstr.find("-");
        if (pos==std::string::npos){
            LogInfo<<"WARNING: tag value is not set, 0 is used!"<<std::endl;
            evtMark=tempstr;
            tag="0x0";
        }
        else{
            evtMark=tempstr.substr(0,pos);
            tag=tempstr.substr(pos+1);
        }
        LogInfo<< " Event is Accepted as " << evtMark << "  tag value:"<<tag<<std::endl;
    }
    else if (result == "Rejected") {
        // only beam-gas rejected defaultly
        // other type event not decided in Event Filter
        // by Fu Chengdong
        evtMark = "Beamgas";
        tag="0x0";
        LogDebug << " Event is Rejected" << std::endl;
    }
    else {
        evtMark="Unknown";
        tag="0x0";
        LogDebug << " Unknown Behaviour, force to Reject" << std::endl;
        result = "Rejected";
    }

    SniperDataPtr<JM::NavBuffer> navbuf(getParent(), "/Event");
    JM::NavBuffer *m_buf=navbuf.data();
    JM::EvtNavigator* tnav=m_buf->curEvt();
    JM::OecHeader* tEventHeader = JM::getHeaderObject<JM::OecHeader>(tnav);
    if (tEventHeader==NULL){
        LogError<<"The OecHeader is not found!"<<std::endl;
        return false;
    }
    JM::OecEvt* tEvent = dynamic_cast<JM::OecEvt*>(tEventHeader->event("JM::OecEvt"));
    int itag=strtoll(tag.c_str(),NULL,16);
    //std::cout<<"hx:"<<itag<<std::endl;
    tEvent->addTag(itag); 
    //tEvent->addTag(evtMark); 
#ifdef HXDEBUG
    m_timerevent[0]->stop();
    f_time[0]=m_timerevent[0]->elapsed();
    m_LEC->Fill();
    //m_timer[0]->stop();
    //float t_time[5];
    //for(int i=0;i<5;i++){
    //  t_time[i] = m_timer[i]->elapsed();
    //}
    LogInfo << " ----------Test Time---------" << std::endl;
    LogInfo << " stepHandler time = "<<f_time[0] << std::endl;
    LogInfo << " 1stStep exe time = "<<f_time[1] << std::endl;
    LogInfo << " 1stStep deci time = "<<f_time[2] << std::endl;
    LogInfo << " 2stStep exe time = "<<f_time[3] << std::endl;
    LogInfo << " 2stStep deci time = "<<f_time[4] << std::endl;
#endif


    //hltResultSvc->setAnswer(result);
    //hltResultSvc->setMark(evtMark);

    /*
#ifdef TIME_MEASUREMENT   //// Timer
m_timer[0]->stop();
g_StepHandlerTime=m_timer[0]->elapsed();
if(m_tuple){
StatusCode status = m_tuple->write();
if (!status.isSuccess()) {
LogError << "Can't fill ntuple!" << std::endl;
}
}
#endif
*/
    return true;

    return true;
    }

bool StepHandler::finalize(){
    if (!m_sequencer->finalize()) return false;
    if (!m_decision->finalize()) return false;
    return true;
}
