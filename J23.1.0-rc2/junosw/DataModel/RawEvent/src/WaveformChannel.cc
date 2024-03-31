#include "Event/WaveformChannel.h"

namespace JM
{
    WaveformChannel::WaveformChannel(UShort_t* data)
        : m_data(data)
    {
        m_size = data[2] * 8;  //num of UShort_t
    }

    WaveformChannel::~WaveformChannel()
    {
    }

    //waveform
    UShort_t* WaveformChannel::waveformData() const
    {
        return m_data+8;
    }

    UInt_t    WaveformChannel::waveformSize() const
    {
        return m_size-16;
    }

    //header
    UShort_t  WaveformChannel::getPMTId() const
    {
        return m_data[1];
    }

    UShort_t  WaveformChannel::getTriggerNumber() const
    {
        return m_data[3];
    }

    UShort_t  WaveformChannel::getTriggerMode() const
    {
        return m_data[4];
    }

    ULong_t   WaveformChannel::getTime() const
    {
        UShort_t* ptime = m_data+5;
        ULong_t timestamp = ptime[0];
        for (int j = 1; j < 3; j++) {
            timestamp <<= 16;
            timestamp |= ptime[j];
        }
        return timestamp << 3;
    }

    //trailer

    UShort_t  WaveformChannel::getVersion() const
    {
        return m_data[m_size-8];
    }

    UShort_t  WaveformChannel::getDataType() const
    {
        return m_data[m_size-7];
    }

    UShort_t  WaveformChannel::getErrorRegister() const
    {
        return m_data[m_size-6];
    }

    UShort_t  WaveformChannel::getHighVoltageEnable() const
    {
        return m_data[m_size-5];
    }

    UShort_t  WaveformChannel::getGCUId() const
    {
        return m_data[m_size-2];
    }

}
