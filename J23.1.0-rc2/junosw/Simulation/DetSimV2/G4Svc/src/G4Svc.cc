#include "G4Svc/G4Svc.h"
#include "G4Svc/G4SvcRunManager.h"

#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"

DECLARE_SERVICE(G4Svc);

G4Svc::G4Svc(const std::string& name)
    : SvcBase(name)
{
    LogDebug << "Create G4SvcRunManager in G4Svc" << std::endl;
    m_runmgr = new G4SvcRunManager;
}

G4Svc::~G4Svc()
{
    LogDebug << "Delete G4SvcRunManager in G4Svc" << std::endl;
    if (m_runmgr) {
        delete m_runmgr;
    }
    m_runmgr = NULL;
}

// * initialize and finalize is the interface in SvcBase
bool G4Svc::initialize() {
    return true;
}

bool G4Svc::finalize() {
    return true;
}

G4SvcRunManager* G4Svc::getRunMgr()
{
    return m_runmgr;
}

