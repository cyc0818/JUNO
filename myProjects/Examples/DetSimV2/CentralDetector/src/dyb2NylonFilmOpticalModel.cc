
#include "dyb2NylonFilmOpticalModel.hh"
#include "G4OpticalPhoton.hh"
#include "G4Version.hh"

#include "G4UnitsTable.hh"
//#include "LSExpAnalysisManager.hh"
#include "Randomize.hh"
#include <TMath.h>
#include <TF1.h>

///#if G4VERSION_NUMBER >= 900
  #include "G4GeometryTolerance.hh"
//#endif
using namespace CLHEP;

// constructor -- also handles all initialization
dyb2NylonFilmOpticalModel::dyb2NylonFilmOpticalModel (G4String modelName,
						      //  G4VPhysicalVolume* envelope_phys,
                                        G4Envelope* envelope)
  : G4VFastSimulationModel(modelName, envelope)
  , _verbosity(0) 

{
  // G4LogicalVolume* envelope_log= envelope_phys->GetLogicalVolume();
}

// destructor
dyb2NylonFilmOpticalModel::~dyb2NylonFilmOpticalModel ()
{
  // nothing to delete 
  // Note: The "MaterialPropertyVector"s are owned by the material, not us.
}


// IsApplicable() method overriding virtual function of G4VFastSimulationModel
// returns true if model is applicable to given particle.
// -- see also Geant4 docs
G4bool
dyb2NylonFilmOpticalModel::IsApplicable(const G4ParticleDefinition &particleType)
{

  return ( &particleType == G4OpticalPhoton::OpticalPhotonDefinition() );
}

// ModelTrigger() method overriding virtual function of G4VFastSimulationModel
// returns true if model should take over this specific track.
// -- see also Geant4 docs
G4bool
dyb2NylonFilmOpticalModel::ModelTrigger(const G4FastTrack &)
{
  // Any optical photon which comes to this FSMR should trigger the model.

  /*
  #if G4VERSION_NUMBER >= 900
        G4double kCarTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
        // By testing: kCarTolerance == 1.e-12*m
  #endif
  */

  return true; 
}


// DoIt() method overriding virtual function of G4VFastSimulationModel
// does the fast simulation for this track.  It is basically a faster but
// complete tracking code for the two-volume case.  It is a monster.
// -- see also Geant4 docs and comments below
void
dyb2NylonFilmOpticalModel::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep)
{
  //JB WEI:
  if (_verbosity >= 2)
    G4cout << "dyb2nylonfilmopticalmodel: Change the track's direction and position." << G4endl;

  // Update track's direction.
  G4ThreeVector direction(0.0,0.0,0.0);
  direction = fastTrack.GetPrimaryTrackLocalDirection();
  direction = CalculateNewDirection(direction);
  fastStep.SetPrimaryTrackFinalMomentum( direction );


  // Update track's position to the outside of FSMR.
  // Ref. to examples/novice/N05/src/ExN05PiModel.cc.
  G4ThreeVector position(0.0,0.0,0.0);
  G4double distance;
  distance = fastTrack.GetEnvelopeSolid()->
    DistanceToOut(fastTrack.GetPrimaryTrackLocalPosition(),
                  fastTrack.GetPrimaryTrackLocalDirection());
  position = fastTrack.GetPrimaryTrackLocalPosition() + 
    distance*fastTrack.GetPrimaryTrackLocalDirection();
  if (_verbosity >= 2){
    G4cout << "fastTrack.GetEnvelopeSolid() = " << fastTrack.GetEnvelopeSolid()->GetName() << G4endl;
    G4cout << "distance2out: " << distance << G4endl;

  }

  fastStep.SetPrimaryTrackFinalPosition( position );

  
  return;
}

// * NEW by Jiangbo WEI - 11/18/2013  23:58:29 
// CalculateNewDirection() method sample a delta angle according a Gaussian distribution,
// and rotate the input direction along one of its perpendicular axis.
// Then ,return the new direction.
G4ThreeVector dyb2NylonFilmOpticalModel::CalculateNewDirection(G4ThreeVector v3MomDir_Ori)
{
  // Only save for case of oblique incidence:
  //LSExpAnalysisManager::getInstance()->SetIncLightDir(v3MomDir_Ori);

  // Sampling according a gauss
  // Non-transparent: reflection + absorption. Need to get from normal incidence simulation.
  const G4double ratio_nonTransparent = 0.19/100.0; // 0.135% reflection + 0.055% absorption&others.
  // ratio_exp: transparent ratio within 2.5*degree. Get from experiments.
  const G4double ratio_exp = m_transparency;
  G4double ratio_gaus = ratio_exp/(1.0-ratio_nonTransparent);
  static G4double sigma = 0.0;
  if(sigma == 0.0)
  {
    sigma = TMath::DegToRad()*CalculateGaussianVariance(ratio_gaus,-2.5,2.5,0.0);
    if(_verbosity>=2)
      std::cout << "Fast Simulation Model: sigma = " << sigma*TMath::RadToDeg() << ",  ratio_gaus = " << ratio_gaus << std::endl;
  }
  // * NEW: 2013.09.12 by Jiangbo WEI
  G4double deltaAng = CLHEP::RandGauss::shoot(0.,sigma)*rad;
  // Save the initial input scattering angle:
  //LSExpAnalysisManager::getInstance()->SetInputScatteringAngle(deltaAng);
  

  // Sample an arbitrary rotating axis which is perpendicular with original momentum:
  G4ThreeVector rotateAxis(0.,0.,0.);
  for(int ii=0; ii<1; )
  { // To make sure we get a non-parallel rotating axis:
    G4double random[3] = {0.,0.,0.};
    for(int i=0; i<3; i++) random[i] = G4UniformRand()-0.5;
    G4ThreeVector randV(random[0],random[1],random[2]);
    if(randV != v3MomDir_Ori)
    {
      rotateAxis = randV.cross(v3MomDir_Ori);
      break;
    }
  }

  if (_verbosity >= 2)
  {
    G4cout << " Its transparent ratio = " << m_transparency*100 << "%" << G4endl; 
    G4cout << "v3MomDir_Ori: " << v3MomDir_Ori << G4endl;
  }
  // Rotate original direction along the rotating axis:
  G4ThreeVector v3MomDir_New = v3MomDir_Ori.rotate(rotateAxis,deltaAng);
  // Only save for case of oblique incidence:
  //LSExpAnalysisManager::getInstance()->SetOutLightDir(v3MomDir_New);
  if (_verbosity >= 2)
    G4cout << "v3MomDir_New: " << v3MomDir_New << G4endl;

  return v3MomDir_New;
}



// * NEW by Jiangbo WEI - 11/18/2013  23:58:29 
// Function CalculateGaussianVariance() needs to be improved!
// G4double dyb2NylonFilmOpticalModel::CalculateGaussianVariance
// ( G4double probability, G4double xlow, G4double xup, G4double mean )
// {
//   G4double sigma = 0.; 

//   if(mean>=xlow && mean<=xup && probability >= 8.0e-7){
//     const long long int N = static_cast<long long int>(1.e11);
//     const G4double sigmaLow = (xup-xlow)/(2.0*6);
//     // Because TF1::Integral(-6*sigma,6*sigma) = 1 for Gaussian function.
//     const G4double sigmaUp  = 1.0e6*(xup-xlow)/2.0;
//     // Because TF1::Integral(-sigma/1.e6,sigma/1.e6) = 8.0e-7, which means 
//     // the minimum probability resolution is about 0.0001%.
//     const G4double step = (sigmaUp - sigmaLow)/N;
 
//     TF1 pdf_Gaus("pdf_Gaus","gausn(0)",xlow,xup);
//     G4double tmpMin = 99999999.;
//     for(long long int i=0; i<N; i++)
//     {   
//       sigma = sigmaLow + i*step;
//       pdf_Gaus.SetParameters(1.0,mean,sigma+step);
//       G4double tmp = fabs( pdf_Gaus.Integral(xlow,xup) - probability );
//       if ( tmp <= tmpMin ) tmpMin = tmp;
//       else{
//         if(_verbosity>=2)
//           std::cout << "CalculateGaussianVariance(): Successfully find the standard deviation!" << std::endl;
//         break;
//       }   
//     }   
//   }else{
//       std::cout << "ERROR: CalculateGaussianVariance() - Sorry! I'm not a ready for your requirements!" << std::endl;
//       return EXIT_FAILURE;
//   }

//   return sigma;
// }


//improved by siguang wang to replace Jiangbo WEI which is slow
G4double dyb2NylonFilmOpticalModel::CalculateGaussianVariance( G4double probability, G4double xlow, G4double xup, G4double mean)
{
  // to perform quick calculate with fitted function in -2.5 to 2.5 range....
  if(probability<0.975 && probability>0.4){
    if(fabs(xlow + 2.5)<1E-7 && fabs(xup-2.5)<1E-7){
      G4double par[8];
      par[0]=55.4256415955;
      par[1]=-486.405020017;
      par[2]=2120.52641023;
      par[3]=-5335.84180109;
      par[4]=8129.51043031;
      par[5]=-7425.31481136;
      par[6]=3748.84388057;
      par[7]=-805.827533548;

      G4double tmpSigma = 0;
      G4double curXn = 1;
      for(int i=0; i<8; i++){
	if(i>=1) curXn *= probability;
	tmpSigma += par[i]*curXn;
      }
      return tmpSigma;
    }
  }

  //to perform number calcualte---------


  G4double sigma = 0.; 
  G4double sigmaLow = (xup-xlow)/(2.0*6);
  // Because TF1::Integral(-6*sigma,6*sigma) = 1 for Gaussian function.
  G4double sigmaUp  = 1.0e6*(xup-xlow)/2.0;
  // Because TF1::Integral(-sigma/1.e6,sigma/1.e6) = 8.0e-7
  
  if(!(mean>=xlow && mean<=xup && probability >= 8.0e-7)) {
    std::cout << "ERROR: CalculateGaussianVariance() - Sorry! I'm not a ready for your requirements!" << std::endl;
    return EXIT_FAILURE;
  }
  if(probability>1) {
    std::cout << "ERROR: probability can not > 1, take as 1" << std::endl;
    return sigmaLow; //to make sure a probability >1 case which is wrong
  }
  if(probability==1) {
    return sigmaLow; //to make sure 6 sigma in the range
  }

  G4double step = 0.3*sigmaLow;
  sigma = sigmaLow;
  G4double rangeH = sigmaUp;

  TF1 pdf_Gaus("pdf_Gaus","gausn(0)",xlow,xup);
  // int n1 = 0;
  for(int iloop=0; iloop<100; iloop++){
    while(sigma < rangeH){
      pdf_Gaus.SetParameters(1.0,mean,sigma);
      G4double tmpInt = pdf_Gaus.Integral(xlow,xup);
      G4double tmp = tmpInt - probability;
      //   n1++;
      //   printf("sigma=%f tmp = %f n1=%d tmpInt=%f rangeH=%f\n",sigma,tmp,n1,tmpInt,rangeH);

      if(tmp<0){//sigma too small
	if(fabs(tmp)>1.E-10){
	  rangeH = sigma;
	  sigma = sigma - step*2;
	  step = step / 10;
	}else{
	  // printf("sigma=%g tmp = %g n1=%d tmpInt=%f\n",sigma,tmp,n1,tmpInt);
	  return sigma;
	}
      }else{
	sigma = sigma + step;
      }
    }
  }
  return sigma;
}


