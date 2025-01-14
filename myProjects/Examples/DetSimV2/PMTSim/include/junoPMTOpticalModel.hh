#ifndef junoPMTOpticalModel_h
#define junoPMTOPticalModel_h 1

#include "G4Step.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4StepPoint.hh"
#include "G4FastTrack.hh"
#include "G4Navigator.hh"
#include "G4TouchableHandle.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4VFastSimulationModel.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4Track.hh"
#include "TGraph2D.h"

enum EWhereAmI { OutOfRegion, kInGlass, kInVacuum };

class junoPMTOpticalModel : public G4VFastSimulationModel
{
    public:
        junoPMTOpticalModel(G4String, G4VPhysicalVolume*, G4Region*); 
        ~junoPMTOpticalModel();

        virtual G4bool IsApplicable(const G4ParticleDefinition&);
        virtual G4bool ModelTrigger(const G4FastTrack&);
        virtual void DoIt(const G4FastTrack&, G4FastStep&);

    private:
        
        G4MaterialPropertyVector* _rindex_glass;
        G4MaterialPropertyVector* _rindex_vacuum;
        G4MaterialPropertyVector* _rindex_surface;
        G4MaterialPropertyVector* _kindex_surface;
        G4MaterialPropertyVector* _thickness_surface;
        G4MaterialPropertyVector* _efficiency_surface;

        G4VSolid* _inner1_solid;
        G4VSolid* _inner2_solid;
        G4VPhysicalVolume* _inner1_phys;
        G4VPhysicalVolume* _inner2_phys;
        G4VSolid* envelope_solid;


        G4double _photon_energy;
        G4double _wavelength;
        G4double _n1;
        G4double _n2, _k2;
        G4double _n3;
        G4double n_glass;
        G4double n_vacuum;
        G4double _efficiency;
        G4double _thickness;
        G4double _eff_420nm;
        
        G4double  _sin_theta1;
        G4double  _cos_theta1;
        G4complex _sin_theta3;
        G4complex _cos_theta3;

        G4double fR_s;
        G4double fT_s;
        G4double fR_p;
        G4double fT_p;
        G4double fR_n;
        G4double fT_n;

        G4double time;
        G4double dist1;
        G4double dist2;
        G4double energy;
        G4ThreeVector pos;
        G4ThreeVector dir;
        G4ThreeVector pol;
        G4ThreeVector norm;

        EWhereAmI whereAmI;

        void CalculateCoefficients();

        void Reflect();
        void Refract();

        void InitOpticalParameters(G4VPhysicalVolume* envelope_phys);
        void UpdateTrackInfo(G4FastStep &fastStep);

};

#endif
