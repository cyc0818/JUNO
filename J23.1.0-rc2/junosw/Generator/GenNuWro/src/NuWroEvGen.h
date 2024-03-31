#ifndef NuWroEvGen_h
#define NuWroEvGen_h

// ===========================================================================
// BELOW IS FOR GenTool
// ===========================================================================
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

#include <memory>
#include <string>

// use forward decl for NuWro interface
class nuwro_interface;

class NuWroEvGen: public ToolBase,
                  public IGenTool
                     {
public:
    NuWroEvGen(const std::string& name);
    ~NuWroEvGen() = default;

    bool configure();
    bool mutate(HepMC::GenEvent& event);

private:

    void Initialize         (void);

private:
    //Default options (override them using the command line arguments):
    std::string gOptNuWroConfigFile;
private:
    // Breaking GenerateEventsAtFixedInitState into two parts: configure and mutate. 
    // These variables are used to share between them.
    // The c++ shared_ptr is used to manage the objects.
    std::shared_ptr<nuwro_interface> evg_driver;
    int ievent = 0;
};

// ===========================================================================
// BELOW IS FOR event to HepMC 
// ===========================================================================

#include "HepMC/GenEvent.h"
class event;
template <typename T>
bool event2HepMC(T&&, HepMC::GenEvent&);

#endif