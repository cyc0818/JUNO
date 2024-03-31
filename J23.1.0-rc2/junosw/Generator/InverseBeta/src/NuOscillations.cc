#include "NuOscillations.hh"
#include <TRandom.h>

NuOscillations *gNuOscillations;

NuOscillations::NuOscillations(Bool_t NH)
{
  L = 5.25e4; // in meters
  // from PDG 2016
  sin2_th12 = 0.297;
  Dm2_21 = 7.37e-5; // in eV2
  Double_t Dm2;  // Dm2 = m_3^2 - (m_2^2 + m_1^3)/2
  if(NH) {
    sin2_th13 = 0.0214;
    Dm2 = 2.50e-3;
  } 
  else {
    sin2_th13 = 0.0218;
    Dm2 = -2.46e-3;
  }
  Dm2_31 = Dm2 + Dm2_21/2.;
  Dm2_32 = Dm2 - Dm2_21/2.;

  UpdateParameters();
}

void NuOscillations::SetBaseLine_m(Double_t val)
{
  L = val;
  printf("Base line is manually set %f m\n", L);
  UpdateParameters();
}

void NuOscillations::SetSin2Th12(Double_t val)
{
  sin2_th12 = val;
  printf("sin2_th12 is manually set %f \n", sin2_th12);
  UpdateParameters();
}

void NuOscillations::SetSin2Th13(Double_t val)
{
  sin2_th13 = val;
  printf("sin2_th13 is manually set %f \n", sin2_th13);
  UpdateParameters();
}

void NuOscillations::SetDm2_21(Double_t val)
{
  Dm2_21 = val;
  printf("Dm2_21 is manually set %f \n", Dm2_21);
  printf("Dm2_21 = %f \n", Dm2_21);
  printf("Dm2_31 = %f \n", Dm2_31);
  printf("Dm2_32 = %f \n", Dm2_32);
  UpdateParameters();
}

void NuOscillations::SetAbsDm2_31(Double_t val)
{
  Double_t absDm2_31 = val;
  if(NH) {
    Dm2_31 = absDm2_31;
    Dm2_32 = Dm2_31 - Dm2_21;
  }
  else {
    Dm2_31 = -1.*absDm2_31;
    Dm2_32 = Dm2_31 - Dm2_21;
  }
  printf("|Dm2_31| is manually set %f \n", absDm2_31);
  printf("Dm2_21 = %f \n", Dm2_21);
  printf("Dm2_31 = %f \n", Dm2_31);
  printf("Dm2_32 = %f \n", Dm2_32);
  UpdateParameters();
}

void NuOscillations::UpdateParameters()
{
  cos2_th12 = 1. - sin2_th12;
  sin2_2th12 = 4.*sin2_th12*cos2_th12;
  cos2_th13 = 1. - sin2_th13;
  sin2_2th13 = 4.*sin2_th13*cos2_th13;
  cos4_th13 = TMath::Power(cos2_th13, 2);
//  printf("%f, %f, %f, %f, %f, %f \n", L, sin2_th12, sin2_th13, Dm2_21, Dm2_31, Dm2_32);
//  printf("%f, %f, %f, %f, %f \n", cos2_th12, sin2_2th12, cos2_th13, sin2_2th13, cos4_th13);
//  printf("%f, %f, %f\n\n", sin2_2th12 * cos4_th13,  sin2_2th13 * cos2_th12, sin2_2th13 * sin2_th12);

}

Double_t NuOscillations::SurvivalProb(Double_t E)
{
//  printf("%f\n", E);
//  printf("%f, %f, %f \n", Dm2_21, Dm2_31, Dm2_32);
  Double_t t1 = sin2_2th12 * cos4_th13 * TMath::Power(TMath::Sin(1.267 * Dm2_21 * L / E), 2);
  Double_t t2 = sin2_2th13 * cos2_th12 * TMath::Power(TMath::Sin(1.267 * Dm2_31 * L / E), 2);
  Double_t t3 = sin2_2th13 * sin2_th12 * TMath::Power(TMath::Sin(1.267 * Dm2_32 * L / E), 2);
  Double_t ndProb = 1. - t1 - t2 - t3;
  return ndProb;
}
