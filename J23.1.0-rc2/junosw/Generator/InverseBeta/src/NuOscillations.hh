// author: Yury Malyshkin 11/02/2016

#ifndef __NOOSCILLATIONS__H__
#define __NOOSCILLATIONS__H__

#include "TObject.h"
#include "TMath.h"

class NuOscillations : public TObject
{
  public:
    NuOscillations(Bool_t valNH=true);
    virtual ~NuOscillations() {;}
    void SetBaseLine_m(Double_t val);
    void SetSin2Th12(Double_t val);
    void SetSin2Th13(Double_t val);
    void SetDm2_21(Double_t val);
    void SetAbsDm2_31(Double_t val);
    void UpdateParameters();
    Double_t SurvivalProb(Double_t E);
  private:
    Double_t NH;
    Double_t L;
    Double_t sin2_th12;
    Double_t cos2_th12;
    Double_t sin2_2th12;
    Double_t sin2_th13;
    Double_t cos2_th13;
    Double_t sin2_2th13;
    Double_t cos4_th13;
    Double_t Dm2_21;
    Double_t Dm2_31;
    Double_t Dm2_32;

};

R__EXTERN NuOscillations *gNuOscillations;

#endif
