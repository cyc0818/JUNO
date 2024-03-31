/*
 * =====================================================================================
 *
 *       Filename:  GtOpticksTool.h
 *
 *    Description:  Used to provide input photons to Opticks for debugging/validation 
 *                  see opticks/ana/input_photons.py for a python script that 
 *                  creates input photon .npy arrays
 *
 *                  One vertex is created for each input optical photon.
 *
 *                  This class needs to be compiled -DWITH_G4CXOPTICKS to do anything useful, 
 *                  although it will compile without that preprocessor macro.
 *
 *         Author:  Simon Blyth (blyth@ihep.ac.cn), 
 *   Organization:  IHEP
 *
 * =====================================================================================
 */
#ifndef GtOpticksTool_h
#define GtOpticksTool_h

#include <string>
#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"
struct NP ;  

namespace HepMC {
    class GenEvent;
}

class GtOpticksTool: public ToolBase,
                     public IGenTool
{
private:
    static const GtOpticksTool* fInstance ; 
    static const char* configure_FAIL_NOTES ;
public:
    static const GtOpticksTool* Get(); 
    GtOpticksTool(const std::string& name);
    ~GtOpticksTool();
public:
    bool configure();
    bool mutate(HepMC::GenEvent& event);
    NP*  getInputPhoton() const ;
private:
    void add_optical_photon(HepMC::GenEvent& event, int idx );
private:
    NP*  m_input_photon ; 

};

#endif
