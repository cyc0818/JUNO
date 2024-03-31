#include "CDWaveFormSaving.h"

#include "SniperKernel/AlgFactory.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/SniperPtr.h"
#include "EvtNavigator/EvtNavHelper.h" //getHeaderObject
#include "SniperKernel/Task.h"

#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Identifier/CdID.h"

DECLARE_ALGORITHM(CDWaveFormSaving);

CDWaveFormSaving::CDWaveFormSaving(const std::string& name)
    : AlgBase(name),
      m_wf_nPEcut(0.0)
{
    m_wf_nPEcut = 0.0;
    
    // 0: disable algrothm;
    // not 0 (1.4e7 for example) to enable waveform saving: only save waveform for total nPE>1.4e7.
    declProp("WF_nPEcut", m_wf_nPEcut);
}

bool CDWaveFormSaving::initialize()
{
    LogDebug << "initializing" << std::endl;

    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }

    m_buf = navBuf.data();

    return true;
}

bool CDWaveFormSaving::execute()
{
    auto nav = m_buf->curEvt();
    if (!nav) {
        LogWarn << "can't load event navigator." << std::endl;
        return dynamic_cast<Task*>(getRoot())->stop();
    }

    auto eh = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
    if(eh==NULL) {
        LogWarn << "can't load CdWaveformHeader, CdWaveform won't be saved." << std::endl;
        return true;
    }
    //m_wf_nPEcut=0: disable algrothm;
    if(m_wf_nPEcut==0.0) {
        nav->setWriteFlag("/Event/CdWaveform", false);
        return true;
    }

    // Calibration/PMTCalibAlg/src/PMTCalibAlg.cc
    // read CalibHit data
    auto chcol = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav);
    const auto& chhlist = chcol->event()->calibPMTCol();
    auto chit = chhlist.begin();
    m_pe_sum = 0;
    while (chit != chhlist.end()) {
        auto calib = *chit++;
        //pmtId = calib->pmtId();
        Identifier id = Identifier(calib->pmtId());
        //unsigned int detID = CdID::id(static_cast<unsigned int>(pmtId));
        if (not CdID::is20inch(id)) continue;
        //time = calib->time();
        //charge = calib->charge();
        //double nPE = calib->nPE();
        //firstHitTime = calib->firstHitTime();
        m_pe_sum += calib->nPE();
    }

    LogDebug<<"set wf_nPEcut = "<<m_wf_nPEcut<<",  total PE = "<<m_pe_sum<<std::endl;
    //deciding to store Elecim waveform or not according to nPE
    //JM::addHeaderObject(nav, eh);
    if(m_pe_sum < m_wf_nPEcut) {
        nav->setWriteFlag("/Event/CdWaveform", false);
    }

    return true;
}

bool CDWaveFormSaving::finalize()
{
    LogDebug << "finalizing" << std::endl;
    return true;
}
