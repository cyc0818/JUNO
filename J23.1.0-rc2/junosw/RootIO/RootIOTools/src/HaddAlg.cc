#include "HaddAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/Task.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "RootIOSvc/RootOutputSvc.h"
#include "EvtNavigator/NavBuffer.h"

DECLARE_ALGORITHM(HaddAlg);

HaddAlg::HaddAlg(const std::string& name)
    : AlgBase(name)
{
    declProp("OutputFile", m_output="output.root");
    declProp("SubTaskName", m_taskName);
    m_init = false;
}

HaddAlg::~HaddAlg()
{
}

bool HaddAlg::initialize()
{
    LogInfo << "Successfully initialized!" << std::endl;
    return true;
}

bool HaddAlg::execute()
{
    // Trigger sub-task to read in data to be merged
    int this_ret = Incident::fire(*getParent(), "input_task"); 
    //If return value is -1, stop the TopTask after execution
    if (-1 == this_ret) return dynamic_cast<Task*>(getRoot())->stop();

    SniperDataPtr<JM::NavBuffer>  oldNavBuf(dynamic_cast<Task*>(getParent()->find("input_task")), "/Event");
    if ( oldNavBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ input_task:/Event" << std::endl;
        return false;
    }
    // Copy EvtNavigator from sub-task
    JM::EvtNavigator* nav = new JM::EvtNavigator(*(oldNavBuf->curEvt()));

    // Set the output stream
    if (!m_init) { 
        auto paths = nav->getPath();
        std::map<std::string,std::string> output;
        for (auto path: paths) {
            std::cout << "LT DEBUG: " << path << std::endl;
            output.insert({path,m_output});
        }
        SniperPtr<RootOutputSvc> osvc(getParent(), "OutputSvc");
        if ( osvc.invalid()) {
            LogError << "cannot get output svc @ top_task" << std::endl;
            return false;
        }
        osvc->setOutputStreams(output);
        m_init = true;
    }
    SniperPtr<IDataMemMgr> mMgr(getParent(), "BufferMemMgr");
    if ( mMgr.invalid() ) {
        LogError << "cannot get BufferMemMgr" << std::endl;
        return false;
    }
    mMgr->adopt(nav, "/Event");
    return true;
}

bool HaddAlg::finalize()
{
    return true;
}
