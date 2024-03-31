#include <iostream>
#include <cstdlib>
#include <cstring>
#include "SolarNeutrinoSpectrum.hh"
#include "TFile.h"
#include "TTree.h"
#include <cmath>
const double MeV = 1.0;
void usage() {
    std::cerr<<"Usage 1: NuSolGen.exe -o {OUTPUTFILE} -n {EVENT} -seed {SEED} -source {SOURCE}"<<std::endl;
    std::cerr<<"Usage 2: NuSolGen.exe -n {EVENT} -source {SOURCE}"<<std::endl;
    std::cerr<<"	OUTPUTFILE will be named as {SOURCE}.root and seed will be set to time()"<<std::endl;
    std::cerr<<"possible source: Be7 pep pp B8 N13 O15 F17 hep"<<std::endl;
    exit(1);
}
#include <string>
void set_neutrino_type(SolarNeutrinoSpectrum &solar,const char *neutype) {
    bool ok = solar.SetNeutrinoType(neutype);
    if(!ok) usage();
}

int main(int argc,char *argv[]) {
    int nEvent = 10;
    int seed = time(nullptr);
    SolarNeutrinoSpectrum solargen;
    set_neutrino_type(solargen,"Be7");
    TString outputName = "NuSol.root";
    //std::cout<<argc<<" "<<argv[1]<<std::endl;
    switch(argc) {
	case 9:
	    if(strcmp(argv[1],"-o")!=0) usage();
	    outputName = argv[2];
	    if(strcmp(argv[3],"-n")!=0) usage();
	    nEvent = atoi(argv[4]);
	    if(strcmp(argv[5],"-seed")!=0) usage();
	    seed = atoi(argv[6]);
	    if(strcmp(argv[7],"-source")!=0) usage();
	    set_neutrino_type(solargen,argv[8]);
	    break;
	case 5:
	    if(strcmp(argv[1],"-n")!=0) usage();
	    nEvent = atoi(argv[2]);
	    if(strcmp(argv[3],"-source")!=0) usage();
	    set_neutrino_type(solargen,argv[4]);
	    outputName = argv[4];
	    outputName+= ".root";
	    break;
	default:
	    usage();
    }

    TFile *output = TFile::Open(outputName.Data(),"RECREATE");
    TTree *tree = new TTree("NuSol","NuSol generator tree");
    Double_t electron_kinetic_energy;
    tree->Branch("electron_kinetic_energy",&electron_kinetic_energy,"electron_kinetic_energy/D");

    const int trueEvents = nEvent;
    for(int i = 0; i < trueEvents; i++)
    {
	solargen.GeneratePrimaries(electron_kinetic_energy);
	if(i%100==0) std::cout<<"i: "<<i<<std::endl;
	tree->Fill();
    }

    tree->Write();
    output->Close();
    return 0;
}
