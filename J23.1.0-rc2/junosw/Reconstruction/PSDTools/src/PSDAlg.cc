#include "PSDTools/PSDAlg.h"

#include "SniperKernel/ToolBase.h"
#include "SniperKernel/AlgFactory.h"
#include "RootWriter/RootWriter.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/CdVertexRecHeader.h"
#include "Geometry/PMTParamSvc.h"
#include "PSDTools/IPSDInputSvc.h"


DECLARE_ALGORITHM(PSDAlg);

PSDAlg::PSDAlg(const std::string &name):AlgBase(name){
    i_ithEvt = -1;
    m_psdTool = nullptr;

    d_psdVal = -1;
    d_recE = 0;
    d_recX = 0;d_recY = 0;d_recZ = 0;

    declProp("Method", s_psdMethod = "");
    declProp("UsePredict", b_usePredict = true);
}

PSDAlg::~PSDAlg()= default;

bool PSDAlg::initialize(){
    //==============Get the event buffer==============
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();


    //==============Initialize the PSDTools method==============
    IPSDInputSvc* psdInputSvc = dynamic_cast<IPSDInputSvc*>(getParent()->find("PSDInputSvc"));
    psdInputSvc->Initialize(dynamic_cast<PMTParamSvc *>(getParent()->find("PMTParamSvc")));

    m_psdTool=tool<IPSDTool>(s_psdMethod);
    if (!m_psdTool){
        LogError<<"The PSDTools tool is not found!"<<std::endl;
        return false;
    }
    if (b_usePredict) m_psdTool->enablePredict();
    else m_psdTool->disablePredict();

    if (!m_psdTool->initialize()){
        LogError<<"Fail to initialize Tool:"<<s_psdMethod<<"!"<<std::endl;
        return false;
    }

    //==============initialize the output file===================
    SniperPtr<RootWriter> rwsvc(getParent(),"RootWriter");
    if ( rwsvc.invalid() ) {
        LogError << "cannot get the rootwriter service." << std::endl;
        return false;
    }
    m_outTree=rwsvc->bookTree(*m_par,"PSDUser", "PSDUser");
    m_outTree->Branch("evtID", &i_ithEvt, "evtID/I");
    m_outTree->Branch("recE", &d_recE, "recE/D");
    m_outTree->Branch("recX", &d_recX, "recX/D");
    m_outTree->Branch("recY", &d_recY, "recY/D");
    m_outTree->Branch("recZ", &d_recZ, "recZ/D");
    rwsvc->attach("USER_OUTPUT", m_outTree);
    return true;
}

bool PSDAlg::finalize(){
    if (!m_psdTool->finalize()){
        LogError<<"Fail to finalize Tool:"<<s_psdMethod<<"!"<<std::endl;
        return false;
    }
    return true;
}

bool PSDAlg::execute(){

    //===============get the current event=================
    auto nav=m_buf->curEvt();

    if (not nav){
		LogError << "Failed to find the navigator. " << std::endl;
		return false;
    }

    i_ithEvt = nav->EventID();
    LogInfo <<" trigger Event ID = " << i_ithEvt<<std::endl;
    m_psdTool->SetEvtID(i_ithEvt);

    LogDebug << "Finished Get Navigator and start to preProcess Data"<<std::endl;
    //=====================PSDTools procedure=====================
    if (!m_psdTool->preProcess(nav))
    {
        LogError<<"Error when pre-processing the "<<i_ithEvt<<"th event! Set PSD Value as -50 to tag error state!"<<std::endl;
        if (b_usePredict) d_psdVal = m_psdTool->CalPSDVal(true);

//        return false;
    }
    else {
        LogDebug << "Finished preProcess() and start to CalPSDVal()" << std::endl;
        if (b_usePredict) d_psdVal = m_psdTool->CalPSDVal(false);

        // ==============Get current rec event ==========================================
        auto recheader = JM::getHeaderObject<JM::CdVertexRecHeader>(nav);
        auto thisRecEvent = recheader->event();

        //===============get the reconstruction information=================
        auto thisRecVertex = thisRecEvent->getVertex(0); // get the first vertex
        d_recE = thisRecVertex->energy();
        d_recX = thisRecVertex->x();
        d_recY = thisRecVertex->y();
        d_recZ = thisRecVertex->z();
        LogInfo << "Energy:" << d_recE << "MeV" << std::endl;
        LogInfo << "Vertex: (" << d_recX << ", " << d_recY << ", " << d_recZ << ") mm." << std::endl;
    }
    m_outTree->Fill();

    return true;
}
