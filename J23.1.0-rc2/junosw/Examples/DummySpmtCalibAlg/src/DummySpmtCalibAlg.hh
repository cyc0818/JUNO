#ifndef DummySpmtCalibAlg_hh
#define DummySpmtCalibAlg_hh

/*
 * In this example, the CalibEvent will be loaded first,
 * then the channels data will be added into the existing CalibEvent.
 */

#include <boost/python.hpp>

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"

#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"

#include "Event/ElecHeader.h"
#include "Event/CalibHeader.h"

#include "EvtNavigator/NavBuffer.h"

class DummySpmtCalibAlg: public AlgBase {

public:
    DummySpmtCalibAlg(const std::string& name);
    ~DummySpmtCalibAlg();


    bool initialize(); 
    bool execute(); 
    bool finalize(); 

private:
    JM::NavBuffer* m_buf; 


};

#endif
