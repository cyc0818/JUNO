#include "RootIOTools/MergingInputSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "RootIOSvc/RootInputSvc.h"
#include "SimEventMerger.h"


DECLARE_SERVICE(MergingInputSvc);

MergingInputSvc::MergingInputSvc(const std::string& name)
    : SvcBase(name),
      m_inputSvc(0),
      m_merger(0)
{
    declProp("InputFile", m_navInputFile);
    declProp("MergingEventType", m_mergingType="SimEvent");
    declProp("MergingEventPath", m_path="/Event/Sim");
}

MergingInputSvc::~MergingInputSvc()
{
}

bool MergingInputSvc::initialize()
{
    LogInfo << "Initialize MergingInputSvc" << std::endl;    

    m_inputSvc = new RootInputSvc("ConcreteInputSvc");
    m_inputSvc->m_navInputFile = m_navInputFile;
    bool okay = m_inputSvc->initialize();
    if (!okay) return false;

    if (m_mergingType == "SimEvent") {
        m_merger = new SimEventMerger(m_inputSvc->getInputStream("EvtNavigator"), m_path);
    }
    else {
        LogError << "Unknown merging event type " << m_mergingType << std::endl;
        return false;
    }
    return true;
}

bool MergingInputSvc::finalize()
{
    LogInfo << "Finalizing MergingInputSvc" << std::endl;

    if (m_inputSvc) {
        bool okay = m_inputSvc->finalize();
        if (!okay) return false;
        delete m_inputSvc;
    }
    if (m_merger) delete m_merger;
    return true;
}

IInputStream* MergingInputSvc::getInputStream(const std::string& path UNUSED)
{
    return m_merger;
}
