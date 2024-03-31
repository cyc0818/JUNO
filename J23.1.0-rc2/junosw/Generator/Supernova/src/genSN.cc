#include <unistd.h>//for getopt()
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <TF1.h>
#include <TF2.h>
#include <TF3.h>
#include <TRandom3.h>
#include <Math/WrappedMultiTF1.h>
#include <Math/AdaptiveIntegratorMultiDim.h>
#include <Math/GSLMCIntegrator.h>
#include "dataflux.h"
#include "detector.h"
#include "pdfs.h"
#include "sneventsoutput.h"
#include "sort.h"

using namespace std;

TDetector *detector = NULL;
TDataFlux *flux = NULL;

void Usage(){
    std::cout << "SN Generator Usage:\n"
        << "------------------------------------------\n"
        << "-i input file of the neutrino model\n"
        << "-o output file of the generator\n"
        << "-d SN distance\n"
        << "-s random seed\n"
        << "-m mass hierarchy (1:NH, 0:IH, -1:no osc)\n"
        << "-n SN (1) or pre-SN (0)\n"
        << "For pre-SN, the time range needs to be specified with [-l lower range; -u upper range] in unit [day]\n"
        << "-c channel number [0:IBD, 1:pES, 2:eES, 3:N12, 4:B12, 5:C12 NC], generate all channels if ignored\n"
        << "To generate events from only one flavor neutrino: -f [0:nuE, 1:nuAE, 2:nuX, 3:nuAX]\n"
        << "------------------------------------------"
        <<std::endl;
}

void CheckArgc(int argc)
{
    int npars = (argc-1)/2;
    if( npars!=10 && npars!=9 && npars!=8 && npars!=7 && npars != 6)
    {
        std::cout << "incorrect command line parameters" << std::endl;
        exit(1);
    }
}

int type_to_pdgid(int type){
  if(type==0) return 12;
  if(type==1) return -12;
  if(type==2) return 14;
  if(type==3) return -14;
  else return 0;
}

//--------Main Function---------//
int main(int argc, char **argv)
{

    //print out the usage of SN generator 
    Usage();

    //----command parameter processing----// 

    CheckArgc(argc);

    string filePath;
    int channelno = -1;
    int seed=-1;
    double distSN = -1; 
    int isNH = false;
    bool isSN = false;
    //time range for integral, default for SN
    double tRange[2] = {-0.05,20.}; 
    int iflavor = -1;
    string ofileName; 

    int opt;
    while((opt = getopt(argc, argv, "i:c:s:d:m:n:l:u:o:f:"))!= -1){
        switch(opt){
            case'i':
                filePath = optarg;
                break;
            case'o':
                ofileName = optarg;
                break;
            case'c':
                channelno = atoi(optarg);//interaction channel
                break;
            case's':
                seed = atoi(optarg);//random seed
                break;
            case'd':
                distSN = atof(optarg);//SN distance
                break;
            case'm':
                isNH = atoi(optarg);//1:NH; 0:IH; -1:no osc
                break;
            case'n':
                isSN = bool(atoi(optarg));//1:SN; 0:pre-SN
                break;
            case'l':
                tRange[0] = atof(optarg);
		if(isSN==0) tRange[0] *= 24*3600; //if pre-SN, lower time range [day]
                break;
            case'u':
                tRange[1] = atof(optarg);
		if(isSN==0) tRange[0] *= 24*3600; //if pre-SN, upper time range [day]
                break;
            case'f':
                iflavor = atoi(optarg);//if specify only one type neutino,neutrino flavor
                break;
            case'?':
                printf("Error: wrong command input option %s\n",optarg);
                break;
            case':':
                printf("Error: Forget to add parameter after option %s\n",optarg);
                break;
        }
    }

    if(iflavor <-1 || iflavor > 3){
        std::cout << "wrong flavor of neutrino specified!!" << std::endl;
        exit(-1);
    }

    //create output file
    TSNEventsOutput *output = new TSNEventsOutput(ofileName.c_str());

    //detector property, 20kt LS with 12% protons
    detector = new TDetector(20.0, 0.12);

    //read in the neutrino model
    flux = new TDataFlux(filePath.data(), distSN, isNH, isSN);

    gRandom = new TRandom3(seed);


    //----------Generate events--------//
    ofstream ofs;
    TF3 *ibdrate, *pesrate, *eesrate;
    TF2 *n12rate, *b12rate, *c12rate, *pestotalrate, *eestotalrate;
    TF1 *n12decay, *b12decay;
    double averageEvents;
    int  trueEvents;
    double mP = 938.272046;
    double mN = 939.565379;
    double mE = 0.510998928;
    double t, eNu, costheta, tmpE, tmpP, tmpPhi;
    Int_t NuID = type_to_pdgid(iflavor); // value for iflavor != -1
    vector <Myevent> v;
    Myevent tmpevt;

    //energy range for integral 
    double eRange[2] = {0};
    if(isSN){
        eRange[0] = 0.;
        eRange[1] = 100.;
    }
    else{
        eRange[0] = 0.;
        eRange[1] = 25.;
    }
    
    //different channles
    if(channelno == -1 || channelno == 0)
    {
        if(iflavor == -1){
            ibdrate = new TF3("IBDRate", PDFs::IBDRate, tRange[0], tRange[1], eRange[0], eRange[1], -1.0, 1.0, 0);
        }
        else{
            ibdrate = new TF3("IBDRate", PDFs::IBDRateFlavor, tRange[0], tRange[1], eRange[0], eRange[1], -1.0, 1.0, 1);
            ibdrate->SetParameter(0,iflavor);
        }
        ibdrate->SetNpx(1000);
        ibdrate->SetNpy(200);
        ibdrate->SetNpz(200); 
        //averageEvents = ibdrate->Integral(tRange[0], tRange[1], eRange[0], eRange[1], -1.0, 1.0);

        ROOT::Math::WrappedMultiTF1 wf1(*ibdrate);
        ROOT::Math::AdaptiveIntegratorMultiDim ig;
        ig.SetFunction(wf1);
        ig.SetRelTolerance(1e-2);
        double xmins[3] = {tRange[0], eRange[0], -1.0};
        double xmaxs[3] = {tRange[1], eRange[1], 1.0};
        averageEvents = ig.Integral(xmins,xmaxs);
        trueEvents = gRandom->Poisson(averageEvents);
        cerr<<"IBD "<<trueEvents<<" events"<<", average "<<averageEvents<<endl;

        for(int i = 0; i < trueEvents; i++)
        {
            ibdrate->GetRandom3(t, eNu, costheta);
            tmpE = PDFs::IBDEe(eNu, costheta);
            tmpP = sqrt(tmpE * tmpE - mE * mE);
            tmpPhi = 2 * M_PI * gRandom->Uniform();
            evt.nparticles = 2;
            evt.pdgid[0] = -11;
            evt.m[0] = mE;
            evt.t[0] = t * 1e9;
            evt.px[0] = tmpP * sqrt(1 - costheta * costheta) * cos(tmpPhi);
            evt.py[0] = tmpP * sqrt(1 - costheta * costheta) * sin(tmpPhi);
            evt.pz[0] = tmpP * costheta;
            evt.pdgid[1] = 2112;
            evt.m[1] = mN;
            evt.t[1] = t * 1e9;
            evt.px[1] = - evt.px[0];
            evt.py[1] = - evt.py[0];
            evt.pz[1] = eNu - evt.pz[0];
            evt.nuE = eNu;
            evt.origPDGID = -12;
            tmpevt.Set(evt);
            v.push_back(tmpevt);
        }
        delete ibdrate;
    }

    if(channelno == -1 || channelno == 1)
    {
        if(iflavor==-1){
            pesrate = new TF3("PESRate", PDFs::PESRate, tRange[0], tRange[1], eRange[0], eRange[1], 0.0, 1.0, 0);
            pestotalrate = new TF2("PESTotalRate", PDFs::PESTotalRate, tRange[0], tRange[1], eRange[0], eRange[1], 0);
        }
        else{
            pesrate = new TF3("PESRate", PDFs::PESRateFlavor, tRange[0], tRange[1], eRange[0], eRange[1], 0.0, 1.0, 1);
            pesrate->SetParameter(0,iflavor);
            pestotalrate = new TF2("PESTotalRate", PDFs::PESTotalRateFlavor, tRange[0], tRange[1], eRange[0], eRange[1], 1);
            pestotalrate->SetParameter(0,iflavor);
        }
        pesrate->SetNpx(1000);
        pesrate->SetNpy(200);
        pesrate->SetNpz(200);
        
        ROOT::Math::WrappedMultiTF1 wf1(*pestotalrate);
        ROOT::Math::AdaptiveIntegratorMultiDim ig;
        ig.SetFunction(wf1);
        ig.SetRelTolerance(1e-2);
        double xmins[2] = {tRange[0], eRange[0]};
        double xmaxs[2] = {tRange[1], eRange[1]};
        averageEvents = ig.Integral(xmins,xmaxs);
        
        //averageEvents = pestotalrate->Integral(tRange[0], tRange[1], eRange[0], eRange[1]);
        delete pestotalrate;
        
        trueEvents = gRandom->Poisson(averageEvents);
        cerr<<"ProtonES "<<trueEvents<<" events" << ", average " << averageEvents<<endl;
        for(int i = 0; i < trueEvents; i++)
        //for(int i = 0; i < 100; i++)
        {
            pesrate->GetRandom3(t, eNu, costheta);
            tmpE = PDFs::PESEp(eNu, costheta);
            tmpP = sqrt(pow(tmpE + mP, 2) - mP * mP);
            tmpPhi = 2 * M_PI * gRandom->Uniform();
            if(iflavor==-1){
              NuID = PDFs::PESNuID(t, eNu, costheta, gRandom);
            }
            evt.nparticles = 1;
            evt.pdgid[0] = 2212;
            evt.m[0] = mP;
            evt.t[0] = t * 1e9;
            evt.px[0] = tmpP * sqrt(1 - costheta * costheta) * cos(tmpPhi);
            evt.py[0] = tmpP * sqrt(1 - costheta * costheta) * sin(tmpPhi);
            evt.pz[0] = tmpP * costheta;
            evt.pdgid[1] = 0;
            evt.m[1] = 0;
            evt.t[1] = 0;
            evt.px[1] = 0;
            evt.py[1] = 0;
            evt.pz[1] = 0;
            evt.nuE = eNu;
            evt.origPDGID = NuID;
            tmpevt.Set(evt);
            v.push_back(tmpevt);
        }
        delete pesrate;
    }

    if(channelno == -1 || channelno == 2)
    {
        if(iflavor==-1){
            eesrate = new TF3("EESRate", PDFs::EESRate, tRange[0], tRange[1], eRange[0], eRange[1], 0.0, 1.0, 0);
            eestotalrate = new TF2("EESTotalRate", PDFs::EESTotalRate, tRange[0], tRange[1], eRange[0], eRange[1], 0);
        }
        else{
            eesrate = new TF3("EESRate", PDFs::EESRateFlavor, tRange[0], tRange[1], eRange[0], eRange[1], 0.0, 1.0, 1);
            eesrate->SetParameter(0,iflavor);
            eestotalrate = new TF2("EESTotalRate", PDFs::EESTotalRateFlavor, tRange[0], tRange[1], eRange[0], eRange[1], 1);
            eestotalrate->SetParameter(0,iflavor);
        }

        eesrate->SetNpx(1000);
        eesrate->SetNpy(200);
        eesrate->SetNpz(200);
        
        ROOT::Math::WrappedMultiTF1 wf1(*eestotalrate);
        ROOT::Math::AdaptiveIntegratorMultiDim ig;
        ig.SetFunction(wf1);
        ig.SetRelTolerance(1e-2);
        double xmins[2] = {tRange[0], eRange[0]};
        double xmaxs[2] = {tRange[1], eRange[1]};
        averageEvents = ig.Integral(xmins,xmaxs);
        
        //averageEvents = eestotalrate->Integral(tRange[0], tRange[1], eRange[0], eRange[1]);
        delete eestotalrate;
        trueEvents = gRandom->Poisson(averageEvents);
        cerr<<"ElectronES "<<trueEvents<<" events"<<", average "<<averageEvents<<endl;
        for(int i = 0; i < trueEvents; i++)
        //for(int i = 0; i < 1; i++)
        {
            eesrate->GetRandom3(t, eNu, costheta);
            //		eesrate->GetRandom3(t, eNu, tmpE);
            //		costheta = EESCosTheta(eNu, tmpE);
            tmpE = PDFs::EESEe(eNu, costheta);
            tmpP = sqrt(pow(tmpE, 2) - mE * mE);
            tmpPhi = 2 * M_PI * gRandom->Uniform();
            if(iflavor==-1){
              NuID = PDFs::EESNuID(t, eNu, costheta, gRandom);
            }
            evt.nparticles = 1;
            evt.pdgid[0] = 11;
            evt.m[0] = mE;
            evt.t[0] = t * 1e9;
            evt.px[0] = tmpP * sqrt(1 - costheta * costheta) * cos(tmpPhi);
            evt.py[0] = tmpP * sqrt(1 - costheta * costheta) * sin(tmpPhi);
            evt.pz[0] = tmpP * costheta;
            evt.pdgid[1] = 0;
            evt.m[1] = 0;
            evt.t[1] = 0;
            evt.px[1] = 0;
            evt.py[1] = 0;
            evt.pz[1] = 0;
            evt.nuE = eNu;
            evt.origPDGID = NuID;
            tmpevt.Set(evt);
            v.push_back(tmpevt);
        }
        delete eesrate;
    }

    if(channelno == -1 || channelno == 3)
    {
        if(iflavor==-1){
            n12rate = new TF2("N12Rate", PDFs::N12Rate, tRange[0], tRange[1], 17.339, eRange[1], 0);
        }
        else{
            n12rate = new TF2("N12Rate", PDFs::N12RateFlavor, tRange[0], tRange[1], 17.339, eRange[1], 1);
            n12rate->SetParameter(0,iflavor); 
        }

        n12rate->SetNpx(1000);
        n12rate->SetNpy(200);
        
        ROOT::Math::WrappedMultiTF1 wf1(*n12rate);
        ROOT::Math::AdaptiveIntegratorMultiDim ig;
        ig.SetFunction(wf1);
        ig.SetRelTolerance(1e-2);
        double xmins[2] = {tRange[0], 17.339};
        double xmaxs[2] = {tRange[1], eRange[1]};
        averageEvents = ig.Integral(xmins,xmaxs);
        
        //averageEvents = n12rate->Integral(tRange[0], tRange[1], 17.339, eRange[1]);
        trueEvents = gRandom->Poisson(averageEvents);
        cerr<<"N12 "<<trueEvents<<" events"<< ", average " <<averageEvents <<endl;

        n12decay = new TF1("N12Decay", PDFs::N12DecayRate, 0, 16.819);
        for(int i = 0; i < trueEvents; i++)
        //for(int i = 0; i < 1; i++)
        {
            n12rate->GetRandom2(t, eNu);
            tmpE = eNu - 16.827;
            tmpP = sqrt(pow(tmpE, 2) - mE * mE);
            costheta = -1 + 2 * gRandom->Uniform();
            tmpPhi = 2 * M_PI * gRandom->Uniform();
            evt.nparticles = 2;
            evt.pdgid[0] = 11;
            evt.m[0] = mE;
            evt.t[0] = t * 1e9;
            evt.px[0] = tmpP * sqrt(1 - costheta * costheta) * cos(tmpPhi);
            evt.py[0] = tmpP * sqrt(1 - costheta * costheta) * sin(tmpPhi);
            evt.pz[0] = tmpP * costheta;

            tmpP = n12decay->GetRandom();
            costheta = -1 + 2 * gRandom->Uniform();
            tmpPhi = 2 * M_PI * gRandom->Uniform();
            evt.pdgid[1] = -11;
            evt.m[1] = mE;
            evt.t[1] = (t + gRandom->Exp(0.011 / log(2))) * 1e9;
            evt.px[1] = tmpP * sqrt(1 - costheta * costheta) * cos(tmpPhi);
            evt.py[1] = tmpP * sqrt(1 - costheta * costheta) * sin(tmpPhi);
            evt.pz[1] = tmpP * costheta;
            evt.nuE = eNu;
            evt.origPDGID = 12;
            tmpevt.Set(evt);
            v.push_back(tmpevt);
        }
        delete n12rate; 
        delete n12decay;
    }

    if(channelno == -1 || channelno == 4)
    {
        if(iflavor==-1){
            b12rate = new TF2("B12Rate", PDFs::B12Rate, tRange[0], tRange[1], 14.392, eRange[1], 0);
        }
        else{
            b12rate = new TF2("B12Rate", PDFs::B12RateFlavor, tRange[0], tRange[1], 14.392, eRange[1], 1);
            b12rate->SetParameter(0, iflavor);
        }
        b12rate->SetNpx(1000);
        b12rate->SetNpy(200);
        
        ROOT::Math::WrappedMultiTF1 wf1(*b12rate);
        ROOT::Math::AdaptiveIntegratorMultiDim ig;
        ig.SetFunction(wf1);
        ig.SetRelTolerance(1e-2);
        double xmins[2] = {tRange[0], 14.392};
        double xmaxs[2] = {tRange[1], eRange[1]};
        averageEvents = ig.Integral(xmins,xmaxs);
        //averageEvents = b12rate->Integral(tRange[0], tRange[1], 14.392, eRange[1]);
        
        trueEvents = gRandom->Poisson(averageEvents);
        cerr<<"B12 "<<trueEvents<<" events"<<", average "<<averageEvents<<endl;

        b12decay = new TF1("B12Decay", PDFs::B12DecayRate, 0, 13.87059);
        for(int i = 0; i < trueEvents; i++)
        //for(int i = 0; i < 1; i++)
        {
            b12rate->GetRandom2(t, eNu);
            tmpE = eNu - 13.88;
            tmpP = sqrt(pow(tmpE, 2) - mE * mE);
            costheta = -1 + 2 * gRandom->Uniform();
            tmpPhi = 2 * M_PI * gRandom->Uniform();
            evt.nparticles = 2;
            evt.pdgid[0] = -11;
            evt.m[0] = mE;
            evt.t[0] = t * 1e9;
            evt.px[0] = tmpP * sqrt(1 - costheta * costheta) * cos(tmpPhi);
            evt.py[0] = tmpP * sqrt(1 - costheta * costheta) * sin(tmpPhi);
            evt.pz[0] = tmpP * costheta;

            tmpP = b12decay->GetRandom();
            costheta = -1 + 2 * gRandom->Uniform();
            tmpPhi = 2 * M_PI * gRandom->Uniform();
            evt.pdgid[1] = 11;
            evt.m[1] = mE;
            evt.t[1] = (t + gRandom->Exp(0.0202 / log(2))) * 1e9;
            evt.px[1] = tmpP * sqrt(1 - costheta * costheta) * cos(tmpPhi);
            evt.py[1] = tmpP * sqrt(1 - costheta * costheta) * sin(tmpPhi);
            evt.pz[1] = tmpP * costheta;
            evt.nuE = eNu;
            evt.origPDGID = -12;
            tmpevt.Set(evt);
            v.push_back(tmpevt);
        }
        delete b12rate;
        delete b12decay;
    }

    if(channelno == -1 || channelno == 5)
    {
        if(iflavor==-1){
            c12rate = new TF2("C12Rate", PDFs::C12Rate, tRange[0], tRange[1], 15.11, eRange[1], 0);
        }
        else{
            c12rate = new TF2("C12Rate", PDFs::C12RateFlavor, tRange[0], tRange[1], 15.11, eRange[1], 1);
            c12rate->SetParameter(0,iflavor);
        }
        c12rate->SetNpx(1000);
        
        ROOT::Math::WrappedMultiTF1 wf1(*c12rate);
        ROOT::Math::AdaptiveIntegratorMultiDim ig;
        ig.SetFunction(wf1);
        ig.SetRelTolerance(1e-2);
        double xmins[2] = {tRange[0], 15.11};
        double xmaxs[2] = {tRange[1], eRange[1]};
        averageEvents = ig.Integral(xmins,xmaxs);
        //averageEvents = c12rate->Integral(tRange[0], tRange[1], 15.11, eRange[1]);
        
        trueEvents = gRandom->Poisson(averageEvents);
        cerr<<"C12 "<<trueEvents<<" events"<< ", average " << averageEvents<<endl;
        for(int i = 0; i < trueEvents; i++)
        //for(int i = 0; i < 5; i++)
        {
            c12rate->GetRandom2(t, eNu);
            tmpE = 15.11;
            tmpP = tmpE;
            costheta = -1 + 2 * gRandom->Uniform();
            tmpPhi = 2 * M_PI * gRandom->Uniform();
            if(iflavor==-1){
              NuID = PDFs::C12NuID(t, eNu, gRandom);
              //std::cout << NuID << std::endl;
            }
            evt.nparticles = 1;
            evt.pdgid[0] = 22;
            evt.m[0] = 0;
            //C12 de-excitation width is 38.5 eV, which is 1.7e-17 s
            evt.t[0] = t * 1e9;	
            evt.px[0] = tmpP * sqrt(1 - costheta * costheta) * cos(tmpPhi);
            evt.py[0] = tmpP * sqrt(1 - costheta * costheta) * sin(tmpPhi);
            evt.pz[0] = tmpP * costheta;

            evt.pdgid[1] = 0;
            evt.m[1] = 0;
            evt.t[1] = 0;
            evt.px[1] = 0;
            evt.py[1] = 0;
            evt.pz[1] = 0;
            evt.nuE = eNu;
            evt.origPDGID = NuID;
            //std::cout << evt.origPDGID << std::endl;
            tmpevt.Set(evt);
            v.push_back(tmpevt);
        }
        delete c12rate;
    }
    //EVENT ttt;
    //for(auto&& tevt: v){
    //  tevt.Get(ttt);
    //  std::cout << ttt.origPDGID << std::endl;
   // }
    cerr << "Start Sorting" << endl;
    NR::hpsort(v);
    int nevents = v.size();
    for(int i = 0; i < nevents; i++)
    {
        v[i].Get(evt);
        output->pushEvents();
    }

    output->writeFile();
    v.clear();	
    delete output;
}
