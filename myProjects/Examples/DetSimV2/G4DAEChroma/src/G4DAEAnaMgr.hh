#ifndef G4DAEAnaMgr_hh
#define G4DAEAnaMgr_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IAnalysisElement.h"
#include "G4DAEChroma/G4DAECerenkovStepList.hh"
#include "G4DAEChroma/G4DAEScintillationStepList.hh"
#include "G4DAEChroma/G4DAEMaterialMap.hh"

class G4DAEAnaMgr: public IAnalysisElement,
                   public ToolBase {
public:
    G4DAEAnaMgr(const std::string& name);
    ~G4DAEAnaMgr();

    // Run Action
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    // Event Action
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

public:
    // == methods to retrieve the Step List ==
    G4DAECerenkovStepList* getCSL() { return m_csl; }
    G4DAEScintillationStepList* getSSL() { return m_ssl; }
    G4DAEMaterialMap* getMM() { return m_matmap; }
    G4DAEMaterialMap* getDMM() { return m_d_matmap; }
    int* getG2C() { return g2c; }

private:
    G4DAECerenkovStepList* m_csl;
    G4DAEScintillationStepList* m_ssl;
    // for geant4 
    G4DAEMaterialMap* m_matmap;
    // for dae
    G4DAEMaterialMap* m_d_matmap;
    int* g2c;

    std::string m_dae_matmap_file;
    int m_reduce_factor;
};

#endif
