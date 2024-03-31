#include <cassert>
#include "junoPMTOpticalModel.hh"

#include "Randomize.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4VPhysicalVolume.hh"
#include "G4GeometryTolerance.hh"
#include "G4PhysicalConstants.hh"
#include "G4ParticleDefinition.hh"
#include "G4VSensitiveDetector.hh"
#include "G4MaterialPropertiesTable.hh"

#include "EGet/EGet.h"

#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
#include "ssys.h"
#include "SLOG.hh"
#include "C4Track.h"
#endif

#ifdef PMTSIM_STANDALONE
#include "MultiFilmSimSvc/MultiFilmModel.h"
#include "C4IPMTAccessor.h"
#else
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperException.h"
#include <boost/filesystem.hpp>
#endif

#include <complex>


#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
const plog::Severity junoPMTOpticalModel::LEVEL = SLOG::EnvLevel("junoPMTOpticalModel", "DEBUG" );
const int            junoPMTOpticalModel::PIDX  = ssys::getenvint("PIDX", -1) ; 
const bool           junoPMTOpticalModel::PIDX_ENABLED  = ssys::getenvbool("junoPMTOpticalModel__PIDX_ENABLED") ; 
#endif


junoPMTOpticalModel::junoPMTOpticalModel(G4String modelName, G4VPhysicalVolume* envelope_phys, G4Region* envelope)
    : G4VFastSimulationModel(modelName, envelope)
{
    _photon_energy  = 0.;
    _wavelength     = 0.;
    _aoi            = 0.;
    _n1             = 0.;
    _n2             = 0.;
    _k2             = 0.;
    _n3             = 0.;
    _k3             = 0.;
    _n4             = 0.;
    _qe             = 0.;

    _sin_theta1     = 0.;
    _cos_theta1     = 0.;
    _sin_theta4     = G4complex(0., 0.);
    _cos_theta4     = G4complex(0., 0.);
    
    fR_s            = 0.;
    fT_s            = 0.;
    fR_p            = 0.;
    fT_p            = 0.;
    fR_n            = 0.;
    fT_n            = 0.;

    n_glass         = 0.;
    n_coating       = 0.;
    k_coating       = 0.;
    d_coating       = 0.;
    n_photocathode  = 0.;
    k_photocathode  = 0.;
    d_photocathode  = 0.;
    n_vacuum        = 1.;

    time            = 0.;
    energy          = 0.;
    pos             = G4ThreeVector(0., 0., 0.);
    dir             = G4ThreeVector(0., 0., 0.);
    pol             = G4ThreeVector(0., 0., 0.);
    norm            = G4ThreeVector(0., 0., 0.);
    
    whereAmI        = OutOfRegion;

    InitOpticalParameters(envelope_phys);

    m_multi_film_model = new MultiFilmModel(4);

#ifdef PMTSIM_STANDALONE
    m_PMTAccessor = nullptr ; 
#endif

#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
    m_track_id = -1 ; 
    m_track_dump = false ; 
#endif


}

junoPMTOpticalModel::~junoPMTOpticalModel()
{
}

G4bool junoPMTOpticalModel::IsApplicable(const G4ParticleDefinition & particleType)
{
    return (&particleType == G4OpticalPhoton::OpticalPhotonDefinition());
}

/**
junoPMTOpticalModel::ModelTrigger
------------------------------------

Observed bug with NNVT PMT, getting absorption/detection on the Vac/Vac midline  
suspect due to the contorted ModelTrigger getting wrong idea of whereAmI arising 
from simplistic volume checking.  The binary assumption of which volume are in 
gets messed by when the volume is a child of inner2 rather than inner2 itself. 

**/

const int junoPMTOpticalModel::ModelTrigger_IMPL = EGet::Get<int>("junoPMTOpticalModel__ModelTrigger_IMPL", 0 ) ;  

const char* junoPMTOpticalModel::ModelTrigger_IMPL_Name() 
{
    const char* IMPL = nullptr ; 
    switch(ModelTrigger_IMPL)
    {   
       case 0: IMPL = _ModelTriggerSimple ; break ; 
       case 1: IMPL = _ModelTriggerBuggy  ; break ; 
    }   
    return IMPL ; 
}

/**
junoPMTOpticalModel::ModelTrigger
-----------------------------------


**/

G4bool junoPMTOpticalModel::ModelTrigger(const G4FastTrack &fastTrack)
{
    _track = fastTrack.GetPrimaryTrack();

#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
    m_track_id = C4Track::GetLabelID(_track) ;  
    m_track_dump = ( m_track_id == PIDX && PIDX_ENABLED ) || PIDX == -2  ; 
    LOG_IF(info, m_track_dump ) << " PIDX " << PIDX << " m_track_id " << m_track_id ; 
#endif

    _pv = _track->GetVolume() ;
    _mlv = _pv->GetMotherLogical();  

    envelope_solid = fastTrack.GetEnvelopeSolid();

    pos     = fastTrack.GetPrimaryTrackLocalPosition();
    dir     = fastTrack.GetPrimaryTrackLocalDirection();
    pol     = fastTrack.GetPrimaryTrackLocalPolarization();
    time    = fastTrack.GetPrimaryTrack()->GetGlobalTime();
    energy  = fastTrack.GetPrimaryTrack()->GetKineticEnergy();

    dist2 = kInfinity ;

    G4bool trig = ModelTrigger_IMPL == 0 ? ModelTriggerSimple_(fastTrack) : ModelTriggerBuggy_(fastTrack)  ; 
    CollectDebugInfo(trig); 
    return trig ; 
}


void junoPMTOpticalModel::CollectDebugInfo(bool trig)
{
#if defined(PMTSIM_STANDALONE) && defined(POM_DEBUG)
    const char* pv_n = _pv->GetName().c_str(); 
    const char* mlv_n = _mlv->GetName().c_str(); 

    dbg.pos_x = pos.x(); 
    dbg.pos_y = pos.y(); 
    dbg.pos_z = pos.z(); 
    dbg.time  = time ; 

    dbg.dir_x = dir.x(); 
    dbg.dir_y = dir.y(); 
    dbg.dir_z = dir.z(); 
    dbg.energy = energy ; 

    dbg.dist1 = dist1 ; 
    dbg.dist2 = dist2 ; 
    dbg.mlv   = ModelTrigger_Debug::MLV.add(mlv_n) ; 
    //dbg.etrig.set called from ModelTriggerBuggy_ enumerating code path of the return 

    dbg.index = m_track_id ; 
    dbg.pv    = ModelTrigger_Debug::PV.add(pv_n) ; 
    dbg.whereAmI = (uint64_t)whereAmI ; 
    dbg.trig = trig ; 

    dbg.next_pos_x = next_pos.x(); 
    dbg.next_pos_y = next_pos.y(); 
    dbg.next_pos_z = next_pos.z(); 
    dbg.next_mct  = next_mct ; 

    dbg.next_norm_x = next_norm.x(); 
    dbg.next_norm_y = next_norm.y(); 
    dbg.next_norm_z = next_norm.z(); 
    dbg.impl  = ModelTrigger_IMPL ; 

    dbg.EInside1 = in1 ; 
    dbg.s61 = 0 ; 
    dbg.s62 = 0 ; 
    dbg.s63 = 0 ; 

    dbg.add(); 
    //if(trig) dbg.add(); 
#endif
}



/**
junoPMTOpticalModel::ModelTriggerBuggy_
------------------------------------------
**/

G4bool junoPMTOpticalModel::ModelTriggerBuggy_(const G4FastTrack& )
{
    if(_pv == _inner2_phys ){
#ifdef POM_DEBUG
        dbg.etrig.set("N_PV_I2 ") ; 
#endif
        return false;
    }

    if(_pv == _inner1_phys){    // VERY FLAWED BINARY ASSUMPTION
        whereAmI = kInVacuum;
    }else{
        whereAmI = kInGlass;
    }

    if(whereAmI == kInGlass) // REFLECTIONS ON DYNODE TURN UP HERE 
    {
        dist1 = _inner1_solid->DistanceToIn(pos, dir);
        dist2 = _inner2_solid->DistanceToIn(pos, dir);

        /**
        With HAMA _inner2_solid is a G4BooleanSolid 
        that gives unexpected DistanceToIn when pos is inside 
        see u4/tests/G4VSolid_Test.sh  

        dist1,dist2
            DistanceToIn:kInfinity for pos at farside and beyond 
            DistanceToIn:zero      for pos at nearside and inside (but not with booleans)

        **/

        if(dist1 == kInfinity)
        {
#ifdef POM_DEBUG
            dbg.etrig.set("N_MIS_I1") ; // not pointing towards inner1 
#endif
            return false;
        }
        else if(dist1>dist2)
        {
#ifdef POM_DEBUG
            dbg.etrig.set("N_I1>I2 ") ;  // are closer to inner2 than inner1 : ie approaching lower hemi   
#endif
            return false;
        }
        else
        {
#ifdef POM_DEBUG
            dbg.etrig.set("Y_GLASS ") ; //  are closer to inner1 : ie approaching upper hemi
#endif
            return true;
        }
    }
    else   // whereAmI:kInVacuum meaning pv != _inner1_phys
    {
        dist1 = _inner1_solid->DistanceToOut(pos, dir);  
        dist2 = _inner2_solid->DistanceToIn(pos, dir);  

        /**
        dist1 
           * DistanceToOut : distance to farside, 0 at-and-beyond farside 

        dist2 
           * DistanceToIn:zero      :  pos inside _inner2_solid 
           * DistanceToIn:kInfinity :  pos at-OR-beyond farside of _inner2 or miss 
           
        (see u4/tests/G4Orb_Test.cc)
        **/ 

        if(dist2 == kInfinity){  
#ifdef POM_DEBUG
            dbg.etrig.set("Y_VACUUM") ; 
#endif
            return true;
        }
    }


#ifdef POM_DEBUG
    dbg.etrig.set("N_TAIL  ") ; 
#endif
    return false;
}

/**
junoPMTOpticalModel::ModelTriggerSimple_
------------------------------------------

This is a "pre-trigger" returning true when approaching 
the upper hemisphere of inner1 from inside or outside.

The implementation is based on the distance *dist1*
in the current momentum direction of the photon to the inner1 solid. 

*dist1* is required to be greater than EPSILON
where EPSILON is 2e-4 which is one fifth of the 
distance between body and inner1 1e-3.

Also the local z of next_pos which is *dist1* ahead
is required to exceed EPSILON in order to exclude
situations where next position is on the Vacuum/Vacuum midline. 

Thus the positions where this returns true 
are expected to be very close to the fake "body" volume 
when coming from outside or close to fake Vacumm/Vacuum 
border when approaching from inside. 

Note that the implemetation avoids relying on the volume that 
Geant4 thinks are currently at because that is unreliable.
Also as the PMT has various volumes for dynodes and MCT 
within inner2 a current volume based implementation would 
be complicated.   

::

     +---Pyrex--pmt-------------------+
     |                                |
     |   +--Pyrex-body - - - - - +    |
     |   : +~~Vacuum~~inner1~~~+ :    |
     |   : !                   ! :    |
     |   : !                   !1e-3  |
     |   : !                   ! :    |
     |   + +- - - - - - - - ---+ +    |
     |   : |                   | :    |
     |   : |                   | :    |
     |   : |                   | :    |
     |   : +--Vacuum--inner2---+ :    |
     |   +- - - - - - - - - - - -+    |
     |                                |
     +--------------------------------+


**/


G4bool junoPMTOpticalModel::ModelTriggerSimple_(const G4FastTrack& )
{
    bool trig = false ; 
    dist1 = Distance( _inner1_solid, pos, dir, in1 ); 
    if( dist1 != kInfinity && dist1 > EPSILON ) 
    {
        next_pos = pos + dir*dist1 ;  
        trig = next_pos.z() > EPSILON ;  // disqualify flat face intersects 

        next_norm = _inner1_solid->SurfaceNormal(next_pos);
        next_mct  = next_norm * dir ; 
        whereAmI  = next_mct < 0. ? kInGlass : kInVacuum ; // against/with normal is Glass/Vacuum
    }

#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
    LOG_IF(info, m_track_dump) 
        << " PIDX " << PIDX 
        << " label.id " << m_track_id 
        << " dist1 " << dist1 
        << " trig " << trig 
        << " whereAmI " << whereAmI
        ; 
#endif
    return trig ; 
}

const G4double junoPMTOpticalModel::EPSILON = 2e-4 ; 



G4double junoPMTOpticalModel::Distance(const G4VSolid* solid, const G4ThreeVector& pos, const G4ThreeVector& dir, EInside& in ) // static
{
    in =  solid->Inside(pos) ; 
    G4double t = kInfinity ; 
    switch( in )
    {   
        case kInside:  t = solid->DistanceToOut( pos, dir ) ; break ; 
        case kSurface: t = solid->DistanceToOut( pos, dir ) ; break ; 
        case kOutside: t = solid->DistanceToIn(  pos, dir ) ; break ; 
        default:  assert(0) ; 
    }   
    return t ; 
}


void junoPMTOpticalModel::DoIt(const G4FastTrack& fastTrack, G4FastStep &fastStep)
{
    const G4Track* track = fastTrack.GetPrimaryTrack();

    _photon_energy  = energy;    // from ModelTrigger
    _wavelength     = twopi*hbarc/energy;

    int pmtid  = get_pmtid(track);

#ifdef PMTSIM_STANDALONE
    assert( m_PMTAccessor ); 
    int pmtcat = m_PMTAccessor->get_pmtcat(pmtid) ; 
    _qe  = m_PMTAccessor->get_pmtid_qe(pmtid, energy);


    std::array<double, 16> ss ; 
    m_PMTAccessor->get_stackspec(ss, pmtcat,  _photon_energy/eV ) ; 

    n_glass = ss[4*0+0] ; 

    n_coating = ss[4*1+0] ; 
    k_coating = ss[4*1+1] ;  
    d_coating = ss[4*1+2] ; 

    n_photocathode = ss[4*2+0] ; 
    k_photocathode = ss[4*2+1] ; 
    d_photocathode = ss[4*2+2] ; 

    n_vacuum = ss[4*3+0] ; 

    LOG(LEVEL) 
        << " pmtid " << pmtid 
        << " pmtcat " << pmtcat
        << " _qe " << _qe 
        << " _photon_energy/eV " << _photon_energy/eV
        << " n_glass " << n_glass
        << " n_coating " << n_coating
        << " k_coating " << k_coating
        << " d_coating " << d_coating
        << " n_photocathode " << n_photocathode
        << " k_photocathode " << k_photocathode
        << " d_photocathode " << d_photocathode
        << " n_vacuum " << n_vacuum
        ; 


#else
    int pmtcat = m_PMTParamSvc->getPMTCategory(pmtid);
    
    n_glass         = _rindex_glass->Value(_photon_energy);
    
    _qe             = m_PMTSimParSvc->get_pmtid_qe(pmtid, energy);

    n_coating       = m_PMTSimParSvc->get_pmtcat_prop(pmtcat, "ARC_RINDEX", _photon_energy);
    k_coating       = m_PMTSimParSvc->get_pmtcat_prop(pmtcat, "ARC_KINDEX", _photon_energy);
    d_coating       = m_PMTSimParSvc->get_pmtcat_const_prop(pmtcat, "ARC_THICKNESS")/m;

    n_photocathode  = m_PMTSimParSvc->get_pmtcat_prop(pmtcat, "PHC_RINDEX", _photon_energy);
    k_photocathode  = m_PMTSimParSvc->get_pmtcat_prop(pmtcat, "PHC_KINDEX", _photon_energy);
    d_photocathode  = m_PMTSimParSvc->get_pmtcat_const_prop(pmtcat, "PHC_THICKNESS")/m;
#endif


    if(whereAmI == kInGlass){
        _n1 = n_glass;
        _n2 = n_coating;
        _k2 = k_coating;
        _d2 = d_coating;
        _n3 = n_photocathode;
        _k3 = k_photocathode;
        _d3 = d_photocathode;
        _n4 = n_vacuum;
    }else{
        _n1 = n_vacuum;
        _n2 = n_photocathode;
        _k2 = k_photocathode;
        _d2 = d_photocathode;
        _n3 = n_coating;
        _k3 = k_coating;
        _d3 = d_coating;
        _n4 = n_glass;

        _qe = 0.;
    }
    pos  += dist1*dir;
    time += dist1*_n1/c_light;

    UpdateTrackInfo(fastStep);

    fastTrack.GetPrimaryTrack()->GetStep()
        ->GetPostStepPoint()->SetStepStatus(fGeomBoundary);

    norm = _inner1_solid->SurfaceNormal(pos);
    if(whereAmI == kInGlass){
        norm *= -1.0;
    }

    _cos_theta1 = dir*norm;

    if(_cos_theta1 < 0.){
        _cos_theta1 = -_cos_theta1;
        norm = -norm;
    }
    _aoi = acos(_cos_theta1)*360./twopi;


#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
    LOG(LEVEL) 
        << " _cos_theta1 " << _cos_theta1
        << " _aoi " << _aoi 
        << " m_track_id " << m_track_id
        ; 
#endif

    CalculateCoefficients();

    G4double T  = 0.;
    G4double R  = 0.;
    G4double A  = 0.;
    G4double An = 0.;
    G4double escape_fac = 0.;
    G4double E_s2 = 0.;

    if(_sin_theta1 > 0.){
        E_s2 = (pol*dir.cross(norm))/_sin_theta1;
        E_s2 *= E_s2;
    }else{
        E_s2 = 0.;
    }

    T = fT_s*E_s2 + fT_p*(1.0-E_s2);
    R = fR_s*E_s2 + fR_p*(1.0-E_s2);
    A = 1.0 - (T+R);

    An = 1.0 - (fT_n+fR_n);
    escape_fac  = _qe/An;


#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
    LOG(LEVEL) 
       << " E_s2 " << E_s2
       << " fT_s " << fT_s  
       << " fT_p " << fT_p
       << " T " << T 
       << " fR_s " << fR_s  
       << " fR_p " << fR_p
       << " R " << R
       << " A " << A 
       << " fT_n " << fT_n 
       << " fR_n " << fR_n 
       << " An " << An
       << " escape_fac " << escape_fac
       ;
#endif


    if(escape_fac > 1.){
        G4cout<<"junoPMTOpticalModel: QE is larger than absorption coeff."<<G4endl;
    }

    G4double rand_absorb = G4UniformRand();
    G4double rand_escape = G4UniformRand();

    if(rand_absorb < A){
        // absorbed
        fastStep.ProposeTrackStatus(fStopAndKill);
        if(rand_escape<escape_fac){
        // detected
            fastStep.ProposeTotalEnergyDeposited(_photon_energy);
        }
    }else if(rand_absorb < A+R){
        // fastStep.ProposeTrackStatus(fStopAndKill);
        // reflected
        Reflect();
        UpdateTrackInfo(fastStep);
    }else{
        // fastStep.ProposeTrackStatus(fStopAndKill);
        // transmitted
        Refract();
        if(whereAmI == kInGlass){
            whereAmI = kInVacuum;
        }else{
            whereAmI = kInGlass;
        }
        UpdateTrackInfo(fastStep);
    }

#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
    {
        LOG_IF(info, m_track_dump ) 
             << " PIDX " << PIDX 
             << " track.GetMomentumDirection " << track->GetMomentumDirection() 
             ; 

        G4double& u0 = rand_absorb ; 
        G4double& u1 = rand_escape ;
        G4double& D  = escape_fac ; 
     
        char status = '?' ; 
        if(      u0 < A)    status = u1 < D ? 'D' : 'A' ; 
        else if( u0 < A+R)  status = 'R' ; 
        else                status = 'T' ;  

        C4Track::SetLabelFlg(track, status);  // U4Recorder passes this into opticks/SEvt   

        LOG(LEVEL) 
            << " pmtid " << pmtid 
            << " pmtcat " << pmtcat
            << " A " << A
            << " R " << R
            << " A+R " << A+R
            << " T " << ( 1. - (A+R) )
            << " D " << D 
            << " u0 " << u0 
            << " status " << status
            ;
    }
#endif

    return;
}

void junoPMTOpticalModel::CalculateCoefficients()
{
    G4complex one(1., 0.);
    _sin_theta1 = sqrt(1.-_cos_theta1*_cos_theta1);
    _sin_theta4 = _n1 * _sin_theta1/_n4;
    _cos_theta4 = sqrt(one-_sin_theta4*_sin_theta4);

#ifdef PMTSIM_STANDALONE
    m_multi_film_model->SetWL(_wavelength/nm);
#else
    m_multi_film_model->SetWL(_wavelength/m);
#endif
    m_multi_film_model->SetAOI(_aoi);

    m_multi_film_model->SetLayerPar(0, _n1);
    m_multi_film_model->SetLayerPar(1, _n2, _k2, _d2);
    m_multi_film_model->SetLayerPar(2, _n3, _k3, _d3);
    m_multi_film_model->SetLayerPar(3, _n4);
    ART art1 = m_multi_film_model->GetART();
    fR_s = art1.R_s;
    fT_s = art1.T_s;
    fR_p = art1.R_p;
    fT_p = art1.T_p;

    m_multi_film_model->SetLayerPar(0, n_glass);
    m_multi_film_model->SetLayerPar(1, n_coating, k_coating, d_coating);
    m_multi_film_model->SetLayerPar(2, n_photocathode, k_photocathode, d_photocathode);
    m_multi_film_model->SetLayerPar(3, n_vacuum);
    ART art2 = m_multi_film_model->GetNormalART();
    fR_n = art2.R;
    fT_n = art2.T;
}

void junoPMTOpticalModel::UpdateTrackInfo(G4FastStep &fastStep)
{
    fastStep.SetPrimaryTrackFinalTime(time);
    fastStep.SetPrimaryTrackFinalPosition(pos);
    fastStep.SetPrimaryTrackFinalMomentum(dir);
    fastStep.SetPrimaryTrackFinalPolarization(pol);
    fastStep.ForceSteppingHitInvocation();


}

void junoPMTOpticalModel::InitOpticalParameters(G4VPhysicalVolume* envelope_phys)
{
    G4LogicalVolume* envelope_log 
        = envelope_phys->GetLogicalVolume();
    G4MaterialPropertiesTable* glass_pt 
        = envelope_log->GetMaterial()->GetMaterialPropertiesTable();
    
    _rindex_glass   = glass_pt->GetProperty("RINDEX");
    _inner1_phys    = envelope_log->GetDaughter(0);
    _inner1_log     = _inner1_phys->GetLogicalVolume() ; 
    _inner1_solid   = _inner1_log->GetSolid();
    _rindex_vacuum  = _inner1_log->GetMaterial()
                                  ->GetMaterialPropertiesTable()->GetProperty("RINDEX");

    _inner2_phys    = envelope_log->GetDaughter(1);
    _inner2_log     = _inner2_phys->GetLogicalVolume(); 
    _inner2_solid   = _inner2_log->GetSolid();

}

void junoPMTOpticalModel::Reflect()
{
    dir -= 2.*(dir*norm)*norm;
    pol -= 2.*(pol*norm)*norm;
}

void junoPMTOpticalModel::Refract()
{
    dir = (real(_cos_theta4) - _cos_theta1*_n1/_n4)*norm + (_n1/_n4)*dir;
    pol = (pol-(pol*dir)*dir).unit();

#if defined(WITH_G4CXOPTICKS) || defined(PMTSIM_STANDALONE)
    LOG_IF(info, m_track_dump ) 
        << " PIDX " << PIDX 
        << " m_track_id " << m_track_id
        << " norm " << norm 
        << " dir " << dir
        << " _n1 " << _n1 
        << " _n4 " << _n4
        << " _cos_theta1 " << _cos_theta1
        ; 
#endif

}

int junoPMTOpticalModel::get_pmtid(const G4Track* track) {
    int ipmt= -1;
    {
        const G4VTouchable* touch= track->GetTouchable();
        int nd= touch->GetHistoryDepth();
        int id=0;
        for (id=0; id<nd; id++) {
            if (touch->GetVolume(id)==track->GetVolume()) {
                int idid=1;
                G4VPhysicalVolume* tmp_pv=NULL;
                for (idid=1; idid < (nd-id); ++idid) {
                    tmp_pv = touch->GetVolume(id+idid);

                    G4LogicalVolume* mother_vol = tmp_pv->GetLogicalVolume();
                    G4LogicalVolume* daughter_vol = touch->GetVolume(id+idid-1)->
                        GetLogicalVolume();
                    int no_daugh = mother_vol -> GetNoDaughters();
                    if (no_daugh > 1) {
                        int count = 0;
                        for (int i=0; (count<2) &&(i < no_daugh); ++i) {
                            if (daughter_vol->GetName()
                                    ==mother_vol->GetDaughter(i)->GetLogicalVolume()->GetName()) {
                                ++count;
                            }
                        }
                        if (count > 1) {
                            break;
                        }
                    }
                }
                ipmt= touch->GetReplicaNumber(id+idid-1);
                break;
            }
        }
        if (ipmt < 0) {
            G4Exception("junoPMTOpticalModel: could not find envelope -- where am I !?!", // issue
                    "", //Error Code
                    FatalException, // severity
                    "");
        }
    }

    return ipmt;
}
