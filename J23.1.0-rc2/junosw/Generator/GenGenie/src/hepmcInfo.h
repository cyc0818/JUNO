#ifndef hepmcInfo_h
#define hepmcInfo_h

/*
 * Description:
 *
 *     1. get the information of hepmc event  
 *     2. modify the direction of hepmc event  
 *
 *
 * Author:
 *     Jie Cheng <chengjie AT ncepu.edu.cn>
 */

#include "HepMC/GenEvent.h"
#include "def.h"

bool isotropic_direction(HepMC::GenEvent& event);

#endif
