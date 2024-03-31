//to do review
//------------------------------------------
// Simplify QMLE alg for fast reconstruction
// Author: Guihong Huang
// Creation time: 2022.12.09 
//------------------------------------------

#include "SQMLE.h"
#include "SniperKernel/ToolFactory.h"


DECLARE_TOOL(SQMLE);

SQMLE::SQMLE(const std::string &name): ToolBase(name){
    fcn = NULL;
    recminuit = NULL;
    for(int i=0;i<ThetaPMTNum;i++) {
        LMu1D[i] = NULL;
    }
}

SQMLE::~SQMLE(){
    if(fcn!=NULL) delete fcn;
    if(recminuit!=NULL) delete recminuit;
    for(int i=0;i<ThetaPMTNum;i++) {
        if(LMu1D[i]!=NULL)  delete LMu1D[i];
    }
}


bool SQMLE::configure(){
    std::string base = getenv("JUNOTOP");
    if(getenv("WORKTOP")) base = getenv("WORKTOP");

    std::string LnPEMapPath  = base + "/data/Reconstruction/OMILREC/RecMap/nPEMap";

    TFile* LnPEMapFile = TFile::Open((LnPEMapPath + "/LnPEMapFile.root").c_str(), "READ");
    if(!LnPEMapFile) {
        LogInfo<<  LnPEMapPath + "/LnPEMapFile.root"  + "is not found!" << std::endl;
        LogInfo<<  "Turn to read JUNOTOP file!" << std::endl;
        delete LnPEMapFile;

        base = getenv("JUNOTOP");
        LnPEMapPath  = base + "/data/Reconstruction/OMILREC/RecMap/nPEMap";
        LnPEMapFile = TFile::Open((LnPEMapPath + "/LnPEMapFile.root").c_str(), "READ");
        if(!LnPEMapFile) {
            LogInfo<<  LnPEMapPath + "/LnPEMapFile.root"  + "is not found!" << std::endl;
            delete LnPEMapFile;
            return false;
        }
    }


    //read Ge-68 to n-H energy scale factor
    nHESF = 1.;
    std::ifstream ESFin;
    ESFin.open((LnPEMapPath + "/nHEScaleFactor.txt").c_str(), std::ios::in);
    if(ESFin.is_open()==true){
        ESFin>>nHESF;
        ESFin.close();
    } else {
        LogError<< "Can't find nHEScaleFactor.txt!" << std::endl;
    }

    for(int i=0;i<ThetaPMTNum;i++) {
        TH2F* h2dtemp = (TH2F*)LnPEMapFile->Get(Form("hLMu2D_%d", i));
        LMu1D[i] = (TH1D*)h2dtemp->ProjectionX(Form("hLMu1D_%d", i), 1, h2dtemp->GetYaxis()->GetNbins());
        LMu1D[i]->Scale(1./h2dtemp->GetYaxis()->GetNbins());
        LMu1D[i]->SetDirectory(nullptr);
        delete h2dtemp;
    }
    LnPEMapFile->Close();
    delete LnPEMapFile;
    LogDebug<< "LMu1D[ThetaPMTNum-1]->Interpolate(rrange[1], Pi/2) "
            << LMu1D[ThetaPMTNum-1]->Interpolate(179) << std::endl;

    fcn = new MyFCN1(this);
    recminuit = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");

    return true;
}

void SQMLE::rec(HitPMTVector &hitpmtvector, PMTTable& pmttable, RecInfo &recinfo){
    double qsum = 0;
    opmttable = &pmttable;

    double ChargePos_[3] = {0., 0., 0.};
    for (auto* thisPMT: pmttable){
        double qtemp = thisPMT->totq/thisPMT->gain;
        qtemp -= thisPMT->dnrate*ADCL/1.e9;
        qsum += qtemp;

        double pmt_pos_x = thisPMT->pos.X();
        double pmt_pos_y = thisPMT->pos.Y();
        double pmt_pos_z = thisPMT->pos.Z();

        ChargePos_[0] += pmt_pos_x*qtemp;
        ChargePos_[1] += pmt_pos_y*qtemp;
        ChargePos_[2] += pmt_pos_z*qtemp;
    }
    double QCx = 1.2*ChargePos_[0]/qsum;
    double QCy = 1.2*ChargePos_[1]/qsum;
    double QCz = 1.2*ChargePos_[2]/qsum;
    

    double QE = qsum/1500.;
    if(QE<1.e-8) QE = 1.;
    double QR = sqrt(QCx*QCx + QCy*QCy + QCz*QCz);
    double QEvtTheta = TMath::Pi()/180./100.;
    double QEvtPhi = 2.*TMath::Pi()/180./100.;

    if(QR>1.e-16) recTheta = TMath::ACos(QCz/QR);
    recPhi = TMath::ATan2(QCy, QCx);

    QEvtTheta = recTheta;
    QEvtPhi = recPhi;
    //CoorTransform(QCx, QCy, QCz,  QR, QEvtTheta, QEvtPhi);
    //recTheta = QEvtTheta;
    //recPhi = QEvtPhi;
  
    
    ROOT::Math::Functor recfunc1(*fcn, 2);

    recminuit->SetFunction(recfunc1);
    recminuit->SetMaxFunctionCalls(maxfuncalls);
    recminuit->SetMaxIterations(maxiters);
    recminuit->SetTolerance(tolerance);

    recminuit->SetLimitedVariable(0,"n", QE, estep, erange[0], erange[1]);
    recminuit->SetLimitedVariable(1, "rPos", 10000., rstep, rrange[0], rrange[1]);

    LogInfo<<"Start Minimization!"<<std::endl;
    int ief = recminuit->Minimize();

    const double *xs = recminuit->X();
    const double *xerr = recminuit->Errors();

    double rec_QE = xs[0];
    double rec_QR = xs[1];

    double Qx = rec_QR*TMath::Sin(QEvtTheta)*TMath::Cos(QEvtPhi);
    double Qy = rec_QR*TMath::Sin(QEvtTheta)*TMath::Sin(QEvtPhi);
    double Qz = rec_QR*TMath::Cos(QEvtTheta);

    TVector3 vertex(Qx, Qy, Qz);
    recinfo.vtx = vertex;
    recinfo.energy = rec_QE;
    LogDebug<<"QCVertex:"<<QCx<<'\t' << QCy<<'\t' << QCz <<'\t' << std::endl;
    LogDebug<<"QVertex:"<<rec_QE << '\t' << Qx<<'\t' << Qy<<'\t' << Qz <<'\t' << std::endl;
    
    recminuit->Clear();
    //LogDebug<<"Energy:"<<recinfo.energy<<"MeV"<<std::endl;
}

double SQMLE::Calculate_EVLikelihood(double E,double m_R) 
{
    double m_Likelihood = 0.;
    
    double m_x = m_R*TMath::Sin(recTheta)*TMath::Cos(recPhi);
    double m_y = m_R*TMath::Sin(recTheta)*TMath::Sin(recPhi);
    double m_z = m_R*TMath::Cos(recTheta);

    if(m_R>rrange[1]-1.e-8) m_R = rrange[1]-1.;
    if(isnan(m_R)) m_R = 10000.;
    double RVar = TMath::Power(m_R, 3.)*180./TMath::Power(rrange[1], 3.);

    double m_average_PE = 0;
    int krange = 1E3;
    double probPrcs = 1.e-4;

    for (auto* thisPMT: *opmttable){
        double qtemp = thisPMT->totq/thisPMT->gain;
        
        double pmt_pos_x = thisPMT->pos.X();
        double pmt_pos_y = thisPMT->pos.Y();
        double pmt_pos_z = thisPMT->pos.Z();

        double PMTR = TMath::Sqrt(pmt_pos_x*pmt_pos_x + pmt_pos_y*pmt_pos_y + pmt_pos_z*pmt_pos_z);
        double cos_theta = (pmt_pos_x*m_x + pmt_pos_y*m_y + pmt_pos_z*m_z)/m_R/PMTR;
        double theta = TMath::ACos(cos_theta)*180/TMath::Pi();
        if(isnan(theta)) theta = 90.0;

        int GrMapIdA = theta/MapThetaStep;

        // 1-D interpolation
        m_average_PE = LMu1D[GrMapIdA]->Interpolate(RVar);
        
        double m_expected_PE = E*m_average_PE/1.022/nHESF;

        double fdmu = thisPMT->dnrate*ADCL/1.e9;
        m_expected_PE += fdmu;

        double Pro_Qi = 1e-16;

        //double Q1 = thisPMT->gain;
        double Q1 = 1.;
        double S1 = 0.33;

        double proTemp = 0., proTemp_last = 0.;
        
        if(qtemp<PedThres) {
            double Poisson0 = exp(-m_expected_PE);
            Pro_Qi = Poisson0;
        }
        else {
            double PoissonProb = exp(-m_expected_PE);
            for(int k = 1;k<krange;k++) {
                double kk = double(k);
                proTemp = TMath::Gaus(qtemp, kk*Q1, sqrt(kk)*S1, true);

                if(proTemp<0.0) {proTemp = probPrcs/10.;}
                if(proTemp<probPrcs && proTemp_last>probPrcs) break;
                proTemp_last = proTemp;

                PoissonProb = PoissonProb*m_expected_PE/kk;
                Pro_Qi += PoissonProb*proTemp;
            }
        }
        //Pro_Qi = exp(-m_expected_PE);
        //if(qtemp>1.e-8) Pro_Qi = 1. - Pro_Qi;

        if(Pro_Qi<1e-16) Pro_Qi = 1e-16;
        m_Likelihood = m_Likelihood - 2.*log(Pro_Qi);
    }

    return m_Likelihood;
}

/*
void SQMLE::CoorTransform(const double evtx, const double evty, const double evtz, double& r, double& theta, double& phi)
{
    r = sqrt(evtx*evtx + evty*evty + evtz*evtz);

    if(r>1.e-16) theta = TMath::ACos(evtz/r);
    if(TMath::Abs(evtx)>1.e-16) {
        phi = TMath::ATan(evty/evtx);
        if(sgn(TMath::Sin(theta)*TMath::Cos(phi))!=sgn(evtx)) phi += TMath::Pi();
        if(phi<0.)  phi += 2.*TMath::Pi();
    }
}


double SQMLE::sgn(double x)
{
    if(x < -1.e-16) return -1;
    else if(x < 1.e-16) return 0;
    else return 1;

}
*/
