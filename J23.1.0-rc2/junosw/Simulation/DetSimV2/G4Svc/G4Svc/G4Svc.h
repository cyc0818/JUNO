#ifndef G4Svc_h
#define G4Svc_h

#include "SniperKernel/SvcBase.h"
#include "G4Svc/IG4Svc.h"

class G4Svc: public IG4Svc, public SvcBase {
    public:
        G4Svc(const std::string& name);
        ~G4Svc();

        bool initialize();
        bool finalize();

        G4SvcRunManager* getRunMgr();

    private:
        G4SvcRunManager* m_runmgr;

};

#endif
