#include "AnalyNavBuffer.h"
#include "RootIOSvc/IInputStream.h"
#include "IADFormat/IndexDict.h"
#include <climits>
#include "RootIOSvc/NavInputStream.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"
#include "TROOT.h"
#include <mutex>

static std::mutex s_streamM;

AnalyNavBuffer::AnalyNavBuffer(double floor, double ceiling) : m_floor(floor), m_ceiling(ceiling), m_fileNum(UINT_MAX), m_iStream(nullptr)
{
    ROOT::EnableThreadSafety();
}

AnalyNavBuffer::~AnalyNavBuffer() {}

// Fix me: Reading all the IAD files' metaBlock, then get all the EDM paths, use them to genrate stream
// There should be a conversion between local entry and global entry in the stream
bool AnalyNavBuffer::loadWindow(const IADEvtBox &iadEvt)
{
    std::unique_lock<std::mutex> lock(s_streamM);
    // TTimeStamp t = TTimeStamp((time_t)iadEvt.evtHeader->sec, (Int_t)iadEvt.evtHeader->nanoSec);
    // LogInfo<<"new cur time    "<<t<<" entry"<<iadEvt.evtHeader->entry<<std::endl;

    if (iadEvt.fileNumber != m_fileNum)
    { // a new EDM file is used
        m_fileNum = iadEvt.fileNumber;
        m_esdFile = IAD::IndexDict::instance()->getPath(iadEvt.evtHeader->indexNum);
        if (m_iStream)
            delete m_iStream;

        m_iStream = new NavInputStream({m_esdFile});
        m_iStream->initialize();
        bool stat = m_iStream->first(); // otherwise the stream can not work properly
        if (!stat)
        {
            throw SniperException("Failed to refrer to ESD data");
        }

        this->clear();
    }

    uint32_t _targetEntry = iadEvt.evtHeader->entry;
    if (isOverlap(iadEvt))
    { // Windows overlap, just read the stream, and set the curEvt properly
        if (TTimeStamp((time_t)iadEvt.evtHeader->sec, (Int_t)iadEvt.evtHeader->nanoSec) > m_dBuf.back()->TimeStamp())
        {
            // When the IAD evt is out of current window, we should firt read it into the navBuffer
            uint32_t sashEntry = m_dBuf.size() - 1 - m_iCur + m_curEntry;
            while (sashEntry < _targetEntry)
            {
                fillNext();
                sashEntry++;
            }
        }

        // The associated evt is in the current window, just set m_iCur
        m_iCur += _targetEntry - m_curEntry;
        m_curEntry = _targetEntry;
    }
    else
    { // No overlap, load left half window, then set the Cur
        this->clear();

        if (!m_iStream->setEntry(_targetEntry))
            return false;

        auto curEvt = ElementPtr(m_iStream->get());
        m_dBuf.push_back(curEvt);
        m_iCur = 0;
        m_curEntry = m_iStream->getEntry();

        while (m_iStream->getEntry() > 0)
        {
            m_iStream->prev();
            auto evt = ElementPtr(m_iStream->get());

            if (m_floor < 0 && timeShift(evt.get()) >= m_floor)
            {
                m_dBuf.push_front(evt);
                m_iCur++;
            }
            else
                break;
        }
    }
    int sashEntry = m_dBuf.size() - 1 - m_iCur + m_curEntry;
    m_iStream->setEntry(sashEntry, false);
    m_beyond = dynamic_cast<JM::EvtNavigator *>(m_iStream->next() ? m_iStream->get() : 0);

    trimDated();
    fillFresh();

    LogInfo << "Window prepared, events in window: " << m_dBuf.size() << std::endl;

    return true;
}

bool AnalyNavBuffer::isOverlap(const IADEvtBox &iadEvt)
{
    if (m_dBuf.empty())
        return false;

    const TTimeStamp &rightSash = m_dBuf.back()->TimeStamp();
    TTimeStamp t = TTimeStamp((time_t)iadEvt.evtHeader->sec, (Int_t)iadEvt.evtHeader->nanoSec);

    t.Add(TTimeStamp(-rightSash.GetSec(), -rightSash.GetNanoSec()));

    // LogInfo<<"elapse "<<t.AsDouble()<<std::endl;
    if (t.AsDouble() > -m_floor)
    {
        return false;
    }
    else
        return true;
}

void AnalyNavBuffer::trimDated()
{
    JM::EvtNavigator *fevt = m_dBuf.front().get();
    while (fevt != m_dBuf[m_iCur].get())
    {
        if (m_floor < 0.0 && timeShift(fevt) >= m_floor)
        {
            break;
        }
        m_dBuf.pop_front();
        --m_iCur;
        fevt = m_dBuf.front().get();
    }
}

void AnalyNavBuffer::fillFresh()
{
    if (m_ceiling > 0.0)
    {
        while (m_beyond != 0 && timeShift(m_beyond) < m_ceiling)
        {
            fillNext();
        }
    }
}

void AnalyNavBuffer::fillNext()
{
    // we have to ensure m_beyond is valid before call this function
    m_dBuf.push_back(ElementPtr(m_beyond));
    m_beyond = dynamic_cast<JM::EvtNavigator *>(m_iStream->next() ? m_iStream->get() : 0);
}

double AnalyNavBuffer::timeShift(JM::EvtNavigator *nav)
{
    const TTimeStamp &t0 = m_dBuf[m_iCur]->TimeStamp();

    TTimeStamp t = nav->TimeStamp();
    t.Add(TTimeStamp(-t0.GetSec(), -t0.GetNanoSec()));
    return t.AsDouble();
}