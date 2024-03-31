#include "ElecEvtAnalysis.h"
#include "Event/CdWaveformHeader.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "TH1S.h"

DECLARE_ALGORITHM(ElecEvtAnalysis);

ElecEvtAnalysis::ElecEvtAnalysis(const std::string& name)
    : AlgBase(name)
{
    declProp("FillRoot", m_fillRoot = true);
    declProp("NumBins", m_nBins = 1000);
}

ElecEvtAnalysis::~ElecEvtAnalysis()
{
}

bool ElecEvtAnalysis::initialize()
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
        m_tree->Branch("ChNum", &m_chNum, "ChNum/i");
        m_tree->Branch("TriggerNumber", &m_triggerNumber, "TriggerNumber/i");
        m_tree->Branch("Time", &m_time, "Time/l");
        m_tree->Branch("PMTId", &m_PMTId, "PMTId[ChNum]/s");
        m_tree->Branch("GCUId", &m_GCUId, "GCUId[ChNum]/s");
        m_tree->Branch("ChSize", &m_chSize, "ChSize[ChNum]/s");
    }

    return true;
}

bool ElecEvtAnalysis::execute()
{
    ++m_count;

    // get the current event from buffer
    auto nav = m_buf->curEvt();
    auto hdr = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
    auto evt = dynamic_cast<JM::CdWaveformEvt*>(hdr->event());

    auto& chs = evt->channelData();

    m_triggerNumber = m_count;
    auto& time = nav->TimeStamp();
    m_time = time.GetSec()*1000000000 + time.GetNanoSec();

    m_chNum = 0;
    std::vector<UShort_t> _wfs;

    //loop channels
    for ( auto& ich : chs ) {
        int pmtId = ich.first;
        m_GCUId[m_chNum] = pmtId/10;
        m_PMTId[m_chNum] = pmtId%10;

        auto& adc = ich.second->adc();
        m_chSize[m_chNum] = adc.size();

        for ( auto i = 0; i < m_chSize[m_chNum]; ++i ) {
            _wfs.push_back(adc[i]);
        }

        ++m_chNum;
    }

    if ( m_fillRoot) {
        m_waveform->Reset();
        if ( _wfs.size() != m_nBins ) {
            m_nBins = _wfs.size();
            m_waveform->SetBins(m_nBins, 0, m_nBins);
        }
        for ( unsigned int i = 0; i < m_nBins; ++i ) {
            m_waveform->SetBinContent(i, _wfs[i]);
        }
        m_tree->Fill();
    }

    return true;
}

bool ElecEvtAnalysis::finalize()
{
    if ( m_fillRoot ) {
        delete m_waveform;
    }

    return true;
}
