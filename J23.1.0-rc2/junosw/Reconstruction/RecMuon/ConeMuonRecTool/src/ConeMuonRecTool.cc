#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/AlgBase.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/SimHeader.h"

#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TMath.h"
#include "TH1F.h"
#include "TRandom.h"
#include <TStopwatch.h>

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

#include "ConeMuonRecTool.h"
#include "llhCone.h"

DECLARE_TOOL(ConeMuonRecTool); 

    ConeMuonRecTool::ConeMuonRecTool(const std::string& name)
    :ToolBase(name)
    , m_ptable(NULL)
{
    declProp("KDEFileLPMT", m_f1name="");
    declProp("KDEFileSPMT", m_f2name="");
    declProp("LSRadius", m_LSRadius = 17700);
    declProp("PMTRadius", m_PMTRadius = 19500);
    declProp("PMTfix", m_PMTfix = 127); 
    declProp("LightSpeed", m_clight =299.792458); 
    declProp("LSRefraction", m_nLSlight = 1.485); 
    declProp("MuonSpeed", m_vmuon = 299.792458);
    declProp("PMTCleaning", m_cleaning = true);
    declProp("generateUserOutput", m_genUserOut = false);
    declProp("UserOutfile", m_userOutfile = "userOutfile.root");  
    declProp("smearTimes", m_smearTimes = true);
    declProp("CleaningTime", m_maxDiff = 5);
    declProp("CleaningTimeSPMT", m_maxDiff_SPMT = 7);
    
    minim = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Combined");
}

ConeMuonRecTool::~ConeMuonRecTool()
{

    // New minimizer interface
    delete minim;
}


    bool
ConeMuonRecTool::configure(const Params* pars, const PmtTable* ptab)
{
    LogDebug  << "configure the reconstruct tool [ConeMuonRecTool]!"
        << std::endl; 

    m_ptable = ptab; 

    TFile* tempf = new TFile(m_f1name.c_str());
    if(!tempf->IsOpen()) {
        LogError << "Failed to open the LPMT KDE_file ["
            << m_f1name << "]!!" << std::endl; 
        return false; 
    }
    TGraph *L_KDE_c[18];
    TGraph *L_KDE_s[18];
    TGraph *L_KDE_ch[18];
    TGraph *L_KDE_fs[18];
    for(int kdeId=0;kdeId<18;kdeId++){
        std::stringstream kdeName_c;
        kdeName_c <<"llh_kde_"<<kdeId<<"_c";
        std::stringstream kdeName_s;
        kdeName_s <<"llh_kde_"<<kdeId<<"_s";
        std::stringstream kdeName_ch;
        kdeName_ch <<"llh_kde_"<<kdeId<<"_ch";
        std::stringstream kdeName_fs;
        kdeName_fs <<"llh_kde_"<<kdeId<<"_fs";
        TGraph *L_kde_c = (TGraph *)tempf->Get(kdeName_c.str().c_str());
        TGraph *L_kde_s = (TGraph *)tempf->Get(kdeName_s.str().c_str());
        TGraph *L_kde_ch = (TGraph *)tempf->Get(kdeName_ch.str().c_str());
        TGraph *L_kde_fs = (TGraph *)tempf->Get(kdeName_fs.str().c_str());
        L_KDE_c[kdeId] = L_kde_c;
        L_KDE_s[kdeId] = L_kde_s;
        L_KDE_ch[kdeId] = L_kde_ch;
        L_KDE_fs[kdeId] = L_kde_fs;
    }
    LogDebug << "--------LPMT PDF Array created---------" << std::endl;
    t0prof_L = (TProfile*)tempf->Get("dt0_prof");
    LogDebug << "------LPMT t0_fix profile loaded-------" << std::endl;
    neighbour_LUT_L = (TH2I*)tempf->Get("neighbour_LUT");
    LogDebug << "------LPMT neighbour LUT loaded--------" << std::endl;

    TFile* tempf2 = new TFile(m_f2name.c_str());
    if(!tempf2->IsOpen()) {
        LogError << "Failed to open the SPMT KDE_file ["
            << m_f2name << "]!!" << std::endl; 
        return false; 
    }
    TGraph *S_KDE_c[18];
    TGraph *S_KDE_s[18];
    TGraph *S_KDE_ch[18];
    TGraph *S_KDE_fs[18];
    for(int kdeId=0;kdeId<18;kdeId++){
        std::stringstream kdeName_c;
        kdeName_c <<"llh_kde_"<<kdeId<<"_c";
        std::stringstream kdeName_s;
        kdeName_s <<"llh_kde_"<<kdeId<<"_s";
        std::stringstream kdeName_ch;
        kdeName_ch <<"llh_kde_"<<kdeId<<"_ch";
        std::stringstream kdeName_fs;
        kdeName_fs <<"llh_kde_"<<kdeId<<"_fs";
        TGraph *S_kde_c = (TGraph *)tempf2->Get(kdeName_c.str().c_str());
        TGraph *S_kde_s = (TGraph *)tempf2->Get(kdeName_s.str().c_str());
        TGraph *S_kde_ch = (TGraph *)tempf2->Get(kdeName_ch.str().c_str());
        TGraph *S_kde_fs = (TGraph *)tempf2->Get(kdeName_fs.str().c_str());
        S_KDE_c[kdeId] = S_kde_c;
        S_KDE_s[kdeId] = S_kde_s;
        S_KDE_ch[kdeId] = S_kde_ch;
        S_KDE_fs[kdeId] = S_kde_fs;
    }
    LogDebug << "--------SPMT PDF Array created---------" << std::endl;
    t0prof_S = (TProfile*)tempf2->Get("dt0_prof");
    LogDebug << "------SPMT t0_fix profile loaded-------" << std::endl;
    neighbour_LUT_S = (TH2I*)tempf2->Get("neighbour_LUT");
    LogDebug << "------SPMT neighbour LUT loaded--------" << std::endl;

    fcn = new llhCone(m_ptable);
    fcn->setLKDE(L_KDE_c,L_KDE_s,L_KDE_ch,L_KDE_fs);
    fcn->setSKDE(S_KDE_c,S_KDE_s,S_KDE_ch,S_KDE_fs);
    fcn->setclight(m_clight);
    //fcn->setnLS(m_nLSlight);
    fcn->setnLS(1.505);         // effective n, large impact
    fcn->setvmuon(m_vmuon);
    LogDebug << "------llhCone Tool setup--------" << std::endl;

    minim->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
    minim->SetPrintLevel(1);

    return true; 
}

bool ConeMuonRecTool::doTimeSmearing(){
    fhtTable.clear();
    const PmtTable& ptab = *m_ptable; 
    unsigned int pmtnum = ptab.size();
    for (unsigned int i = 0; i < pmtnum; ++i){
        if(m_smearTimes){
            fhtTable.push_back(gRandom->Gaus(ptab[i].fht, ptab[i].res));
        }	
        else{
            fhtTable.push_back(ptab[i].fht);
        }
    }
    fcn->setFhtTable(fhtTable);
    return true;
}

    bool
ConeMuonRecTool::reconstruct(RecTrks* trks)
{   
    //TStopwatch ts1;
    //ts1.Start();
    LogDebug << "--Time smearing before anything---" << std::endl;
    doTimeSmearing();
    LogDebug << "------FHT smeared with gaus-------" << std::endl;
    LogDebug << "reconstructing..." << std::endl;
    //Fast Event Analysis
    TVector3 r0, direction; 
    double t0, t0_S, trackl;
    double dfc, t0_fix;
    TVector3 firstLPMT, firstSPMT;
    int initPmtUsed = 0;
    int initLPMTused, initSPMTused;
    initPmtUsed = fastAna(firstLPMT, firstSPMT, dfc, t0, t0_S, t0_fix, initLPMTused, initSPMTused);
    double nPEmean_S, nPEmean_L;
    if(initSPMTused > 0) nPEmean_S = m_totalpe_S*1./initSPMTused;
    else nPEmean_S = -1;
    if(initLPMTused > 0) nPEmean_L = m_totalpe_L*1./initLPMTused;
    else nPEmean_L = -1;
    LogDebug << "Init #LPMT=" << initLPMTused << " with mean nPE = " << nPEmean_L << ", init #SPMT=" << initSPMTused << " with mean nPE = " << nPEmean_S << std::endl;
    LogDebug << "DFC is " << dfc << ", t0+t0fix is " << t0 << "+" << t0_fix << "ns" << std::endl;
    //Do PMT Cleaning
    int remLPMT, remSPMT;
    int remPMT = removePmts(dfc, firstLPMT, t0+t0_fix, nPEmean_L, nPEmean_S, remLPMT, remSPMT);
    if(initLPMTused >0) LogDebug << "Cleaning removed " << remLPMT << " LPMTs (" << 100*remLPMT/(1.*initLPMTused) << " %)" << std::endl;
    if(initSPMTused >0) LogDebug << "Cleaning removed " << remSPMT << " SPMTs (" << 100*remSPMT/(1.*initSPMTused) << " %)" << std::endl;
    LogDebug << "Cleaning removed " << remPMT << " total PMTs (" << 100*remPMT/(1.*initLPMTused+1.*initSPMTused) << " %)" << std::endl;
    //Calculate the inital values of arguments; 
    int numPmtUsed = 0;
    if(initSPMTused >0) numPmtUsed = iniargs(r0, t0_S, direction, trackl, dfc);
    else numPmtUsed = iniargs(r0, t0, direction, trackl, dfc);
    // Try true track as seed
    //numPmtUsed = iniWithTruth(r0, direction, trackl);
    double rho0=m_LSRadius;
    double the0 = r0.Theta(); 
    double phi0 = r0.Phi(); 
    double theTrk = direction.Theta(); 
    double phiTrk = direction.Phi(); 

    double degree = TMath::Pi()/180; 
    
    ROOT::Math::Functor func(*fcn,7);
    minim->SetFunction(func);

    minim->SetFixedVariable(0, "rho0", rho0);
    minim->SetVariable(1, "theta0", the0, 0.5*degree);
    minim->SetVariable(2, "phi0", phi0, 0.5*degree);
    minim->SetVariable(3, "t0", t0+t0_fix, 0.1);
    minim->SetVariable(4, "theTrk", theTrk, 0.5*degree);
    minim->SetVariable(5, "phiTrk", phiTrk, 0.5*degree);
    minim->SetVariable(6, "tracklength", trackl, 100);

    if(numPmtUsed>0)                                                                         
    {
        double ier = minim->Minimize();
        double minstat = minim->Status();

        LogDebug << "------------------------------" << std::endl                              
            << "ier     :   "  << minstat<< std::endl; 
        minResult = minstat;                                            
        minVal = minim->MinValue();                                            
    }

    double re_theta0=0, re_phi0=0, re_theTrk=0, re_phiTrk=0, re_rho0=0, re_t0=0 , re_trackl=0;

    // get the restrcution parameters
    if(numPmtUsed>0)
    {
        const double *xs = minim->X();
        re_rho0     = xs[0];
        re_theta0   = xs[1];
        re_phi0     = xs[2];
        re_t0       = xs[3];
        re_theTrk   = xs[4];
        re_phiTrk   = xs[5];
        re_trackl   = xs[6];

        LogDebug << "Complete to miniut." << std::endl; 
    }

    double array[] = {re_rho0, re_theta0, re_phi0, re_t0, re_theTrk, re_phiTrk, re_trackl}; 
    std::vector<double> pars(array, array+sizeof(array)/sizeof(double)); 
    double chi2 = (*fcn)(pars); // meaningless!
    double ndf = numPmtUsed-7; 
    TVector3 re_R0, re_dir, re_Re; 
    re_R0.SetMagThetaPhi(re_rho0, re_theta0, re_phi0); 
    re_dir.SetMagThetaPhi(1, re_theTrk, re_phiTrk); 
    re_Re = re_R0+re_dir*re_trackl;
    // FIXME: Tracklength is not implemented in Reco -> Value of exitpoint Re is non-sense here!
    // Beware!
    double re_te = re_t0+re_trackl/m_vmuon; 

    TVector3 start(re_R0[0], re_R0[1], re_R0[2]);
    TVector3 end(re_Re[0], re_Re[1], re_Re[2]);
    trks->addTrk(start, end, re_t0, re_te, m_totalpe_L, chi2/ndf, 0);

    // CLHEP::HepLorentzVector start(re_R0[0], re_R0[1], re_R0[2], re_t0); 
    // CLHEP::HepLorentzVector end(re_Re[0], re_Re[1], re_Re[2], re_te); 
    // JM::RecTrack* mtrk = new JM::RecTrack(start, end); 
    // mtrk->setQuality(chi2/ndf); // meaningless!
    // JM::CDTrackRecEvent* evt = new JM::CDTrackRecEvent();
    // evt->addTrack(mtrk);
    // rh->setCDTrackEvent(evt);
    //ts1.Stop();
    //ts1.Print(); 

    LogDebug << "------------------------------" << std::endl
        << "Result : " << std::endl
        << "chi2/ndf   :   " << chi2/ndf << std::endl
        << "re_rho0(mm):   "  << re_rho0<< std::endl
                                 << "re_theta0  :   "  << re_theta0*180/TMath::Pi()<< std::endl
                                 << "re_phi0    :   "  << re_phi0*180/TMath::Pi() << std::endl
                                 << "re_t0(ns)  :   "  << re_t0<< std::endl
                                                          << "re_theTrk  :   "  << re_theTrk*180/TMath::Pi()<< std::endl
                                                          << "re_phiTrk  :   "  << re_phiTrk*180/TMath::Pi()<< std::endl
                                                          << "re_trackl(mm)  :   "  << re_trackl<< std::endl
                                                                                       << "start(" << re_R0[0] << ", " << re_R0[1] << ", " << re_R0[2] << ", " << re_t0 << "ns) "  << std::endl
                                                                                       << "end(" << re_Re[0] << ", " << re_Re[1] << ", " << re_Re[2] << ", " << re_te <<  "ns) " << std::endl;
    if(m_genUserOut) writeUserOutput(re_R0, re_dir, re_Re, minResult, minVal);
    double re_dfc = (-re_R0.Cross(re_dir)).Mag();
    double re_theta = TMath::ACos(re_dir.Z());
    LogDebug  << "Reconstructed Track has: " << std::endl;
    LogDebug  << "Distance from Center D = " << re_dfc/1000. << std::endl;
    LogDebug  << "Inclination Theta = " << re_theta*180./TMath::Pi() << std::endl;

    minim->Clear();
    return true;
}
std::ostream& operator << (std::ostream& s, const TVector3& v)
{
    s  <<  "("  <<  v.x()  <<  ", "  <<  v.y()  <<  ", "  <<  v.z()  <<  ")"; 
    return s; 
}

TVector3 operator/ (const TVector3& v1, double c)
{
    double x = v1.x()/c; 
    double y = v1.y()/c; 
    double z = v1.z()/c; 
    return TVector3(x, y, z); 
}
int
ConeMuonRecTool::iniargs(TVector3 & r0, double & t0, 
        TVector3 & direction, double& length, double dfc )// output
{
    // Get Punch-in first
    TVector3 pi(0, 0, 0); // Clear it before setting 
    length=0; 
    int numPmtUsed=0; 
    const PmtTable& ptab = *m_ptable; 
    unsigned int pmtnum = ptab.size();
    for (unsigned int i = 0; i < pmtnum; ++i){
        if(ptab[i].used == 1){
            numPmtUsed++;
            if(m_totalpe_S > 0){
                if(fhtTable.at(i) < t0+2.5 && ptab[i].type == _PMTINCH3){ // Use first fired SPMT because they are closer to entry point
                    pi += ptab[i].q * ptab[i].pos;
                }
            }
            else if(m_totalpe_L > 0){
                if(fhtTable.at(i) < t0+2.5 && ptab[i].type == _PMTINCH20){ // Use first fired LPMT if there are no SPMT
                    pi += ptab[i].q * ptab[i].pos;
                }
            }
        }
    }
    pi = m_PMTRadius * pi.Unit();
    // Now get Punch-out
    TVector3 po(0,0,0);
    double tracklength = 2.*sqrt(m_PMTRadius*m_PMTRadius - dfc*dfc); // in mm
    double tid = tracklength/m_vmuon;
    double t_po = t0+tid;
    double twin = 10., lwin = 2.5;
    int pmtcount = 0;
    while(pmtcount < 6){
        po.SetXYZ(0,0,0);
        pmtcount = 0;
        for (unsigned int i = 0; i < pmtnum; ++i){
            if(ptab[i].used == 1){
                //if( ptab[i].fht > t_po-twin/2. && ptab[i].fht < t_po+twin/2.){
                if( fhtTable.at(i) > t_po-twin/2. && fhtTable.at(i) < t_po+twin/2.){
                    if( (ptab[i].pos - pi).Mag() > m_vmuon*(tid-lwin) && (ptab[i].pos - pi).Mag() < m_vmuon*(tid+lwin) ){
                        po += ptab[i].q * ptab[i].pos;
                        pmtcount++;
                    }
                }
            }
        }
        twin +=2.;
        lwin +=0.5;
    }
    po = m_PMTRadius * po.Unit();
    length = (po-pi).Mag();
    direction = (po-pi).Unit();

    double par1 = pi*direction;
    double par2 = pi*pi - m_LSRadius*m_LSRadius;
    double lPI = -par1-sqrt(par1*par1-par2);
    r0 = pi + lPI*direction; 

    LogDebug  << std::endl
        <<"Pmt used count: " << numPmtUsed << std::endl
        <<"Inital argument: " << std::endl
        << " r0:" << r0 << std::endl
        << " t0:" << t0 << std::endl
        << " re:" << po << std::endl
        << " theta0:" << r0.Theta()*180/TMath::Pi() << std::endl
        << " phi0:" << r0.Phi()*180/TMath::Pi() << std::endl
        << " theTrk:" << direction.Theta()*180/TMath::Pi() << std::endl
        << " phiTrk:" << direction.Phi() *180/TMath::Pi()<< std::endl
        << " tracklength:" << length << std::endl; 

    return numPmtUsed; 

}

int ConeMuonRecTool::iniWithTruth(TVector3 & r0, TVector3 & direction, double& length){
    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" 
            << std::endl;
        return false;
    }
    auto m_buf = navBuf.data();
    // Get Truth
    auto nav = m_buf->curEvt();
    auto simheader = JM::getHeaderObject<JM::SimHeader>(nav);
    auto se = (JM::SimEvt*)simheader->event();
    auto simtrk = (JM::SimTrack*) se->findTrackByTrkID(0);
    TVector3 pi;
    pi.SetXYZ(simtrk->getInitX(), simtrk->getInitY(), simtrk->getInitZ());
    direction.SetXYZ(simtrk->getInitPx(), simtrk->getInitPy(), simtrk->getInitPz());
    direction = direction.Unit();

    double par1 = pi*direction;
    double par2 = pi*pi - m_LSRadius*m_LSRadius;
    double lPI = -par1-sqrt(par1*par1-par2);
    r0 = pi + lPI*direction;

    double mc_dfc = (-r0.Cross(direction)).Mag();
    length = 2*sqrt(m_LSRadius*m_LSRadius - mc_dfc*mc_dfc);
    LogDebug  << std::endl
        <<"Inital argument: " << std::endl
        << " r0:" << r0 << std::endl
        << " theta0:" << r0.Theta()*180/TMath::Pi() << std::endl
        << " phi0:" << r0.Phi()*180/TMath::Pi() << std::endl
        << " theTrk:" << direction.Theta()*180/TMath::Pi() << std::endl
        << " phiTrk:" << direction.Phi() *180/TMath::Pi()<< std::endl
        << " tracklength:" << length << std::endl; 

    return 17739; 

}

int ConeMuonRecTool::fastAna(TVector3 & firstLPMT, TVector3 & firstSPMT, double & dfc, double & t0, double& t0_S, double& t0_fix, int& initLPMTused, int& initSPMTused ){// output
    m_totalpe = 0;
    m_totalpe_L = 0;
    m_totalpe_S = 0;
    int t0L=9999999;
    int t0S=9999999;
    const PmtTable& ptab = *m_ptable; 
    unsigned int pmtnum = ptab.size();
    int numPmtUsed=0;
    initLPMTused=0;
    initSPMTused=0; 
    for (unsigned int i = 0; i < pmtnum; ++i){
        if(ptab[i].used){
            m_totalpe += ptab[i].q;
            numPmtUsed++;
            if(ptab[i].type == _PMTINCH20){
                //h1->Fill(fhtTable.at(i));
                m_totalpe_L += ptab[i].q;
                initLPMTused++;
                if (fhtTable.at(i)<t0L)
                {
                    t0L = fhtTable.at(i);
                    firstLPMT = ptab[i].pos;
                }
            }
            else if(ptab[i].type == _PMTINCH3){
                m_totalpe_S += ptab[i].q;
                initSPMTused++;
                if (fhtTable.at(i)<t0S)
                {
                    t0S = fhtTable.at(i);
                    firstSPMT = ptab[i].pos;
                }
            }
        }
    }
    LogDebug << "Init LPMT used " << initLPMTused << ", init SPMT used: " << initSPMTused << std::endl;
    int evtcase; // case 
    if(initLPMTused == 0) evtcase = 2;
    else evtcase = 1;
    TH1D *h1 = new TH1D("h1", "fht distribution", evtcase*400, 0, evtcase*400);  // 400 bins for LPMT, 800 for SPMT
    for (unsigned int i = 0; i < pmtnum; ++i){
        if(ptab[i].used){
            if(evtcase == 1 && ptab[i].type == _PMTINCH20) h1->Fill(fhtTable.at(i));
            else if(evtcase == 2) h1->Fill(fhtTable.at(i));
        }
    }
    LogDebug << "Duration histo has " << h1->GetEntries() << " entries" << std::endl;
    int binId = h1->FindFirstBinAbove(0);
    LogDebug << "First bin above zero: " <<  binId << std::endl;
    int nPmtUsed = 0;
    while(nPmtUsed < evtcase*50){   // LPMT: 50, SPMT: 100 (double number of PMTs)
        nPmtUsed += h1->GetBinContent(binId);
        binId++;
    }
    LogDebug << "While is done! binId =" <<  binId << std::endl;
    double fhtStart = h1->GetBinCenter(binId-1);
    double fhtStop = h1->GetBinCenter(h1->FindLastBinAbove(3*h1->GetBinContent(binId)));
    double duration = fhtStop - fhtStart;
    if(evtcase == 1){
        dfc = 1000*(duration-150.025)/2.64032;      // for LPMT
        t0 = t0L;
    }
    else if(evtcase == 2){
        dfc = 1000*(duration-168.419)/3.57574;      // for SPMT
        t0 = t0S;
    }
    if (dfc<0) dfc = 0.;
    else if(dfc > m_LSRadius) dfc = m_LSRadius - 700.;
    t0_S = t0S;
    if(evtcase == 1) t0_fix = t0prof_L->GetBinContent(t0prof_L->FindBin(dfc/1000.));
    else if(evtcase == 2) t0_fix = t0prof_S->GetBinContent(t0prof_S->FindBin(dfc/1000.));
    LogDebug << "t0 fix from profile: " << t0_fix << "ns for track with D=" << dfc/1000. << std::endl;
    //t0_fix = 5.5;
    return numPmtUsed;
}

int ConeMuonRecTool::removePmts(double dfc, TVector3 firstPMT, double toff, double nPEmean_L, double nPEmean_S, int& remLPMT, int&remSPMT){
    const PmtTable& ptab = *m_ptable; 
    unsigned int pmtnum = ptab.size();
    usedTable.clear();
    for (unsigned int i = 0; i < pmtnum; ++i){
        if(ptab[i].used)usedTable.push_back(1);
        else usedTable.push_back(0);
    }
    int removedPmt=0;
    int removedPmt_early=0;
    int removedSPmt_charge=0;
    int removedLPmt_outlier=0;
    int removedSPmt_outlier=0;
    remLPMT=0;
    remSPMT=0;
    if(m_cleaning){
        LogDebug << "---------Hit cleaning for LPMT and SPMT----------" << std::endl;
        LogDebug << "----Strategy: Cleaning removes any PMTs with earlier FHT than " << toff+8 << "ns -----" << std::endl;
        LogDebug << "----Strategy: Cleaning removes LPMTs outliers from neighbours in " << m_maxDiff<< "ns window -----" << std::endl;
        LogDebug << "----Strategy: Cleaning removes SPMTs with less than " << 0.5*nPEmean_S << " PE -----" << std::endl;
        //LogDebug << "----Strategy: Cleaning removes SPMTs outliers with respect to LPMTs " << std::endl;
        for(unsigned int pmtId = 0; pmtId < pmtnum; ++pmtId){
            if(ptab[pmtId].used){
                // Hard cut below logical first possible LS hit time
                if(ptab[pmtId].fht < toff + 8){
                    usedTable.at(pmtId) = 0;
                    removedPmt_early++;
                    removedPmt++;
                    if(ptab[pmtId].type == _PMTINCH20) remLPMT++;
                    else if(ptab[pmtId].type == _PMTINCH3) remSPMT++;
                    continue;
                }
                
                if(ptab[pmtId].type == _PMTINCH20){
                    //do 20ich PMT cleaning

                    // Get Ids of neighbour PMT
                    std::vector<int> neighbour_L_IDs = getNeigbours(pmtId, 0);
                    // Get Mean of neighbour hit times
                    double sum = 0;
                    int counter = 0;
                    for(std::vector<int>::iterator it=neighbour_L_IDs.begin(); it < neighbour_L_IDs.end(); it++){
                        sum += ptab[*it].fht;
                        counter++;
                    }
                    double mean = sum/counter;
                    if(fabs(ptab[pmtId].fht-mean) > m_maxDiff){
                        usedTable.at(pmtId) = 0;
                        removedLPmt_outlier++;
                        removedPmt++;
                        remLPMT++;
                    }
                }

                if(ptab[pmtId].type == _PMTINCH3){
                    //do 3ich PMT cleaning
                    if(ptab[pmtId].q < 0.5*nPEmean_S){
                        usedTable.at(pmtId) = 0;
                        removedSPmt_charge++;
                        removedPmt++;
                        remSPMT++;
                        continue;
                    }
                    // put outlier removal based on LPMT here
                    /*
                    std::vector<int> neighbour_S_IDs = getNeigbours(pmtId, 1);
                    double sum = 0;
                    int counter = 0;
                    for(std::vector<int>::iterator it=neighbour_S_IDs.begin(); it < neighbour_S_IDs.end(); it++){
                        sum += ptab[*it].fht;
                        counter++;
                    }
                    double mean = sum/counter;
                    if(fabs(ptab[pmtId].fht-mean) > m_maxDiff_SPMT){
                        usedTable.at(pmtId) = 0;
                        removedSPmt_outlier++;
                        removedPmt++;
                        remSPMT++;
                    }
                    */
                }
            }
        }
        LogDebug << "--------------Early__ PMT cut removed " << removedPmt_early << " PMTs----------" << std::endl;
        LogDebug << "--------------Charge_ SPMT cut removed " << removedSPmt_charge << " PMTs----------" << std::endl;
        LogDebug << "--------------Outlier PMT cut removed " << removedLPmt_outlier << " LPMTs and " << removedSPmt_outlier << " SPMTs ----------" << std::endl;
    }
    fcn->setused(usedTable);
    return removedPmt;
}

std::vector<int> ConeMuonRecTool::getNeigbours(unsigned int pmtId, int type){
    std::vector<int> v;
    if(type==0){
        int nBinsY = neighbour_LUT_L->GetNbinsY();
        for(int ybin = 1; ybin <= nBinsY; ybin++){
            v.push_back(neighbour_LUT_L->GetBinContent(pmtId+1, ybin));
        }
    }
    else if(type==1){
        int nBinsY = neighbour_LUT_S->GetNbinsY();
        for(int ybin = 1; ybin <= nBinsY; ybin++){
            v.push_back(neighbour_LUT_S->GetBinContent(pmtId+1, ybin));
        }
    }
    return v;
}

bool ConeMuonRecTool::writeUserOutput(TVector3 re_R0, TVector3 re_dir, TVector3 re_Re, double minResult, double minVal){
    // Get Service
    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" 
            << std::endl;
        return false;
    }
    auto m_buf = navBuf.data();
    // Get Truth
    auto nav = m_buf->curEvt();
    auto simheader = JM::getHeaderObject<JM::SimHeader>(nav);
    auto se = (JM::SimEvt*)simheader->event();
    auto simtrk = (JM::SimTrack*) se->findTrackByTrkID(0);
    TVector3 mc_R0, mc_dir; 
    mc_R0.SetXYZ(simtrk->getInitX(), simtrk->getInitY(), simtrk->getInitZ());
    mc_dir.SetXYZ(simtrk->getInitPx(), simtrk->getInitPy(), simtrk->getInitPz());
    mc_dir = mc_dir.Unit();
    TFile* userOutfile = new TFile(m_userOutfile.c_str(),"UPDATE");
    TTree* t1 = new TTree("t1","Tree with extracted RecResult");
    t1->Branch("mc_R0", &mc_R0);
    t1->Branch("mc_dir", &mc_dir);
    t1->Branch("re_R0", &re_R0);
    t1->Branch("re_dir", &re_dir);
    t1->Branch("re_Re", &re_Re);
    t1->Branch("minResult", &minResult);
    t1->Branch("minVal", &minVal);
    t1->Fill();
    userOutfile->Write();
    userOutfile->Close();
    return true;
}
