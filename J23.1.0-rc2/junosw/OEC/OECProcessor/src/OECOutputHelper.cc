#include "OECOutputHelper.hh"

OECOutputHelper::OECOutputHelper(OECTagSvc* tagsvc)
    : m_tagsvc(tagsvc) {
        m_tqconvertor = new TQConvertor();
}

OECOutputHelper::~OECOutputHelper(){
    delete m_tqconvertor;
}

bool OECOutputHelper::copy(JM::EvtNavigator* src, JM::EvtNavigator* dst) {

    auto oecnav=src;
    auto outnav=dst;
    outnav->setTimeStamp(oecnav->TimeStamp());
    outnav->setRunID(oecnav->RunID());
    outnav->setEventID(oecnav->EventID());

    int thetag = 0;
    if (true){//Store the OevEvt
        JM::OecHeader* tempheader = JM::getHeaderObject<JM::OecHeader>(oecnav);
        JM::addHeaderObject(outnav, tempheader);
        //get the tag
        JM::OecEvt* tmpevt = dynamic_cast<JM::OecEvt*>(tempheader->event("JM::OecEvt"));
        thetag = tmpevt->getTag();
    }
    if (true){//Store the trigger info
        JM::CdTriggerHeader* header_cdtrigger = JM::getHeaderObject<JM::CdTriggerHeader>(oecnav);
        if (header_cdtrigger!=NULL) JM::addHeaderObject(outnav, header_cdtrigger);
        JM::WpTriggerHeader* header_wptrigger = JM::getHeaderObject<JM::WpTriggerHeader>(oecnav);
        if (header_wptrigger!=NULL) JM::addHeaderObject(outnav, header_wptrigger);
        JM::TtTriggerHeader* header_tttrigger = JM::getHeaderObject<JM::TtTriggerHeader>(oecnav);
        if (header_tttrigger!=NULL) JM::addHeaderObject(outnav, header_tttrigger);
    }
    if (true){//Store MC truth
        JM::GenHeader* header_gen = JM::getHeaderObject<JM::GenHeader>(oecnav);
        if (header_gen!=NULL) JM::addHeaderObject(outnav, header_gen);
        JM::SimHeader* header_sim = JM::getHeaderObject<JM::SimHeader>(oecnav);
        if (header_sim!=NULL) JM::addHeaderObject(outnav, header_sim);
    }
    if (true){//Store elec truth
        JM::CdLpmtElecTruthHeader* header_lpmttruth = JM::getHeaderObject<JM::CdLpmtElecTruthHeader>(oecnav);
        if (header_lpmttruth!=NULL) JM::addHeaderObject(outnav, header_lpmttruth);
        JM::CdSpmtElecTruthHeader* header_spmttruth = JM::getHeaderObject<JM::CdSpmtElecTruthHeader>(oecnav);
        if (header_spmttruth!=NULL) JM::addHeaderObject(outnav, header_spmttruth);
        JM::WpElecTruthHeader* header_wptruth = JM::getHeaderObject<JM::WpElecTruthHeader>(oecnav);
        if (header_wptruth!=NULL) JM::addHeaderObject(outnav, header_wptruth);
        JM::TtElecTruthHeader* header_tttruth = JM::getHeaderObject<JM::TtElecTruthHeader>(oecnav);
        if (header_tttruth!=NULL) JM::addHeaderObject(outnav, header_tttruth);
        JM::TrackElecTruthHeader* header_tracktruth = JM::getHeaderObject<JM::TrackElecTruthHeader>(oecnav);
        if (header_tracktruth!=NULL) JM::addHeaderObject(outnav, header_tracktruth);
    }
    if (m_tagsvc->getStoreFormat(thetag)){//Store the waveform
        //waveform
        JM::CdWaveformHeader* header_cdwaveform = JM::getHeaderObject<JM::CdWaveformHeader>(oecnav);
        if (header_cdwaveform!=NULL) JM::addHeaderObject(outnav, header_cdwaveform);
        JM::WpWaveformHeader* header_wpwaveform = JM::getHeaderObject<JM::WpWaveformHeader>(oecnav);
        if (header_wpwaveform!=NULL) JM::addHeaderObject(outnav, header_wpwaveform);
    }
    if (true){//Store the T/Q
        //T/Q
        JM::CdLpmtElecHeader* header_cdltq_FPGA = JM::getHeaderObject<JM::CdLpmtElecHeader>(oecnav);
        JM::CdLpmtElecHeader* header_cdltq_oec = NULL;
        JM::CdSpmtElecHeader* header_cdstq = JM::getHeaderObject<JM::CdSpmtElecHeader>(oecnav);
        JM::WpElecHeader* header_wptq = JM::getHeaderObject<JM::WpElecHeader>(oecnav);
        JM::TtElecHeader* header_tttq = JM::getHeaderObject<JM::TtElecHeader>(oecnav);
        //-----------------------CD large PMT TQ--------------------
        if (header_cdltq_oec==NULL){
            //Get the calibEvt for T/Q
            JM::CdLpmtCalibHeader* calib_cdltq = JM::getHeaderObject<JM::CdLpmtCalibHeader>(oecnav);
            //Convert it to elecheader
            header_cdltq_oec = m_tqconvertor->convertTQ(calib_cdltq, TQConvertor::WFREC);
        }
        //-----------------------CD small PMT TQ--------------------
        if (header_cdstq==NULL){
            //Get the calibEvt for T/Q
            JM::CdSpmtCalibHeader* calib_cdstq = JM::getHeaderObject<JM::CdSpmtCalibHeader>(oecnav);
            //Convert it to elecheader
            header_cdstq = m_tqconvertor->convertTQ(calib_cdstq, TQConvertor::WFREC);
        }
        //-----------------------WP PMT TQ--------------------
        if (header_wptq==NULL){//create the WpElecHeader, and fill T/Q into it
            //Get the calibEvt for T/Q
            JM::WpCalibHeader* calib_wptq = JM::getHeaderObject<JM::WpCalibHeader>(oecnav);
            header_wptq = m_tqconvertor->convertTQ(calib_wptq, TQConvertor::WFREC);
        }
        //-----------------------TT PMT TQ--------------------
        if (header_tttq==NULL){//create the TtElecHeader, and fill T/Q into it
            //Get the calibEvt for T/Q
            JM::TtCalibHeader* calib_tttq = JM::getHeaderObject<JM::TtCalibHeader>(oecnav);
            header_tttq = m_tqconvertor->convertTQ(calib_tttq, TQConvertor::WFREC);
        }
        if (header_cdltq_FPGA!=NULL) JM::addHeaderObject(outnav, header_cdltq_FPGA, "/Event/CdLpmtElec_FPGA");
        if (header_cdltq_oec!=NULL) JM::addHeaderObject(outnav, header_cdltq_oec);
        if (header_cdstq!=NULL) JM::addHeaderObject(outnav, header_cdstq);
        if (header_wptq!=NULL) JM::addHeaderObject(outnav, header_wptq);
        if (header_tttq!=NULL) JM::addHeaderObject(outnav, header_tttq);
    }

    return true;
}
