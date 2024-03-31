#ifndef IPreMixingFilter_hh
#define IPreMixingFilter_hh

/*
 * Description:
 *   In order to reduce the I/O when using pre-mixed data 
 *   in electroncis simulation, it is necessary to apply a filter
 *   during pre-mixing and re-calculate the rates. 
 *
 *   This interface defines whether the event should be stored into
 *   the pre-mixed data or not.
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 *   Sept 1st, 2022
 */

#include "EvtNavigator/NavBuffer.h"

class IPreMixingFilter {
public:
    virtual bool accept(JM::EvtNavigator* nav) = 0;
};

#endif
