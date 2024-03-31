#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SniperLog.h"
#include "IADUtil/WorkerInputStream.h"
#include "IADSvc/DataExchangeSvc.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"

//This is the filter of multithreading two-step data analysis
//It get IAD event from globalbuffer. If the event satisfy the criterial, it will be forwarded to the to top task by the criteria

class WorkerFilterAlg : public AlgBase
{
public:
    WorkerFilterAlg(const std::string &name);
    virtual ~WorkerFilterAlg();

    virtual bool initialize() override;
    virtual bool execute() override;
    virtual bool finalize() override;

private:
    WorkerInputStream* m_inputStream;
    DataExchangeSvc* m_swap;
    uint32_t m_count;
};

#include "SniperKernel/AlgFactory.h"
DECLARE_ALGORITHM(WorkerFilterAlg);

WorkerFilterAlg::WorkerFilterAlg(const std::string &name)
    : AlgBase(name)
{   
}

WorkerFilterAlg::~WorkerFilterAlg()
{
}

bool WorkerFilterAlg::initialize()
{   
    m_inputStream = new WorkerInputStream();
    m_inputStream->initialize();

    SniperPtr<DataExchangeSvc> svc(getParent(), "DataExchangeSvc");
    if (!svc.valid())
    {
        LogError << "failed to get service DataExchangeSvc" <<std::endl;
        return false;
    }
    m_swap = svc.data();

    return true;
}

bool WorkerFilterAlg::execute()
{   
    if(!m_inputStream->next()){
        //All the IAD events are processed, stop task
        m_par->getRoot()->stop();
    }

    auto box = m_inputStream->get();
    
    if(m_count % 10000 == 0){//Your criterial
        m_swap->setEvt(box);//forward the target event
    }
    m_count++;

    return true;
}

bool WorkerFilterAlg::finalize()
{   
    return true;
}