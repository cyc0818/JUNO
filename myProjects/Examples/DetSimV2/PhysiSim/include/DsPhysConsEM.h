/**
 * \class DsPhysConsEM
 *
 * \brief ElectroMagentism physics
 *
 * Taken from G4dyb
 *
 *
 *
 * bv@bnl.gov Wed Apr 16 10:05:53 2008
 *
 */



#ifndef DSPHYSCONSEM_H
#define DSPHYSCONSEM_H

#include "G4VPhysicsConstructor.hh"
#include "SniperKernel/ToolBase.h"

class DsPhysConsEM : public G4VPhysicsConstructor
                   , public ToolBase
{

public:

    DsPhysConsEM(const G4String& name = "EM");
    virtual ~DsPhysConsEM();

    // Interface methods
    void ConstructParticle();
    void ConstructProcess();

private:

    /// FluorCut : cut for fluorescence from photons, electrons and
    /// bremsstrahlung photons:
    double m_fluorCut;

    bool m_positronium_flag; // true: on, false: off
};

#endif  // DSPHYSCONSEM_H
