#include <iostream>
#include "TSystem.h"
#include "TMath.h"
#include "TComplex.h"
#include "TROOT.h"
#include <math.h>

gSystem->Load("libMathCore");
double me = 0.5109989;

//Fermi Function comes from    J. Phys. G: Nucl. Phys. 11 (1985) 359-364.

double Fermi(int Z, double Ek) {
     double  A = 0.073*Z + 0.94;
     double  a = 5.5465*10E-3;
     double  b = 76.929*10E-3;
     double  B = a*Z*exp(b*Z);
    
     return sqrt(A + B/(Ek + me - 1));
}

    
double beta (int Z, double Q, double Ek) {
return Fermi(Z,Ek)*pow(Q-Ek,2)*sqrt(Ek*Ek+2*Ek*me)*(Ek+me);
}   



