#ifndef RawWaveform_H
#define RawWaveform_H

#include "Event/EventObject.h"
#include "Event/WaveformChannel.h"
#include <vector>

namespace JM
{
    class RawWaveform : public EventObject
    {
        private :
            UInt_t      m_size;
            Char_t*     m_data;     // [m_size][0,0,4096]

            std::vector<UInt_t>  m_chPos;
            std::vector<WaveformChannel>  m_chs;  //!

            RawWaveform(const RawWaveform&) = delete;
            RawWaveform& operator=(const RawWaveform&) = delete;

        public :
            RawWaveform();
            virtual ~RawWaveform();

            void setData(UInt_t size, Char_t* data);            // for DAQ test data
            void setChannelData(UInt_t size, Char_t* data);     // for GCU test data
            uint64_t getTimeStamp();                            // get GCU timestamp

            const std::vector<WaveformChannel>& getChannels();

            ClassDef(RawWaveform, 1)
    };
}

#endif //RawWaveform_H
