#ifndef DATA_EXCHANGE_SVC_H
#define DATA_EXCHANGE_SVC_H

#include "SniperKernel/SvcBase.h"
#include "IADEvtBox.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/ExecUnit.h"

class DataExchangeSvc: public SvcBase{
public:
    DataExchangeSvc(const std::string& name);
    virtual ~DataExchangeSvc(){};

    virtual bool initialize();
    virtual bool finalize(){return true;};

    void setEvt(const IADEvtBox& evt);
    const IADEvtBox& getEvt();
    bool isEmpty(){return m_isEmpty;}
    bool filt();
private:
    IADEvtBox m_evt;
    bool m_isEmpty;
    Incident m_incident;
    ExecUnit* m_topTask;
};

#endif