#include "Event/RawWaveform.h"
#include <iostream>

ClassImp(JM::RawWaveform)

namespace JM
{
    RawWaveform::RawWaveform()
        : m_size(0),
          m_data(nullptr)
    {
    }

    RawWaveform::~RawWaveform()
    {
    }

    void RawWaveform::setData(UInt_t size, Char_t* data)
    {
        m_size = size;
        m_data = data;
    }

    // set GCU channel data
    void RawWaveform::setChannelData(UInt_t size, Char_t* data)
    {
        m_size = size;
        m_data = data;
        m_chPos.push_back(0);
    }

    // get GCU timestamp
    uint64_t RawWaveform::getTimeStamp() {
        if (m_data == nullptr) {
            throw std::logic_error("wrong channel data address in RawWaveform::getTimeStamp()");
        }
        UShort_t* _gcuHdr = (UShort_t*)m_data;
        uint64_t timestamp = _gcuHdr[5];
        for (int i = 6; i < 8; ++i) {
            timestamp <<= 16;
            timestamp |= _gcuHdr[i];
        }
        timestamp = timestamp << 3;         // Timestamp = timestamp * 8 ns
        return timestamp;
    }

    const std::vector<WaveformChannel>& RawWaveform::getChannels()
    {
        if ( m_chPos.empty() ) {
            UInt_t curPos = 0;                              // in bytes
            UShort_t* dptr = (UShort_t*)m_data;

            /* dptr[curPos/2]   --- ChannelID
               dptr[curPos/2+1] --- high 8 bits: data type, low 8 bits: detector type */
            while ( curPos < m_size ) {
                if (dptr[curPos/2+1]>>8 != 0x10) {          // check the data type which should be waveform
                    throw std::logic_error("wrong channel tag in RawWaveform::getChannels()");
                }
                UInt_t GCUId = dptr[curPos/2]/3;            // GCUId = ChannelID/3
                curPos += 4;                                // skip the channel tag
                m_chPos.push_back(curPos);                  // m_chPos: start position of each waveform_header
                UInt_t tmp = curPos/2+2;                    // index of wf_length in wf_header for UShort_t*
                curPos += dptr[tmp]*8*2;
                tmp = curPos/2-2;                           // index of GCU_BEC_port in wf_trailer for UShort_t*
                dptr[tmp] = GCUId;                          // override GCU_BEC_port with GCUId
            }
        }

        if ( m_chs.empty() ) {
            m_chs.reserve(m_chPos.size());
            for ( auto pos : m_chPos ) {
                UShort_t* dptr = (UShort_t*)(m_data+pos);   // points to the start position of the next wf_fragment
                m_chs.emplace_back(dptr);                   // handle the wf_fragment
            }
        }

        return m_chs;
    }
}
