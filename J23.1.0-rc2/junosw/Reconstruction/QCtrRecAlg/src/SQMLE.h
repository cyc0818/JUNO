#ifndef QSUMTOOL_H
#define QSUMTOOL_H
#include "RecTools/IRecTool.h"
#include "Minuit2/FCNBase.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TFile.h"
#include "SniperKernel/ToolBase.h"
#include <vector>
#include <fstream>

const int ThetaPMTNum= 1440;
const double ADCL = 1000.;
static const int maxiters = 1E4;
static const int maxfuncalls = 1E4;
static const double tolerance = 1.e-6;
static const double erange[2] = {0., 1.e6}; //MeV
static const double rrange[2] = {0., 17400.}; // mm
static const double MapThetaStep = 180./1440.; //degree
static const double estep = 0.001; //MeV
static const double rstep = 10; // mm
static const double PedThres = 0.1; // pe


class SQMLE: public ToolBase, public IRecTool{
    public:
        SQMLE(const std::string &);
        virtual ~SQMLE();

        bool configure();
        void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);
        double Calculate_EVLikelihood(double, double);
        //double sgn(double x);
        //void CoorTransform(const double evtx, const double evty, const double evtz, double& r, double& theta, double& phi);

        class MyFCN1: public ROOT::Minuit2::FCNBase {
            public:
                MyFCN1(SQMLE *alg) {m_alg = alg;}
                double operator() (const std::vector<double>& x) const {
                    return m_alg->Calculate_EVLikelihood(x[0],x[1]);
                }
                double operator() (const double * x)const{
                    std::vector<double> p(x, x+2);
                    return (*this)(p);
                }

                double Up() const {return 0.5;}
            private:
                SQMLE *m_alg;
        };

    private:
        ROOT::Math::Minimizer* recminuit;
        MyFCN1* fcn;

        double recTheta;
        double recPhi;
        double recR;
        double recE;
        TH1D* LMu1D[ThetaPMTNum];
        double nHESF;
        PMTTable* opmttable;

};
#endif
