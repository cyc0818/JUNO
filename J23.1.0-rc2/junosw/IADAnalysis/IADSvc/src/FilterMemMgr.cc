#include "FilterMemMgr.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include "BeginEvtHdl.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/DataMemSvc.h"
#include "AnalyNavBuffer.h"
#include "TROOT.h"

DECLARE_SERVICE(FilterMemMgr);

FilterMemMgr::FilterMemMgr(const std::string& name): SvcBase(name){
    ROOT::EnableThreadSafety();
    declProp("TimeWindow", m_bufBounds);
    declProp("StopTaskOpt", m_stoptask_option=0);
}

FilterMemMgr::~FilterMemMgr(){}

bool FilterMemMgr::initialize(){

    Task* par = dynamic_cast<Task*>(getParent());

    if ( m_bufBounds.size() < 2 ) {
        m_bufBounds.resize(2);
        m_bufBounds[0] = 0.0, m_bufBounds[1] = 0.0;
    }
    SniperPtr<DataMemSvc> mSvc(*par, "DataMemSvc");
    mSvc->regist("/Event",
            new AnalyNavBuffer(m_bufBounds[0], m_bufBounds[1]));

    //regist beginEvtHdl
    IIncidentHandler* bi = new BeginEvtHdl(par, m_stoptask_option);
    bi->regist("BeginEvent");
    m_icdts.push_back(bi);

    return true;
}

bool FilterMemMgr::finalize(){
    return true;
}


