#ifndef timefitfunction
#define timefitfunction


using namespace std;

double TimeIntervalFunc(double* x, double* par){

    double NH = par[0];
    double NGd = par[1];
    double alpha = par[2];
    double tau_H = par[3];
    double tau_Gd = par[4];
    double tau_thr = par[5];
    double constant = par[6]/999;


    double H_term = (NH/tau_H)*exp(-x[0]/tau_H);
    double Gd_term = (NGd*(1+alpha)/tau_Gd)*exp(-x[0]/tau_Gd);
    double thr_term = (NGd*alpha/tau_thr)*exp(-x[0]/tau_thr);


    return H_term + Gd_term - thr_term + constant;

}



double GdLSComponent(double* x, double* par){

    double NGd = par[0];
    double alpha = par[1];
    double tau_Gd = par[2];
    double tau_thr = par[3];


    double Gd_term = (NGd*(1+alpha)/tau_Gd)*exp(-x[0]/tau_Gd);
    double thr_term = (NGd*alpha/tau_thr)*exp(-x[0]/tau_thr);


    return Gd_term - thr_term;



}




double LSComponent(double* x, double* par){

    double NH = par[0];
    double tau_H = par[1];


    return (NH/tau_H)*exp(-x[0]/tau_H);



}




#endif
