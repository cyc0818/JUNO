#include "Event/RawHeader.h"
#include <stdexcept>
#include <iostream>

ClassImp(JM::RawHeader)

namespace JM
{
    RawHeader::RawHeader()
        : m_addr(nullptr),
          m_evtTag_type(0xFFFFFFFF)
    {
    }

    RawHeader::~RawHeader() {
        delete[] m_addr;
    }

    // - set Event_Fragment:
    int RawHeader::setRawEvt(uint32_t size, char* addr) {
        m_addr = addr;

        UInt_t* _evtHdr = (UInt_t*)m_addr;
        if ( _evtHdr[0] != 0xee1234ee ) {
            return -9;
        }

        UInt_t _leftSize = size - _evtHdr[1];                   // _evtHdr[1] is the size of Event_Header_Record
        UInt_t _shift = _evtHdr[1];                             // skip the Event_Header_Record & points to the first Channel_tag

        // - check the Event_tag:
        m_evtTag_type = _evtHdr[6] >> 16;

        if ( m_evtTag_type == 0x0011 ) {                        // only CD-LPMT now, other types in future
            while( _leftSize != 0 ) {
                UShort_t* _chTag = (UShort_t*)(m_addr+_shift);  // points to current Channel_tag
                /* _chTag[0] --- ChannelID
                   _chTag[1] --- high 8 bits: data type, low 8 bits: detector type */

                UShort_t dataType = _chTag[1] >> 8;
                if ( dataType == 0x10 ) {                       // data type: waveform
                    UInt_t dataSize = _evtHdr[14];              // waveform size in bytes, including the Channel_tags

                    JM::RawWaveform* wf = new JM::RawWaveform();
                    wf->setData(dataSize, m_addr+_shift);
                    setRawWaveform(wf);
                    setEventType("JM::RawWaveform");

                    // - setting
                    _leftSize -= dataSize;
                    _shift += dataSize;                         // jump to next Channel_tag
                }
                else if ( dataType == 0x31 ) {                  // data type: OEC result
                    UInt_t dataSize = _evtHdr[9];               // size in bytes, including the Channel_tags

                    //TODO: decode it in the future when the details are fixed

                    // - setting
                    _leftSize -= dataSize;
                    _shift += dataSize;                         // jump to next Channel_tag
                }
                else if ( dataType == 0x30 ) {                  // data type: Waveform TQ
                    UInt_t dataSize = _evtHdr[15];              // size in bytes, including the Channel_tags

                    //TODO: decode it in the future when the details are fixed

                    // - setting
                    _leftSize -= dataSize;
                    _shift += dataSize;                         // jump to next Channel_tag
                }
                else {
                    // std::cerr << "RawHeader::channelHandler() failed to encode Channel_tag: unknown data type: "
                    //     << std::hex << _chTag[1] << std::dec << std::endl;
                    return -1;
                }
            }
        }
        else {
              // std::cerr << "RawHeader::setRawEvent() wrong Event_tag_type: "
              //     << std::hex << m_evtTag_type << std::dec << std::endl;
              // std::cerr << "RawHeader::setRawEvent() failed to set the raw event, current Event_ID:"
              //     << std::hex << _evtHdr[5] << std::dec << std::endl;
            return -2;
        }

        return 0;
    }

    // - get DAQ Timestamp:
    uint64_t RawHeader::getTimeStamp() {
        if (m_addr == nullptr) {
            throw std::logic_error("wrong evt data address in RawHeader::getTimeStamp()");
        }
        UInt_t* _evtHdr = (UInt_t*)m_addr;
        uint64_t timestamp = 0;
        /* _evtHdr[7]: Second part
           _evtHdr[8]: Nano Second part */
        timestamp = _evtHdr[7];
        timestamp *= 1000000000;
        timestamp += _evtHdr[8];
        return timestamp;
    }

    void RawHeader::setEventEntry(const std::string& eventName, Long64_t& value) {
        if (eventName == "JM::RawWaveform") {
            m_waveform.setEntry(value);
        }
    }

    EventObject* RawHeader::event(const std::string& eventName) {
        if (eventName == "JM::RawWaveform") {
            return m_waveform.GetObject();
        }
        return 0;
    }

}
