#include "RawEvtBuilder.h"
#include "Event/RawHeader.h"
#include "Event/CdWaveformHeader.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/Task.h"

DECLARE_ALGORITHM(RawEvtBuilder);

RawEvtBuilder::RawEvtBuilder(const std::string& name)
    : AlgBase(name),
      m_ndone(0),
      m_count(0)
{
    declProp("NumGCUs", m_ngcus = 0);
}

RawEvtBuilder::~RawEvtBuilder()
{
}

bool RawEvtBuilder::initialize()
{
    SniperPtr<IDataMemMgr> mmgr(m_par, "BufferMemMgr");
    if ( mmgr.invalid() ) {
        LogError << "failed to get BufferMemMgr" << std::endl;
        return false;
    }
    m_mmgr = mmgr.data();

    return true;
}

bool RawEvtBuilder::execute()
{
    if (m_count == 0) {
        // initialize for the first time of execute
        char itask[8];
        for ( unsigned int i = 1; i <= m_ngcus; ++i ) {
            sprintf(itask, "gcu%02d", i);
            SniperDataPtr<JM::NavBuffer> ibuf(m_par, std::string(itask) + ":/Event");
            if (ibuf.invalid()) {
                LogError << "failed to get NavBuffer of " << itask << std::endl;
                return false;
            }
            m_ibuf.push_back(ibuf.data());
            m_iname.push_back(itask);
            Incident::fire(*m_par, itask);
        }
    }

    ++m_count;

    ULong_t time = 0xFFFFFFFFFFFFFFFF;
    std::vector<const JM::WaveformChannel*> wfs;
    std::vector<int> chnums;

    //loop all input streams
    for ( unsigned int i = 0; i < m_ngcus; ++i ) {
        if( m_ibuf[i] == nullptr ) {
            continue;
        }
        auto nav = m_ibuf[i]->curEvt();
        auto hdr = JM::getHeaderObject<JM::RawHeader>(nav);
        auto wf = dynamic_cast<JM::RawWaveform*>(hdr->rawWaveform());
        auto& chs = wf->getChannels();
        auto& ch = chs[0];  //only 1 channel in GCU data

        // find the event with minimized time
        ULong_t itime = ch.getTime();
        if ( itime < time ) {
            wfs.clear();
            chnums.clear();
            time = itime;
        }
        if ( itime == time ) {
            wfs.push_back(&ch);
            chnums.push_back(i);
        }
    }

    //the event built 
    if ( chnums.size() == m_ngcus ) {
        LogDebug << "event " << m_count << " channels " << chnums.size() << std::endl;
    }
    else {
        LogWarn << "event " << m_count << " channels " << chnums.size() << std::endl;
    }
    auto nav = new JM::EvtNavigator();
    ULong_t _time = time*8;
    nav->setTimeStamp(TTimeStamp(int(_time/1000000000), int(_time%1000000000)));
    m_mmgr->adopt(nav, "/Event");
    auto eh = new JM::CdWaveformHeader;
    nav->addHeader(eh);
    auto ee = new JM::CdWaveformEvt;
    eh->setEvent(ee);

    for ( unsigned int i = 0; i < chnums.size(); ++i ) {
        auto ch = wfs[i];
        //std::cout << chnums[i] << ": " << ch->getTriggerNumber() << "  " << ch->getTime()*8 << "  " << ch->getPMTId() << "  " << ch->getGCUId() << std::endl;
        int pmtId = ch->getGCUId()*10 + ch->getPMTId();  //FIXME: should be consistent with the hardware identifier
        auto _wfSize = ch->waveformSize();
        auto _wfData = ch->waveformData();
        auto ech = ee->addChannel(pmtId);
        auto& adc = ech->adc();
        for ( unsigned int j = 0; j < _wfSize; ++j ) {
            adc.push_back(_wfData[j]);
        }
    }

    //prepare for next event
    for (auto ich : chnums) {
        try {
            Incident::fire(*m_par, m_iname[ich]);
        }
        catch (StopRunProcess& e) {
            LogInfo << "reach end of " << m_iname[ich] << std::endl;
            m_ibuf[ich] = nullptr;
            ++m_ndone;
        }
    }

    //reach the end of all input streams, stop the process
    if ( m_ndone == m_ngcus ) {
        dynamic_cast<Task*>(m_par)->stop();
    }

    return true;
}

bool RawEvtBuilder::finalize()
{
    return true;
}

