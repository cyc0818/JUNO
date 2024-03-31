#include "OECPruneGBAlg.hh"

#include "SniperKernel/AlgFactory.h"
DECLARE_ALGORITHM(OECPruneGBAlg);

OECPruneGBAlg::OECPruneGBAlg(const std::string& name)
    : AlgBase(name), m_buf(nullptr), m_navbuf_input(nullptr),
      m_memmgr_lec(nullptr), m_tagsvc(nullptr),
      m_task_input(nullptr), m_task_hec(nullptr), m_task_output(nullptr) {
    declProp("InputTaskName", m_taskname_input="HECInputTask");
    declProp("HECTaskName", m_taskname_hec="HECTask");
    declProp("OutputTaskName", m_taskname_output="HECOutputTask");

    declProp("TimeWindow",v_bufRange);

}

OECPruneGBAlg::~OECPruneGBAlg() {

}

bool OECPruneGBAlg::initialize() {
    // ========================================================================
    // retrieve the tasks
    // ========================================================================

    LogInfo<<"initializing tasks in OECPruneGBAlg!"<<std::endl;
    SniperPtr<Task> task_input(getParent(), m_taskname_input);
    if (task_input.invalid()) {
        LogError << "Failed to find the input task " << m_taskname_input << std::endl;
        return false;
    }
    m_task_input = task_input.data();

    SniperPtr<Task> task_hec(getParent(), m_taskname_hec);
    if (task_hec.invalid()) {
        LogError << "Failed to find the HEC task " << m_taskname_hec << std::endl;
        return false;
    }
    m_task_hec = task_hec.data();

    SniperPtr<Task> task_output(getParent(), m_taskname_output);
    if (task_output.invalid()) {
        LogError << "Failed to find the output task " << m_taskname_output << std::endl;
        return false;
    }
    m_task_output = task_output.data();


    // ========================================================================
    // setup the necessary services (see OECProcessorAlg)
    // ========================================================================

    LogInfo<<"initializing services in OECPruneGBAlg!"<<std::endl;
    if (v_bufRange.size()<2){
        v_bufRange.resize(2);
        v_bufRange[0]=0.0;
        v_bufRange[1]=0.0;
    }

    SniperPtr<DataMemSvc> msvc(*getParent(),"DataMemSvc");
    msvc->regist("/Event",new OECBuffer(v_bufRange[0],v_bufRange[1]));

    SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = dynamic_cast<OECBuffer*>(navBuf.data());


    SniperPtr<OECTagSvc> tagsvc(getParent(),"OECTagSvc");
    if (tagsvc.invalid()){
        LogError << "Unable to locate OECTagSvc" << std::endl;
        return false;
    }
    m_tagsvc = tagsvc.data();
    m_output_helper = new OECOutputHelper(m_tagsvc);


    return true;
}

bool OECPruneGBAlg::execute() {

    // ========================================================================
    // Input Task
    // ========================================================================
    LogDebug<<"check if there is enough events in OECBuffer."<<std::endl;
    while (m_buf->t2Ceiling(1)<v_bufRange[1]) {
        bool st = Incident::fire(*getRoot(), m_taskname_input);
        if (!st) {
            LogError << "Fail to fire Task: " << m_taskname_input << std::endl;
            return false;
        }
        if (m_task_input->Snoopy().state()==Sniper::RunState::Stopped) {
            break;               // TODO: maybe just break the while loop?
            throw StopRunProcess(); // TODO
        }

        //store the event read by input_Task in the OECBuffer
        if (!m_navbuf_input){
            SniperDataPtr<JM::NavBuffer> navbuf(m_task_input, "/Event");
            if (navbuf.invalid()){
                LogError << "can't get the NavBuffer in " << m_taskname_input << std::endl;
                return false;
            }
            m_navbuf_input = navbuf.data();
        }

        // put the evtnav into OECBuffer
        JM::NavBuffer::ElementPtr nav=*m_navbuf_input->current();
        m_buf->adopt(nav);
    }


    bool nextStat=m_buf->next();
    if (!nextStat){
        LogInfo<<"Poping all!"<<std::endl;
        m_buf->popAll();
        if (!writeOutput()) return false; // TODO
        dynamic_cast<Task*>(getParent())->stop(Sniper::StopRun::Peacefully);
        return true;
    }

    // ========================================================================
    // HEC Task
    // ========================================================================
    bool st = Incident::fire(*getRoot(), m_taskname_hec);

    // ========================================================================
    // Output Task
    // ========================================================================
    // st = Incident::fire(*getRoot(), m_taskname_output);
    if (!writeOutput()) return false; // TODO

    return true;
}

bool OECPruneGBAlg::finalize() {
    delete m_output_helper;
    return true;
}

bool OECPruneGBAlg::writeOutput(){
    //store waveform or T/Q according to OECTag
    std::shared_ptr<JM::EvtNavigator> pevtnav=m_buf->realPop();
    while (pevtnav){
        //JM::EvtNavigator *oecnav=m_buf->curEvt();
        JM::EvtNavigator *oecnav=pevtnav.get();
        JM::EvtNavigator *outnav=new JM::EvtNavigator();
        m_output_helper->copy(oecnav, outnav);
        

        SniperPtr<IDataMemMgr> mMgr(m_task_output, "BufferMemMgr");
        if ( mMgr.invalid() ) {
            LogError << "cannot get the BufferMemMgr." << std::endl;
            return false;
        }
        IDataMemMgr * m_mMgr=mMgr.data(); 
        m_mMgr->adopt(outnav,"/Event");
        if(!Incident::fire(*getRoot(), m_taskname_output)) return false;
        pevtnav=m_buf->realPop();
    }
    return true;
}

