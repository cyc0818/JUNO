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

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include <boost/filesystem.hpp>
#include <complex>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

junoPMTOpticalModel::junoPMTOpticalModel(G4String modelName, G4VPhysicalVolume* envelope_phys, G4Region* envelope)
    : G4VFastSimulationModel(modelName, envelope)
{
    _photon_energy  = 0.;
    _wavelength     = 0.;
    _n1             = 0.;
    _n2             = 0.;
    _k2             = 0.;
    _n3             = 0.;
    _efficiency     = 0.;
    _thickness      = 0.;
    
    _sin_theta1     = 0.;
    _cos_theta1     = 0.;
    _sin_theta3     = G4complex(0., 0.);
    _cos_theta3     = G4complex(0., 0.);

    fR_s            = 0.;
    fT_s            = 0.;
    fR_p            = 0.;
    fT_p            = 0.;
    fR_n            = 0.;
    fT_n            = 0.;

    n_glass         = 0.;
    n_vacuum        = 1.;

    time            = 0.;
    energy          = 0.;
    pos             = G4ThreeVector(0., 0., 0.);
    dir             = G4ThreeVector(0., 0., 0.);
    pol             = G4ThreeVector(0., 0., 0.);
    norm            = G4ThreeVector(0., 0., 0.);
    
    whereAmI        = OutOfRegion;

    InitOpticalParameters(envelope_phys);
}

junoPMTOpticalModel::~junoPMTOpticalModel()
{
}

G4bool junoPMTOpticalModel::IsApplicable(const G4ParticleDefinition & particleType)
{
    return (&particleType == G4OpticalPhoton::OpticalPhotonDefinition());
}

G4bool junoPMTOpticalModel::ModelTrigger(const G4FastTrack &fastTrack)
{
    G4double kCarTolerance = 3.e-9;

    if(fastTrack.GetPrimaryTrackLocalPosition().z()<=-kCarTolerance){
        return false;
    }

    envelope_solid = fastTrack.GetEnvelopeSolid();
    
    pos     = fastTrack.GetPrimaryTrackLocalPosition();
    dir     = fastTrack.GetPrimaryTrackLocalDirection();
    pol     = fastTrack.GetPrimaryTrackLocalPolarization();
    time    = fastTrack.GetPrimaryTrack()->GetGlobalTime();
    energy  = fastTrack.GetPrimaryTrack()->GetKineticEnergy();

    if(fastTrack.GetPrimaryTrack()->GetVolume() == _inner1_phys){
        whereAmI = kInVacuum;
    }else{
        whereAmI = kInGlass;
    }

    if(whereAmI == kInGlass){
        dist1 = envelope_solid->DistanceToOut(pos, dir);
        dist2 = _inner1_solid->DistanceToIn(pos, dir);

        if(dist1 >= dist2){
            return true;
        }
        return false;
    }else{
        dist1 = _inner1_solid->DistanceToOut(pos, dir);
        dist2 = _inner2_solid->DistanceToIn(pos, dir);

        if(dist1-dist2 < -kCarTolerance){
            return true;
        }
        return false;
    }

    return false;
}

void junoPMTOpticalModel::DoIt(const G4FastTrack& fastTrack, G4FastStep &fastStep)
{
    _photon_energy  = energy;
    _wavelength     = twopi*hbarc/energy;

    n_glass         = _rindex_glass->Value(_photon_energy);
    _n2             = _rindex_surface->Value(_photon_energy);
    _k2             = _kindex_surface->Value(_photon_energy);
    _thickness      = _thickness_surface->Value(pos.z());
    _efficiency     = _efficiency_surface->Value(_photon_energy);

    _efficiency = _efficiency*0.4/_eff_420nm;

    if(whereAmI == kInGlass){
        _n1 = n_glass;
        _n3 = n_vacuum;

        pos  += dist2*dir;
        time += dist2*_n1/c_light;
    }else{
        _n1 = n_vacuum;
        _n3 = n_glass;

        _efficiency = 0;
        pos  += dist1*dir;
        time += dist1/c_light;
    }
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
    escape_fac = _efficiency/An;

    if(escape_fac > 1.){
        G4cout<<"junoPMTOpticalMode: QE is larger than absorption coeff."<<G4endl;
    }

    G4double rand_absorb = G4UniformRand();
    G4double rand_escape = G4UniformRand();
    
    if(rand_absorb < A){ //change it back later
        // absorbed
        fastStep.ProposeTrackStatus(fStopAndKill);
        if(rand_escape<escape_fac){
        // detected
            fastStep.ProposeTotalEnergyDeposited(_photon_energy);
        }
    }else if(rand_absorb < A+R){
        // reflected
        Reflect();
        UpdateTrackInfo(fastStep);
    }else{
        // transmitted
        Refract();
        if(whereAmI == kInGlass){
            whereAmI = kInVacuum;
        }else{
            whereAmI = kInGlass;
        }
        UpdateTrackInfo(fastStep);
    }

    return;
}

void junoPMTOpticalModel::CalculateCoefficients()
{
    G4complex delta2(0., 0.);
    G4complex _N2(_n2, _k2);
    G4complex one(1., 0.);
    _sin_theta1 = sqrt(1.-_cos_theta1*_cos_theta1);
    G4complex _sin_theta2 = _n1 * _sin_theta1/_N2;
    G4complex _cos_theta2 = sqrt(one - _sin_theta2*_sin_theta2);
    _sin_theta3 = _n1 * _sin_theta1/_n3;
    _cos_theta3 = sqrt(one-_sin_theta3*_sin_theta3);

    G4complex zi(0., 1.), ampr(0., 0.), ampt(0., 0.);
    G4complex r12(0., 0.), r23(0., 0.), t12(0., 0.), t21(0., 0.), t23(0., 0.);

    //s-polarization
    r12 = (_n1*_cos_theta1-_N2*_cos_theta2)/(_n1*_cos_theta1+_N2*_cos_theta2);

    r23 = (_N2*_cos_theta2-_n3*_cos_theta3)/(_N2*_cos_theta2+_n3*_cos_theta3);
    t12 = (2.*_n1*_cos_theta1)/(_n1*_cos_theta1+_N2*_cos_theta2);
    t23 = (2.*_N2*_cos_theta2)/(_N2*_cos_theta2+_n3*_cos_theta3);

    delta2 = twopi * _N2 * _thickness * _cos_theta2/_wavelength;
    ampr = (r12+exp(2.*delta2*zi)*r23)/(1.+exp(2.*delta2*zi)*r12*r23);
    ampt = exp(zi*delta2)*t12*t23/(1.+exp(zi*2.*delta2)*r12*r23);

    fR_s = real(ampr*conj(ampr));
    fT_s = real(ampt*conj(ampt)*(_n3*real(_cos_theta3)/(_n1*_cos_theta1)));

    //p-polarization
    r12 = (_N2*_cos_theta1-_n1*_cos_theta2)/(_n1*_cos_theta2+_N2*_cos_theta1);
    r23 = (_n3*_cos_theta2-_N2*_cos_theta3)/(_N2*_cos_theta3+_n3*_cos_theta2);
    t12 = (2.*_n1*_cos_theta1)/(_n1*_cos_theta2+_N2*_cos_theta1);
    t23 = (2.*_N2*_cos_theta2)/(_N2*_cos_theta3+_n3*_cos_theta2);
    
    delta2 = twopi * _N2 * _thickness * _cos_theta2/_wavelength;
    ampr = (r12+exp(2.*delta2*zi)*r23)/(1.+exp(2.*delta2*zi)*r12*r23);
    ampt = exp(zi*delta2)*t12*t23/(1.+exp(zi*2.*delta2)*r12*r23);

    fR_p = real(ampr*conj(ampr));
    fT_p = real(ampt*conj(ampt)*(_n3*real(_cos_theta3))/(_n1*_cos_theta1));

    //norm incident
    r12 = (_n1 - _N2)/(_n1 + _N2);
    r23 = (_N2 - _n3)/(_N2 + _n3);
    t12 = (2. * _n1)/(_n1 + _N2);
    t23 = (2. * _N2)/(_N2 + _n3);
    
    delta2 = twopi * _N2 * _thickness/_wavelength;
    ampr = (r12+exp(2.*delta2*zi)*r23)/(1.+exp(2.*delta2*zi)*r12*r23);
    ampt = exp(zi*delta2)*t12*t23/(1.+exp(zi*2.*delta2)*r12*r23);

    fR_n = real(ampr*conj(ampr));
    fT_n = real(ampt*conj(ampt)*_n3/_n1);
}

void junoPMTOpticalModel::UpdateTrackInfo(G4FastStep &fastStep){
    fastStep.SetPrimaryTrackFinalTime(time);
    fastStep.SetPrimaryTrackFinalPosition(pos);
    fastStep.SetPrimaryTrackFinalMomentum(dir);
    fastStep.SetPrimaryTrackFinalPolarization(pol);
}

void junoPMTOpticalModel::InitOpticalParameters(G4VPhysicalVolume* envelope_phys){
    G4LogicalVolume* envelope_log 
        = envelope_phys->GetLogicalVolume();
    G4MaterialPropertiesTable* glass_pt 
        = envelope_log->GetMaterial()->GetMaterialPropertiesTable();

    _rindex_glass = glass_pt->GetProperty("RINDEX");

    _inner1_phys = envelope_log->GetDaughter(0);
    G4LogicalBorderSurface* Surface = 
        G4LogicalBorderSurface::GetSurface(envelope_phys, _inner1_phys);
    G4OpticalSurface*  OpticalSurface = 
        (G4OpticalSurface*)(Surface->GetSurfaceProperty());
    G4MaterialPropertiesTable* OpticalSurface_pt =
        OpticalSurface->GetMaterialPropertiesTable();
    
    _rindex_surface = OpticalSurface_pt->GetProperty("RINDEX");
    _kindex_surface = OpticalSurface_pt->GetProperty("KINDEX");
    _thickness_surface = OpticalSurface_pt->GetProperty("THICKNESS");
    _efficiency_surface = OpticalSurface_pt->GetProperty("EFFICIENCY");
    
    _eff_420nm      = _efficiency_surface->Value(2.9524*eV);

    _inner1_solid  = _inner1_phys->GetLogicalVolume()->GetSolid();
    _rindex_vacuum = _inner1_phys->GetLogicalVolume()->GetMaterial()
                   ->GetMaterialPropertiesTable()->GetProperty("RINDEX");
    
    _inner2_phys   = envelope_log->GetDaughter(1);
    _inner2_solid  = _inner2_phys->GetLogicalVolume()->GetSolid();
}

void junoPMTOpticalModel::Reflect()
{
    dir -= 2.*(dir*norm)*norm;
    pol -= 2.*(pol*norm)*norm;
}

void junoPMTOpticalModel::Refract()
{
    dir = (real(_cos_theta3) - _cos_theta1*_n1/_n3)*norm + (_n1/_n3)*dir;
    pol = (pol-(pol*dir)*dir).unit();
}
