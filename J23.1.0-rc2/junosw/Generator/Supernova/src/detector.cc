#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include "detector.h"
using namespace std;
using namespace ROOT::Math;

//C-12 cross sections data
double inputenu[17] = {16, 18, 20, 22, 24, 26, 28, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100};
double sigman12[17] = {0, 0.036, 0.287, 0.772, 1.49, 2.44,3.62, 5.03, 9.47, 15.1, 21.8, 29.2, 45.2, 60.8, 74.2, 84.2, 90.6};
double sigmab12[17] = {0.086, 0.327, 0.711, 1.23, 1.87, 2.62, 3.48, 4.42, 7.10, 10.1, 13.2, 16.4, 22.2, 27.0, 30.5, 32.8, 34.2};
double sigmac12nu[17] = {0.010, 0.106, 0.302, 0.599, 0.994, 1.49, 2.07, 2.74, 4.78, 7.26, 10.1, 13.1, 19.5, 25.4, 30.2, 33.7, 35.8};
double sigmac12nubar[17] = {0.0095, 0.099, 0.279, 0.547, 0.896, 1.32, 1.82, 2.38, 4.03, 5.95, 8.03, 10.2, 14.4, 17.9, 20.7, 22.5, 23.6};


TDetector::TDetector(double mass, double hRatio)
{
    vector <double> x, y;
    hnum = mass * hRatio * 5.978637932680474e32;
    cnum = mass * (1 - hRatio) * 5.018419078000822e31;

    //Cross sections of C-12 are all interpolated
    x.push_back(17.338);
    y.push_back(0.0);
    for(int i = 1; i < 17; i++)
    {
        x.push_back(inputenu[i]);
        y.push_back(sigman12[i]);
    }
    n12 = new Interpolator(x, y);
    x.clear();
    y.clear();

    x.push_back(14.391);
    y.push_back(0.0);
    for(int i = 0; i < 17; i++)
    {
        x.push_back(inputenu[i]);
        y.push_back(sigmab12[i]);
    }
    b12 = new Interpolator(x, y);
    x.clear();
    y.clear();

    x.push_back(15.11);
    y.push_back(0.0);
    for(int i = 0; i < 17; i++)
    {
        x.push_back(inputenu[i]);
        y.push_back(sigmac12nu[i]);
    }
    c12nu = new Interpolator(x, y);
    x.clear();
    y.clear();

    x.push_back(15.11);
    y.push_back(0.0);
    for(int i = 0; i < 17; i++)
    {
        x.push_back(inputenu[i]);
        y.push_back(sigmac12nubar[i]);
    }
    c12nubar = new Interpolator(x, y);
    x.clear();
    y.clear();
}

TDetector::~TDetector()
{
    delete n12;
    delete b12;
    delete c12nu;
    delete c12nubar;
}

//input: eNu [MeV]
//output: dSigma/dCosTheta [cm^2]
//A. Strumia, Phys.Lett.B 564(2003)42-54
//not P.Vogel Phys.Rev.D 60 053003
double TDetector::GetIBD(double eNu, double costheta)
{
    double mP = 938.272046;
    double mN = 939.565379;
    double mE = 0.510998928;
    //double mPi = 139.57018; 

    //reaction threshold
    double Ethr = (pow(mN+mE,2)-mP*mP)/(2*mP);
    if(eNu<Ethr) return 0.0;


    //Ee
    double M     = (mP+mN)/2;
    double Delta = mN-mP;
    double eps   = eNu/mP;
    double delta = (mN*mN-mP*mP-mE*mE)/(2*mP); 
    double k     = pow(1+eps,2)-pow(eps*costheta,2);
    double Ee = ( (eNu-delta)*(1+eps) + eps*costheta*sqrt(pow((eNu-delta),2)-mE*mE*k) )/k;

    //1GeV=(1.9733e-14cm)^-1
    double GF2cos2pi = 1.613256e-44;//cm^2
    double s = mP*mP+2*mP*eNu;
    double u  = s-2*mP*(eNu+Ee)+mE*mE;
    double t = mN*mN-mP*mP-2*mP*(eNu-Ee);

    double xi = 3.706;
    double f1 = 1+2.5*t*1e-6;
    double f2 = xi*(1+3.4*t*1e-6);
    double g1 = -1.270/pow(1-t*1e-6,2);
    //double g2 = 2*M*M*g1/(mPi*mPi-t);


    //|M^2|
    double A = M*M*(f1*f1-g1*g1)*(t-mE*mE)
        -M*M*Delta*Delta*(f1*f1+g1*g1)
        -2*mE*mE*M*Delta*g1*(f1+f2);
    double B = t*g1*(f1+f2);
    double C = (f1*f1+g1*g1)/4;
    double Mat = A-(s-u)*B+(s-u)*(s-u)*C;

    //dsigma/dt, dsigma/dE
    double dsigdt = Mat*GF2cos2pi/(2*pow(s-mP*mP,2)); 
    double dsigdE = 2*mP*dsigdt;

    //dsigma/dcostheta
    double pe = sqrt(Ee*Ee-mE*mE);
    double dsigdcos = dsigdE*pe*eps/(1+eps*(1-Ee/pe*costheta));
    return hnum*dsigdcos;
}
////input: eNu [MeV]
////output: dSigma/dCosTheta [cm^2]
////Not using Formula: arXiv:astro-ph/0302055
////Using JUNO KRLInverseBeta.cc 
//double TDetector::GetIBD(double eNu, double costheta)
//{
//	if(eNu < 1.806)
//	{
//		return 0.0;
//	}
//	double mP = 938.272046;
//	double mN = 939.565379;
//	double mE = 0.510998928;
//
//  double fF = 1;
//  double fG = 1.26;
//  double fF2 = 3.706;
//  double fDelta = mN - mP;
//  double fYsq = (fDelta*fDelta - mE * mE)/2;
//  double fF2Plus3G2 = fF*fF + 3*fG*fG;
//  double fF2MinusG2 = fF*fF - fG*fG;
//  double fSigma0 = 0.0952/(fF2Plus3G2); // *10^{-42} cm^2, eqn 9
//  double fE0 = eNu - fDelta;
//  double fP0 = sqrt(fE0 * fE0 - mE * mE);
//  double fV0 = fP0 / fE0;
//  double sqBracket = 1 - eNu * (1 - fV0 * costheta) / mP;
//
//  double firstLine = (2*fE0+fDelta)*(1-fV0*costheta)-mE*mE/fE0;
//  firstLine *= (2*(fF+fF2)*fG);
//  double secondLine = fDelta*(1+fV0*costheta) +mE*mE/fE0;
//  secondLine *= (fF*fF+fG*fG);
//  double thirdLine = fF2Plus3G2 *( (fE0+fDelta)*(1-costheta/fV0) - fDelta);
//  double fourthLine = (fF*fF - fG*fG)*fV0*costheta;
//  fourthLine *= ( (fE0+fDelta)*(1-costheta/fV0) - fDelta );
//
//  double GammaTerm = firstLine + secondLine + thirdLine + fourthLine; 
//	double eE = fE0 * sqBracket - fYsq / mP;
//	
//	if(eE < mE)
//	{
//		return 0.0;
//	}
//
//	double pE = sqrt(eE * eE - mE * mE);
//	double vE = pE / eE;
//
//	return hnum * 1e-42 * ((fSigma0/2) * (fF2Plus3G2 + fF2MinusG2*vE*costheta)* eE*pE
//		- fSigma0 * GammaTerm * fE0 * fP0 / (2* mP));
///*	double costheta = (mN * mN - mE * mE - mP * mP + 2 * eNu * (eE - mP) + 2 * mP * eE) 
//			/ (2 * eNu * sqrt(eE * eE - mE * mE));
//*/
//
//
///*	return hnum * 1e-43 * (eNu-1.294) * sqrt((eNu-1.294)*(eNu-1.294) - 0.511*0.511) *
//		pow(eNu, - 0.07056 + 0.02018 * log(eNu) -
//		0.001953 * pow(log(eNu), 3));
//*/
//}

//input: eNu [MeV], eP (actually Tp) [MeV]
//output: dSigma/dTp [cm^2 MeV^{-1}]
//Formula: Phys. Rev. D 35 (1987) 785, http://theory.lngs.infn.it/astroparticle/sn.html
double TDetector::GetProtonES(double eNu, double eP, bool isAnti)
{
    //nu-P scattering parameter
    double sw2 = 0.23126;
    double hbarc = 1.973269718e-11;	//Mev * cm
    double gf = 1.1663787e-11;	//Mev^2
    double mp = 938.272046;	//MeV
    double mvv = 840.0;	//MeV
    double kappap = 1.792847356;
    double kappan = -1.9130427;
    //double mav = 1032.0;	//MeV
    double ga = -1.267;
    double alpha = 1.0 - 2.0 * sw2;
    double beta = -1.0;
    double gamma = - 2.0 / 3.0 * sw2;
    double constt = mp * pow(gf * hbarc, 2) / M_PI;
    //Input
    double eps = eNu / mp;
    double tau = eP / 2.0 / mp;
    if(eps <= 0.0 || tau <= 0.0 || tau > pow(eps, 2) / (1 + 2 * eps))
    {
        return 0.0;
    }

    double qf3v = 0.5 * (kappap - kappan) / ((1 + tau) * pow(1 + 4 * tau * pow(mp / mvv, 2), 2));
    double qf0v = 1.5 * (kappap + kappan) / ((1 + tau) * pow(1 + 4 * tau * pow(mp / mvv, 2), 2));
    double qg3v = 0.5 * (kappap - kappan + 1) / pow(1 + 4 * tau * pow(mp / mvv, 2), 2);
    double qg0v = 1.5 * (kappap + kappan + 1) / pow(1 + 4 * tau * pow(mp / mvv, 2), 2);

    double qf2 = alpha * qf3v + gamma * qf0v;
    double qf1 = alpha * qg3v + gamma * qg0v - qf2;

    double qga3 = 0.5 * ga / pow(1 + 4 * tau * pow(mp / mvv, 2), 2);
    double qg = qga3 * beta;

    double funca = 4 * tau * qf1 * qf2 
        + (1 - tau) * (tau * pow(qf2, 2) - pow(qf1, 2)) + (1 + tau) * pow(qg, 2);
    double funcb = 4 * qg * (qf1 + qf2);
    double funcc = pow(qf1, 2) + tau * pow(qf2, 2) + pow(qg, 2);

    return hnum * constt * (pow(eps - tau, 2) * funcc 
            + (isAnti ? -1 : 1) * (eps - tau) * tau * funcb 
            + tau * funca) / pow(eps, 2);
}


double TDetector::GetTotalProtonES(double eNu, bool isAnti)
{
    double mp = 938.272046;
    double top = 2 * mp * pow(eNu / mp, 2) / (1 + 2 * eNu / mp);
    double delta = top / 100.0;
    double result = 0.0;
    for(double eP = 0.0; eP < top; eP += delta)
    {
        result += GetProtonES(eNu, eP, isAnti);
    }
    result *= delta;
    return result;
}
//input: eNu [MeV], eE [MeV]
//output: dSigma/dEe [cm^2 MeV^{-1}]
//Formula: Mod.Phys.Lett. A8 (1993) 1067-1088
double TDetector::GetElectronES(double eNu, double eE, NuType type)
{
    //nu-E scatter parameters
    double hbarc = 1.973269718e-11; //Mev * cm
    double G_F = 1.1663787e-11;	//MeV^-2
    double g_L = -0.26845;	//-0.5 + SW2
    double g_R = 0.23155;	//SW2
    double mE = 0.510998928;	//MeV
    //Input
    double y = eE / eNu;
    double x = mE / eNu;
    if(eNu <= 0.0 || eE < mE || y > x + 1.0 / (1.0 + 0.5 * x))
    {
        return 0.0;
    }

    switch(type)
    {
        case nuE:
            return pow(hbarc, 2) * (hnum + cnum * 6) * 2 * pow(G_F, 2) * mE / M_PI 
                * (pow(g_L + 1, 2) + pow(g_R, 2) * pow(1 - y, 2) 
                        - (g_L + 1) * g_R * mE * y / eNu);
        case nuAE:
            return pow(hbarc, 2) * (hnum + cnum * 6) * 2 * pow(G_F, 2) * mE / M_PI 
                * (pow(g_R, 2) + pow(g_L + 1, 2) * pow(1 - y, 2) 
                        - (g_L + 1) * g_R * mE * y / eNu);
        case nuX:
            return pow(hbarc, 2) * (hnum + cnum * 6) * 2 * pow(G_F, 2) * mE / M_PI 
                * (pow(g_L, 2) + pow(g_R, 2) * pow(1 - y, 2) 
                        - g_L * g_R * mE * y / eNu);
        case nuAX:
            return pow(hbarc, 2) * (hnum + cnum * 6) * 2 * pow(G_F, 2) * mE / M_PI 
                * (pow(g_R, 2) + pow(g_L, 2) * pow(1 - y, 2) 
                        - g_L * g_R * mE * y / eNu);
    }

    return 0;
}

double TDetector::GetTotalElectronES(double eNu, NuType type)
{
    double mE = 0.510998928;
    double top = mE + eNu / (1 + 0.5 * mE / eNu);
    double delta = top / 100.0;
    double result = 0.0;
    for(double eE = mE; eE < top; eE += delta)
    {
        result += GetElectronES(eNu, eE, type);
    }
    result *= delta;
    return result;
}
//input: eNu [MeV]
//output: sigma [cm^2]
//Formula: Phys.Lett. B212 (1988) 139
double TDetector::GetN12(double eNu)
{
    return eNu < 17.339 ? 0 : cnum * 0.989 * n12->Eval(eNu) * 1e-42;
}

//input: eNu [MeV]
//output: sigma [cm^2]
//Formula: Phys.Lett. B212 (1988) 139
double TDetector::GetB12(double eNu)
{
    return eNu < 14.392 ? 0 : cnum * 0.989 * b12->Eval(eNu) * 1e-42;
}

//input: eNu [MeV]
//output: sigma [cm^2]
//Formula: Phys.Lett. B212 (1988) 139
double TDetector::GetC12(double eNu, bool isAnti)
{
    if(eNu <= 15.11)
    {
        return 0.0;
    }

    if(!isAnti)
    {
        return cnum * 0.989 * c12nu->Eval(eNu) * 1e-42;
    }
    else
    {
        return cnum * 0.989 * c12nubar->Eval(eNu) * 1e-42;
    }
}
