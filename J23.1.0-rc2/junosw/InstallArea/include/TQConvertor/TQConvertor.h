#ifndef TQCONVERTOR_H
#define TQCONVERTOR_H
#include "Event/CdLpmtElecHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtElecHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "Event/WpElecHeader.h"
#include "Event/WpCalibHeader.h"
#include "Event/TtElecHeader.h"
#include "Event/TtCalibHeader.h"

class TQConvertor{
    public:
        TQConvertor();
        ~TQConvertor();

        enum TQSRC{
            WFREC=0, FPGA
        };
        // = Indicate whether tq is from waveform rec alg or FPGA =
        // = May be stored in EDM in the future =

        // = Convert TQ format from calib to elec =
        // = Used by OEC =
        JM::CdLpmtElecHeader* convertTQ(JM::CdLpmtCalibHeader*, TQSRC);
        JM::CdSpmtElecHeader* convertTQ(JM::CdSpmtCalibHeader*, TQSRC);
        JM::WpElecHeader* convertTQ(JM::WpCalibHeader*, TQSRC);
        JM::TtElecHeader* convertTQ(JM::TtCalibHeader*, TQSRC);

        // = Convert TQ format from elec to calib =
        // = Used by reconstruction =
        JM::CdLpmtCalibHeader* convertTQ(JM::CdLpmtElecHeader*, TQSRC);
        JM::CdSpmtCalibHeader* convertTQ(JM::CdSpmtElecHeader*, TQSRC);
        JM::WpCalibHeader* convertTQ(JM::WpElecHeader*, TQSRC);
        JM::TtCalibHeader* convertTQ(JM::TtElecHeader*, TQSRC);

    private:
        // = Convert T/Q format for tq from rec alg =
        uint32_t t_flt2uint(float t_flt);
        float t_uint2flt(uint32_t t_uint);
        uint32_t q_flt2uint(float q_flt);
        float q_uint2flt(uint32_t q_uint);
};
#endif
