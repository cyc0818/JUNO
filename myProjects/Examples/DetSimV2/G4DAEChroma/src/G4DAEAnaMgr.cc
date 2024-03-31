
#include "G4DAEAnaMgr.hh"
#include "G4DAEChroma/G4DAEMetadata.hh"
#include "G4Event.hh"
#include "G4MaterialTable.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include <sstream>

DECLARE_TOOL(G4DAEAnaMgr);

G4DAEAnaMgr::G4DAEAnaMgr(const std::string& name) 
    : ToolBase(name)
{
    m_csl = 0;
    m_ssl = 0;
    m_matmap = 0;
    m_d_matmap = 0;
    g2c = 0;
    m_reduce_factor = 50;

    declProp("GGeoMatMap", m_dae_matmap_file);
    declProp("ReduceFactor", m_reduce_factor);
}

G4DAEAnaMgr::~G4DAEAnaMgr() {

}

void
G4DAEAnaMgr::BeginOfRunAction(const G4Run* aRun) {
    // material map of geant4
    const G4MaterialTable* mt = G4Material::GetMaterialTable();
    m_matmap = new G4DAEMaterialMap(mt);
    m_matmap->Print();

    // load the ggeo mat index from json file
    LogInfo << "Open Metadata file: '" << m_dae_matmap_file << "'" << std::endl;
    G4DAEMetadata* meta = G4DAEMetadata::CreateFromFile(m_dae_matmap_file.c_str()); 
    meta->Print("G4DAEAnaMgr::BeginOfRunAction ");
    // m_d_matmap = new G4DAEMaterialMap(meta, "/chroma_material_map");
    m_d_matmap = new G4DAEMaterialMap(meta, "/");
    m_d_matmap->Print();

    g2c = G4DAEMaterialMap::MakeLookupArray( m_matmap, m_d_matmap );

}

void
G4DAEAnaMgr::EndOfRunAction(const G4Run* aRun) {

}

void
G4DAEAnaMgr::BeginOfEventAction(const G4Event* evt) {
    delete m_csl;
    m_csl = new G4DAECerenkovStepList(10000);
    delete m_ssl;
    m_ssl = new G4DAEScintillationStepList(10000);
}

void
G4DAEAnaMgr::EndOfEventAction(const G4Event* evt) {
    int evtid = evt->GetEventID();    
    std::stringstream ss;
    ss << "csl-evt" << evtid << ".npy";
    std::string f = ss.str();
    // TODO only get a part of the steps to make the ggeoview run
    G4DAECerenkovStepList* slice_csl = new G4DAECerenkovStepList(m_csl, 0, m_csl->GetCount(), m_reduce_factor);
    slice_csl->SavePath(f.c_str(), "NPY");

    ss.str("");
    ss << "ssl-evt" << evtid << ".npy";
    f = ss.str();
    G4DAEScintillationStepList* slice_ssl = new G4DAEScintillationStepList(m_ssl, 0, m_ssl->GetCount(), m_reduce_factor);
    slice_ssl->SavePath(f.c_str(), "NPY");
    delete slice_csl;
}

