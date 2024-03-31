#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SniperLog.h"
#include "IADSvc/DataExchangeSvc.h"
#include "IADSvc/IADEvtBox.h"
#include "IADSvc/IADInputStream.h"
#include "IADFormat/DummyData.h"

//This Filter alg, to filt IAD events. It is the alg of the subTask

class FilterAlg : public AlgBase
{
public:
    FilterAlg(const std::string &name);
    virtual ~FilterAlg();

    virtual bool initialize() override;
    virtual bool execute() override;
    virtual bool finalize() override;

    bool getNext();

private:
    std::string m_inputFile;
    IADInputStream* m_inputStream;

    int m_count = 1;
    DataExchangeSvc* m_swap;
    bool m_forward;
    IADEvtBox m_curBox;
};

#include "SniperKernel/AlgFactory.h"
DECLARE_ALGORITHM(FilterAlg);

FilterAlg::FilterAlg(const std::string &name)
    : AlgBase(name)
{   
    declProp("Forward", m_forward = true);
    declProp("Input", m_inputFile);

}

FilterAlg::~FilterAlg()
{
}

bool FilterAlg::initialize()
{   
    m_inputStream = new IADInputStream(m_inputFile);

    SniperPtr<DataExchangeSvc> svc(getParent(), "DataExchangeSvc");
    if (!svc.valid())
    {
        LogError << "failed to get service DataExchangeSvc" <<std::endl;
        return false;
    }
    m_swap = svc.data();
    return true;
}

bool FilterAlg::execute()
{   
    if(!m_inputStream->next()){
        //All the IAD events are processed, stop task
        m_par->getRoot()->stop();
    }
    auto box = m_inputStream->get();//obtain the evt to be processed
    auto evtHeader = box.evtHeader;
    auto evt = IAD::Block::getEvt<IAD::DummyData>(evtHeader);//obtain the physical information

    if(m_count % 10000 == 0){//Your criterial
        m_swap->setEvt(box);//forward the target event
    }
    m_count++;
    return true;
}

bool FilterAlg::getNext(){
    return true;
}

bool FilterAlg::finalize()
{   

    return true;
}



