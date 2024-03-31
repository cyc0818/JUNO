#ifndef G4SvcRunManager_h
#define G4SvcRunManager_h

#include "G4RunManager.hh"

class G4SvcRunManager: public G4RunManager {
    public:
        G4SvcRunManager();
        ~G4SvcRunManager();

        bool initializeRM();
        bool SimulateEvent(int i_event);
        bool finalizeRM();

};

#endif
