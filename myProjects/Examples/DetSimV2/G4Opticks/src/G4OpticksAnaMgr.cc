#include "G4OpticksAnaMgr.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include <sstream>
#include <fstream>

#include "OpMgr.hh"
#include "G4Material.hh"
#include "G4Event.hh"

DECLARE_TOOL(G4OpticksAnaMgr);

G4OpticksAnaMgr::G4OpticksAnaMgr(const std::string& name)
    : ToolBase(name), m_opmgr(0)
{

}

G4OpticksAnaMgr::~G4OpticksAnaMgr()
{

}

void
G4OpticksAnaMgr::BeginOfRunAction(const G4Run* aRun) {

    const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

    for (G4MaterialTable::const_iterator it=theMaterialTable->begin();
         it != theMaterialTable->end(); ++it) {
        m_mat_g[(*it)->GetName()] = (*it)->GetIndex();
    }

    // export a mapping file
    // {
    //    "LS": 48
    // }

    // To setup Lookup table in opticks, we prepare a json format string.
    std::stringstream ff; 
    // std::ofstream ff("GBoundaryLibMetadataMaterialMap.json");
    ff << "{" << std::endl;
    for (G4MaterialTable::const_iterator it=theMaterialTable->begin(); 
         it != theMaterialTable->end(); ++it) {
        ff << '"' << (*it)->GetName() << '"' << ": " << (*it)->GetIndex() << ',' << std::endl;
    }
    ff << '"' << "ENDMAT" << '"' << ": 999999" << std::endl;
    ff << "}" << std::endl;
    // ff.close();
    std::string json_string = ff.str();
    const char* json_str = json_string.c_str();

    LogInfo << json_str << std::endl;

    // construct opmgr
    // static const char* extracmd = " --gltf 3 --tracer --compute --save --embedded ";
    static const char* extracmd = " --gltf 3 --compute --save --embedded --natural ";
    m_opmgr = new OpMgr(0, 0, extracmd);

    m_opmgr->setLookup(json_str);
    // m_opmgr->snap();
}

void
G4OpticksAnaMgr::EndOfRunAction(const G4Run* aRun) {

}

void
G4OpticksAnaMgr::BeginOfEventAction(const G4Event* evt) {

}

void
G4OpticksAnaMgr::EndOfEventAction(const G4Event* evt) {
    // try to prepare the event (collections of gensteps) 
    // and start running on GPU

    // save the event
    std::stringstream ss;
    ss << "/tmp/output-genstep-" << evt->GetEventID() << ".npy";
    m_opmgr->saveEmbeddedGensteps(ss.str().c_str());

    m_opmgr->propagate();
}

void
G4OpticksAnaMgr::addGenstep( float* data, unsigned num_float ) {
    m_opmgr->addGenstep(data, num_float);
}
