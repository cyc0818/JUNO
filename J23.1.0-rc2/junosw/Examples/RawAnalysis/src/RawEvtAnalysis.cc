#include "RawEvtAnalysis.h"
#include "Event/RawHeader.h"
#include "Event/CdWaveformHeader.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "TH1S.h"

DECLARE_ALGORITHM(RawEvtAnalysis);

RawEvtAnalysis::RawEvtAnalysis(const std::string& name)
    : AlgBase(name)
{
    declProp("FillRoot", m_fillRoot = false);
    declProp("FillElec", m_fillElec = false);
    declProp("NumBins", m_nBins = 1000);
}

RawEvtAnalysis::~RawEvtAnalysis()
{
}

bool RawEvtAnalysis::initialize()
{
    //get the buffer.
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if (navBuf.invalid()) {
        return false;
    }
    m_buf = navBuf.data();

    if ( m_fillRoot ) {
        //get the RootWriter and book the Tree
        SniperPtr<RootWriter> rw(this->getParent(), "RootWriter");
        if ( ! rw.valid() ) {
            LogError << "Failed to get RootWriter instance!" << std::endl;
            return false;
        }

        m_waveform = new TH1S("waveform", "waveform", m_nBins, 0, m_nBins);

        m_tree = rw->bookTree(*m_par, "waveform/data", "Integration test data for JUNO");
        m_tree->Branch("waveform", "TH1S", &m_waveform, 32000, 0);
        m_tree->Branch("PMTId", &m_PMTId, "PMTId/s");
        m_tree->Branch("TriggerNumber", &m_triggerNumber, "TriggerNumber/s");
        m_tree->Branch("TriggerMode", &m_triggerMode, "TriggerMode/s");
        m_tree->Branch("Time", &m_time, "Time/l");
        m_tree->Branch("Version", &m_version, "Version/s");
        m_tree->Branch("DataType", &m_dataType, "DataType/s");
        m_tree->Branch("ErrorRegister", &m_errorRegister, "ErrorRegister/s");
        m_tree->Branch("HighVoltageEnable", &m_highVoltageEnable, "HighVoltageEnable/s");
        m_tree->Branch("GCUId", &m_GCUId, "GCUId/s");
    }

    return true;
}

bool RawEvtAnalysis::execute()
{
    ++m_count;

    // get the current event from buffer
    auto nav = m_buf->curEvt();
    // get the RawHeader from current event
    auto hdr = JM::getHeaderObject<JM::RawHeader>(nav);
    // get the RawWaveform from RawHeader
    auto wf = dynamic_cast<JM::RawWaveform*>(hdr->rawWaveform());

    auto& chs = wf->getChannels();

    std::map<int,JM::ElecWaveform*>* echs = nullptr;
    if ( m_fillElec ) {
        auto eh = new JM::CdWaveformHeader;
        nav->addHeader(eh);
        auto ee = new JM::CdWaveformEvt;
        eh->setEvent(ee);

        echs = const_cast<std::map<int,JM::ElecWaveform*>*>(&ee->channelData());
    }

    for ( auto& ch : chs ) {
        m_PMTId = ch.getPMTId();
        m_triggerNumber = ch.getTriggerNumber();
        m_triggerMode = ch.getTriggerMode();
        m_time = ch.getTime();
        m_version = ch.getVersion();
        m_dataType = ch.getDataType();
        m_errorRegister = ch.getErrorRegister();
        m_highVoltageEnable = ch.getHighVoltageEnable();
        m_GCUId = ch.getGCUId();
        auto _wfSize = ch.waveformSize();
        auto _wfData = ch.waveformData();

        if ( m_fillRoot) {
            m_waveform->Reset();
            if ( _wfSize != m_nBins ) {
                m_nBins = _wfSize;
                m_waveform->SetBins(m_nBins, 0, m_nBins);
            }
            for ( unsigned int i = 0; i < m_nBins; ++i ) {
                m_waveform->SetBinContent(i, _wfData[i]);
            }
            m_tree->Fill();
        }

        if ( m_fillElec ) {
            int pmtId = m_GCUId*10 + m_PMTId;  //use Identifier in the future
            auto ech = new JM::ElecWaveform();
            echs->insert(std::make_pair(pmtId, ech));
            auto& adc = ech->adc();
            adc.reserve(_wfSize);
            for ( unsigned int i = 0; i < _wfSize; ++i ) {
                adc.push_back(_wfData[i]);
            }
        }
    }

    return true;
}

bool RawEvtAnalysis::finalize()
{
    if ( m_fillRoot ) {
        delete m_waveform;
    }

    return true;
}
