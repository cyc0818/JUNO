#include "FullStateNavBuf.h"
#include "RootIOSvc/IInputStream.h"

using JM::EvtNavigator;

FullStateNavBuf::FullStateNavBuf(double floor, double ceiling)
    : m_rEntry(-1),
      m_floor(floor),
      m_ceiling(ceiling),
      m_beyond(0),
      m_iStream(0)
{
}

FullStateNavBuf::~FullStateNavBuf()
{
}

void FullStateNavBuf::init(IInputStream* iStream)
{
    bool first = m_iStream == 0;

    m_iStream = iStream;

    bool stat = true;
    if ( first || m_rEntry <= 0 ) {
        stat = m_iStream->first(m_rEntry<=0);
    }
    if ( m_rEntry > 0 ) {
        stat = m_iStream->setEntry(m_rEntry);
        m_rEntry = -1;
    }

    if ( stat ) {
        m_beyond = dynamic_cast<JM::EvtNavigator*>(m_iStream->get());
        if ( m_beyond != 0 ) {
            fillNext();
        }
    }
}

bool FullStateNavBuf::next()
{
    if ( ++m_iCur >= m_dBuf.size() ) {
        if ( m_beyond == 0 ) {
            return false;
        }
        fillNext();
    }

    trimDated();
    fillFresh();

    return true;
}

bool FullStateNavBuf::adopt(std::shared_ptr<JM::EvtNavigator> &nav)
{
    m_dBuf.push_back(nav);
    m_iCur = m_dBuf.size() - 1;
    trimDated();

    return true;
}

bool FullStateNavBuf::reset(int entry)
{
    if ( m_iStream != 0 ) {
        clear();
        m_iCur = -1;
        init(m_iStream);
    }
    else {
        m_rEntry = entry;
    }
    return true;
}

void FullStateNavBuf::trimDated()
{
    JM::EvtNavigator* fevt = m_dBuf.front().get();
    while ( fevt != m_dBuf[m_iCur].get() ) {
        if ( m_floor < 0.0 && timeShift(fevt) >= m_floor ) {
            break;
        }
        m_dBuf.pop_front();
        --m_iCur;
        fevt = m_dBuf.front().get();
    }
}

void FullStateNavBuf::fillFresh()
{
    if ( m_ceiling > 0.0 ) {
        while ( m_beyond != 0 && timeShift(m_beyond) < m_ceiling ) {
            fillNext();
        }
    }
}

void FullStateNavBuf::fillNext()
{
    //we have to ensure m_beyond is valid before call this function
    m_dBuf.push_back(ElementPtr(m_beyond));
    m_beyond = dynamic_cast<JM::EvtNavigator*>(m_iStream->next() ? m_iStream->get() : 0);
}

double FullStateNavBuf::timeShift(EvtNavigator* nav)
{
    const TTimeStamp& t0 = m_dBuf[m_iCur]->TimeStamp();

    TTimeStamp t = nav->TimeStamp();
    t.Add(TTimeStamp(-t0.GetSec(), -t0.GetNanoSec()));
    return t.AsDouble();
}
