#ifndef ghep2hepmc_h
#define ghep2hepmc_h

/*
 * Description:
 *     Utility to convert the GHEP (GENIE Event Record) to HepMC
 *
 *     GENIE source code src/Apps/gNtpConv.cxx provides a good example. 
 *
 * Author:
 *     Tao Lin <lintao AT ihep.ac.cn>
 */

#include "Framework/EventGen/EventRecord.h"
#include "HepMC/GenEvent.h"

bool ghep2hepmc(genie::EventRecord&, HepMC::GenEvent&);

#endif
