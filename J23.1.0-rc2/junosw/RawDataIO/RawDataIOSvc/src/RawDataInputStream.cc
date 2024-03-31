#include "RawDataInputStream.h"
#include "Event/RawHeader.h"
#include "RawFile/GCUReader.h"
#include "RawFile/RawEvtReader.h"
#include "EvtNavigator/EvtNavigator.h"
#include "SniperKernel/SniperLog.h"
#include "TTimeStamp.h"

RawDataInputStream::RawDataInputStream(const std::vector<std::string>& files, const std::string& type)
    : m_addr(nullptr),
      m_reader(nullptr),
      m_files(files),
      m_scope("RawDataInputStream"),
      m_type(type)
{
}

RawDataInputStream::~RawDataInputStream()
{
}

bool RawDataInputStream::initialize()
{
    bool stat = false;

    if ( m_type == "DAQ" ) {
        m_reader = new RawEvtReader(m_files);
        stat = m_reader->status();
    }
    else if ( m_type == "GCU" ) {
        m_reader = new GCUReader(m_files);
        stat = m_reader->status();
    }

    return stat;
}

bool RawDataInputStream::finalize()
{
    return true;
}

bool RawDataInputStream::next(int step, bool read)
{
    bool stat = true;

    if ( step > 1 ) {
        stat = m_reader->skipEvent(step-1);
    }

    if ( stat && read ) {
        stat = m_reader->readEvent(m_addr, m_readbyte);
    }

    return stat;
}

bool RawDataInputStream::first(bool read)
{
    bool stat = true;

    if ( read ) {
        stat = m_reader->readEvent(m_addr, m_readbyte);
    }

    return stat;
}

TObject* RawDataInputStream::get()
{
    if ( m_addr == nullptr ) {
        LogError << "no data valid" << std::endl;
        return nullptr;
    }

    JM::EvtNavigator* nav = new JM::EvtNavigator();
    JM::RawHeader* header = new JM::RawHeader();
    nav->addHeader(header);

    // write data into event according to the type of data (through Channel tag etc).
    if ( m_type == "DAQ" ) {
        int status = header->setRawEvt(m_readbyte, m_addr);
        uint64_t timestamp = header->getTimeStamp();
        time_t sec = timestamp/1000000000;
        Int_t nsec = timestamp%1000000000;
        TTimeStamp ts(sec, nsec);
        nav->setTimeStamp(ts);
        if ( status != 0 ) {
            LogError << "Bad event status: " << status
                     << " with marker: 0x" << std::hex << *((uint32_t*)m_addr) << std::dec
                     << std::endl;
            delete nav;
            nav = nullptr;
        }
    }
    else if ( m_type == "GCU" ) {
        JM::RawWaveform* wf = new JM::RawWaveform();
        wf->setChannelData(m_readbyte, m_addr);
        uint64_t timestamp = wf->getTimeStamp();
        time_t sec = timestamp/1000000000;
        Int_t nsec = timestamp%1000000000;
        TTimeStamp ts(sec, nsec);
        nav->setTimeStamp(ts);

        header->setRawWaveform(wf);
        header->setEventType("JM::RawWaveform");

        // move the ownership of the memory block to the header, or else the memory will be leaked
        header->setRawEvt(m_readbyte, m_addr);
    }
    m_addr = nullptr;
    m_readbyte = 0;

    return nav;
}
