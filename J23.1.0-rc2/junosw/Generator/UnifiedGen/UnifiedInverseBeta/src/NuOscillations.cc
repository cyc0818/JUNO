#include "NuOscillations.hh"
# include "CommonXSec/ReadOscParam.hh"
# include <stdio.h>
# include <iostream>
# include <string>
# include <map>


NuOscillations *gNuOscillations;

NuOscillations::NuOscillations(Bool_t NH)
{

  L = 5.25e4; // in meters
  gReader -> GenerateOscParamMap();
  // from PDG 2020
  sin2_th12 = gReader->GetOscParamMap("SinSqTheta12");
  Dm2_21 = gReader->GetOscParamMap("DeltaM21Sq"); // in eV2
  sin2_th13 = gReader->GetOscParamMap("SinSqTheta13");
  if(NH) {
    Dm2_32 = gReader->GetOscParamMap("DeltaM32Sq_NormalHierarchy");
  } 
  else {
    Dm2_32 = gReader->GetOscParamMap("DeltaM32Sq_InvertedHierarchy");
  }
  Dm2_31 = Dm2_32 + Dm2_21;

}

void NuOscillations::SetBaseLine_m(Double_t val)
{
  L = val;
  printf("Base line is manually set %f m\n", L);
}

void NuOscillations::SetSin2Th12(Double_t val)
{
  sin2_th12 = val;
  printf("sin2_th12 is manually set %f \n", sin2_th12);
}

void NuOscillations::SetSin2Th13(Double_t val)
{
  sin2_th13 = val;
  printf("sin2_th13 is manually set %f \n", sin2_th13);
}

void NuOscillations::SetDm2_21(Double_t val)
{
  Dm2_21 = val;
  printf("Dm2_21 is manually set %f \n", Dm2_21);
  printf("Dm2_21 = %f \n", Dm2_21);
  printf("Dm2_31 = %f \n", Dm2_31);
  printf("Dm2_32 = %f \n", Dm2_32);
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
}


Double_t NuOscillations::SurvivalProb0(Double_t E)
{
//  printf("%f\n", E);
//  printf("%f, %f, %f \n", Dm2_21, Dm2_31, Dm2_32);
  cos2_th12 = 1. - sin2_th12;
  sin2_2th12 = 4.*sin2_th12*cos2_th12;
  cos2_th13 = 1. - sin2_th13;
  sin2_2th13 = 4.*sin2_th13*cos2_th13;
  cos4_th13 = TMath::Power(cos2_th13, 2);
  Double_t t1 = sin2_2th12 * cos4_th13 * TMath::Power(TMath::Sin(1.267 * Dm2_21 * L / E), 2);
  Double_t t2 = sin2_2th13 * cos2_th12 * TMath::Power(TMath::Sin(1.267 * Dm2_31 * L / E), 2);
  Double_t t3 = sin2_2th13 * sin2_th12 * TMath::Power(TMath::Sin(1.267 * Dm2_32 * L / E), 2);
  Double_t ndProb = 1. - t1 - t2 - t3;
  return ndProb;
}


Double_t NuOscillations::SurvivalProb(Double_t E)
{
  //Matter effect
  double MassOfe_kg = 9.1093837015e-31; //kg
  double Rho_Rock = 2.45e3; //kg/m^3
  double MassOfPronton_kg = 1.67262192369e-27; //kg
  double N_e = Rho_Rock / (MassOfe_kg + MassOfPronton_kg) / 2.;
  double G_F = 1.1663787e-5; //GeV^-2
  double hbar_C = 197.3269804;
  Double_t matterdensity = 2.45e3; //kg/m^3
  Double_t A = -2 * sqrt(2) * E * G_F * N_e * hbar_C * hbar_C * hbar_C * 1e-39; //to eV^2, times (hbar c)^3/10^39
  Double_t Delta_c = Dm2_31*(1-sin2_th12)+Dm2_32*sin2_th12;
  Double_t alpha_c = Dm2_21/Delta_c;
  Double_t A_star = A*(1-sin2_th13)/Dm2_21;
  Double_t A_c = A/Delta_c;
  Double_t cos_2th12 = 1-2.*sin2_th12;
  Double_t cos_2th13 = 1-2.*sin2_th13;
  Double_t C_hat_12_prime = sqrt(1+2.0*A_star*cos_2th12+A_star*A_star);
  Double_t C_hat_13_prime = sqrt(1+2.0*A_c*cos_2th13+A_c*A_c);

  Double_t cos2_th12_tilde = 0.5*(1+(A_star+cos_2th12)/C_hat_12_prime);
  Double_t cos2_th13_tilde = 0.5*(1+(A_c+cos_2th13)/C_hat_13_prime);

  Double_t sin2_th13_mat = 1-cos2_th13_tilde;
  Double_t sin2_2th13_mat = 4*sin2_th13_mat*cos2_th13_tilde;
  Double_t sin2_th12_mat = 1-cos2_th12_tilde;
  Double_t sin2_2th12_mat = 4*sin2_th12_mat*cos2_th12_tilde;

  Double_t Dm2_21_mat;
  Double_t Dm2_31_mat;

  if (Dm2_31>0){
    Dm2_21_mat = Delta_c * (0.5 * (1 - A_c - C_hat_13_prime) + alpha_c * (C_hat_12_prime + A_star));
    Dm2_31_mat = Delta_c * (0.5 * (1 - A_c + C_hat_13_prime) + alpha_c * 0.5 * (C_hat_12_prime + A_star - cos_2th12));
  }
  else{
    Dm2_21_mat = Delta_c * (-0.5 * (1 - A_c - C_hat_13_prime) + alpha_c * (C_hat_12_prime - A_star));
    Dm2_31_mat = Delta_c * (C_hat_13_prime + alpha_c * 0.5 * (C_hat_12_prime - A_star - cos_2th12));
  }
  Double_t Dm2_32_mat = Dm2_31_mat - Dm2_21_mat;

  Double_t t1 = sin2_2th12_mat * pow((1-sin2_th13_mat),2) * TMath::Power(TMath::Sin(1.267 * Dm2_21_mat * L / E), 2);
  Double_t t2 = sin2_2th13_mat * (1-sin2_th12_mat) * TMath::Power(TMath::Sin(1.267 * Dm2_31_mat * L / E), 2);
  Double_t t3 = sin2_2th13_mat * sin2_th12_mat * TMath::Power(TMath::Sin(1.267 * Dm2_32_mat * L / E), 2);
  Double_t ndProb = 1. - t1 - t2 - t3;
  return ndProb;
}
