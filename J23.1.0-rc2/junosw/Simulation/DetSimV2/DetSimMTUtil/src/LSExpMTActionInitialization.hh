#ifndef LSExpMTActionInitialization_hh
#define LSExpMTActionInitialization_hh

#include "G4VUserActionInitialization.hh"


class LSExpMTActionInitialization : public G4VUserActionInitialization
{
public:
    LSExpMTActionInitialization();
    ~LSExpMTActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

    virtual G4VSteppingVerbose* InitializeSteppingVerbose() const;

};

#endif
