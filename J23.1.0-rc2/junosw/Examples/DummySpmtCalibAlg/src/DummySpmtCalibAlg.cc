#include "DummySpmtCalibAlg.hh"

DECLARE_ALGORITHM(DummySpmtCalibAlg);

DummySpmtCalibAlg::DummySpmtCalibAlg(const std::string& name)
    : AlgBase(name) {

}

DummySpmtCalibAlg::~DummySpmtCalibAlg() {

}

bool DummySpmtCalibAlg::initialize() {
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    return true;
}

bool DummySpmtCalibAlg::execute() {

    JM::EvtNavigator* nav = m_buf->curEvt(); 

    if (!nav) {
        LogError << "Failed to get event navigator. " << std::endl;
        return false;
    }

    LogInfo << "Nav pointer: " << nav << std::endl;

    JM::ElecHeader* eh = dynamic_cast<JM::ElecHeader*>(nav->getHeader("/Event/Elec"));
    if (not eh) {
        LogError << "Failed to get Elec Header. " << std::endl;
        return false;
    }

    JM::SpmtElecEvent* ee  = dynamic_cast<JM::SpmtElecEvent*>(eh->spmtEvent());
    if (!eh->hasSpmtEvent()) {
        LogInfo << "SpmtElecEvent is not available in this event. " << std::endl;
        return true;
    }

    JM::CalibHeader* calibheader = dynamic_cast<JM::CalibHeader*>(nav->getHeader("/Event/Calib")); 
    if (!calibheader) {
        LogError << "Failed to get CalibHeader. " << std::endl;
        // return true;
        calibheader = new JM::CalibHeader();
        nav->addHeader("/Event/Calib", calibheader);
    }

    JM::CalibEvent* calibevent = dynamic_cast<JM::CalibEvent*>(calibheader->event());
    if (!calibevent) {
        LogError << "Failed to get CalibEvent. " << std::endl;
        // return true;
        calibevent = new JM::CalibEvent();
        calibheader->setEvent(calibevent);
    }

    LogInfo << "Adding dummy channels..." << std::endl;
    for (int pmtid = 300000; pmtid < 300010; ++pmtid) {
        JM::CalibPMTChannel* cpc = calibevent->addCalibPmtChannel(pmtid); 

        // channel
        double npe = 2;
        double firstHitTime = 100.;
        cpc->setNPE(npe);
        cpc->setFirstHitTime(firstHitTime);
        // hit
        int nhit = 2;
        std::vector<double> vec_npe;
        std::vector<double> vec_time;
        for (int ihit = 0; ihit < nhit; ++ihit) {
            vec_npe.push_back(1.0);
            vec_time.push_back(firstHitTime+ihit*100);
        }
        cpc->setCharge(vec_npe);
        cpc->setTime(vec_time);
    }

    return true;
}

bool DummySpmtCalibAlg::finalize() {

    return true;
}
