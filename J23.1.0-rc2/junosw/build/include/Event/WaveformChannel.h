#ifndef WaveformChannel_H
#define WaveformChannel_H

#include "RtypesCore.h"

namespace JM
{
    class WaveformChannel
    {
        private :

            UInt_t     m_size;
            UShort_t*  m_data;

        public :

            WaveformChannel(UShort_t* data);
            virtual ~WaveformChannel();

            //waveform
            UShort_t*  waveformData() const;
            UInt_t     waveformSize() const;

            //header
            UShort_t   getPMTId() const;
            UShort_t   getTriggerNumber() const;
            UShort_t   getTriggerMode() const;
            ULong_t    getTime() const;

            //trailer
            UShort_t   getVersion() const;
            UShort_t   getDataType() const;
            UShort_t   getErrorRegister() const;
            UShort_t   getHighVoltageEnable() const;
            UShort_t   getGCUId() const;

    };
}

#endif //WaveformChannel_H
