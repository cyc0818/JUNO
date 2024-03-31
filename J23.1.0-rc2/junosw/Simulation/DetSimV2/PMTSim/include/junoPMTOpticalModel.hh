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

struct C4IPMTAccessor ; 

#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
#include "plog/Severity.h"
#endif

#ifdef PMTSIM_STANDALONE
    #ifdef POM_DEBUG
    #include "ModelTrigger_Debug.h"
    #endif
    class MultiFilmModel ; 
#else
    #include "SniperKernel/SvcBase.h"
    #include "Geometry/PMTParamSvc.h"
    #include "MultiFilmSimSvc/MultiFilmModel.h"
    #include "IPMTSimParamSvc/IPMTSimParamSvc.h"
    #include "Geometry/IPMTParamSvc.h"
#endif


enum EWhereAmI { OutOfRegion, kInGlass, kInVacuum, kWhereAmIUnset };


#ifdef PMTSIM_STANDALONE
#include "PMTSIM_API_EXPORT.hh"
class PMTSIM_API junoPMTOpticalModel : public G4VFastSimulationModel
#else
class            junoPMTOpticalModel : public G4VFastSimulationModel
#endif
{
    public:
        junoPMTOpticalModel(G4String, G4VPhysicalVolume*, G4Region*); 
        ~junoPMTOpticalModel();

        virtual G4bool IsApplicable(const G4ParticleDefinition&);

        static const int   ModelTrigger_IMPL ; 
        static const char* ModelTrigger_IMPL_Name() ; 
        static constexpr const char* _ModelTriggerBuggy  = "ModelTriggerBuggy"  ; 
        static constexpr const char* _ModelTriggerSimple = "ModelTriggerSimple" ; 

        virtual G4bool ModelTrigger(const G4FastTrack&);
        void CollectDebugInfo(bool trig);
        G4bool ModelTriggerBuggy_(const G4FastTrack&);
        G4bool ModelTriggerSimple_(const G4FastTrack&);

        static const G4double EPSILON ; 
        static G4double Distance(const G4VSolid* solid, const G4ThreeVector& pos, const G4ThreeVector& dir, EInside& in ); 

        virtual void DoIt(const G4FastTrack&, G4FastStep&);





#ifdef PMTSIM_STANDALONE
        void setPMTAccessor(const C4IPMTAccessor* accessor) { m_PMTAccessor = accessor ; }
#else
        void setPMTSimParamSvc(IPMTSimParamSvc* svc) { m_PMTSimParSvc = svc; }
        IPMTSimParamSvc* getPMTSimParamSvc() const { return m_PMTSimParSvc; }

        void setPMTParamSvc(IPMTParamSvc* svc) { m_PMTParamSvc = svc; }
        IPMTParamSvc* getPMTParamSvc() const { return m_PMTParamSvc; }
#endif
    
    private:
        G4MaterialPropertyVector* _rindex_glass;
        G4MaterialPropertyVector* _rindex_vacuum;

        G4VSolid* _inner1_solid;
        G4VSolid* _inner2_solid;
        G4VSolid* envelope_solid;

        const G4Track* _track ; 
        const G4VPhysicalVolume* _pv ; 
        const G4LogicalVolume* _mlv ;  


        G4VPhysicalVolume* _inner1_phys;
        G4VPhysicalVolume* _inner2_phys;

        G4LogicalVolume*   _inner1_log ; 
        G4LogicalVolume*   _inner2_log ; 

        G4double _photon_energy;
        G4double _wavelength;
        G4double _aoi;
        G4double _n1;
        G4double _n2, _k2, _d2;
        G4double _n3, _k3, _d3;
        G4double _n4;
        G4double _qe;
        G4double n_glass;
        G4double n_vacuum;
        G4double n_coating;
        G4double k_coating;
        G4double d_coating;
        G4double n_photocathode;
        G4double k_photocathode;
        G4double d_photocathode;

        G4double _sin_theta1;
        G4double _cos_theta1;
        G4complex _sin_theta4;
        G4complex _cos_theta4;
        
        G4double fR_s;
        G4double fT_s;
        G4double fR_p;
        G4double fT_p;
        G4double fR_n;
        G4double fT_n;

        G4double time;
        G4double dist1;
        G4double dist2;
        EInside  in1 ;
        EInside  in2 ;

        G4double energy;
        G4ThreeVector pos;
        G4ThreeVector dir;
        G4ThreeVector pol;
        G4ThreeVector norm;

        G4ThreeVector next_pos;
        G4ThreeVector next_norm;
        G4double      next_mct ; 

        EWhereAmI whereAmI;


#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
        static const plog::Severity LEVEL ; 
        static const int PIDX ; 
        static const bool PIDX_ENABLED ; 
        int m_track_id ; 
        bool m_track_dump ; 
#endif

#ifdef PMTSIM_STANDALONE   
        const C4IPMTAccessor* m_PMTAccessor;
#else
        IPMTParamSvc* m_PMTParamSvc;
        IPMTSimParamSvc* m_PMTSimParSvc;
#endif

#if defined(PMTSIM_STANDALONE) && defined(POM_DEBUG)
        ModelTrigger_Debug dbg ; 
#endif   

        MultiFilmModel* m_multi_film_model;

        void CalculateCoefficients();
        
        void Reflect();
        void Refract();

        void InitOpticalParameters(G4VPhysicalVolume* envelope_phys);
        void UpdateTrackInfo(G4FastStep &fastStep);

        int get_pmtid(const G4Track* track);
};

#endif
