/// This is a simple program to generate the 0.834 MeV gamma that result from
/// Mn-54 decays. (modified from K-40 generator)
/// The implementation is based on the data-base in Brookheaven National Laboratory and Table of Radionuclides.
/// https://www.nndc.bnl.gov/ensdf/#
/// https://www.bipm.org/documents/20126/53814638/Monographie+BIPM-5+-+Comments+%282010%29.pdf/4c77cfc0-8e0a-92f2-e989-3ab369ecd822
/// Electrons and gamma-ray emissions with Ekin<10 keV are ignored in this program.
///
/// \author Akira Takenaka (SJTU)
/// akira913cats@gmail.com, 4 Apr., 2022

#include <cstdio>
#include <cstring>
#include <math.h>
#include <iostream>
#include "TMath.h"

#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Random/Randomize.h>
#include <CLHEP/Units/PhysicalConstants.h>


using namespace std;
using namespace CLHEP;

void ProcessArgs(int argc, char** argv, long* rseed, char** outfilename, 
                 unsigned int* nevents );
void Usage();

int main(int argc, char** argv) {
    long rseed = 0;
    char* outFilename = NULL;
    unsigned int nEvents = 1000000000; // a billion. Default to something big for piping 
    ProcessArgs(argc, argv, &rseed, &outFilename, &nEvents);
    Hep3Vector p1; // the gamma momentum vector. Unit GeV/c
    FILE* stream = stdout;
    if( outFilename!=NULL ) {
        stream = fopen(outFilename, "w");
        if( stream==NULL ) {
            printf("Please enter a valid filename.\n");
            Usage();
            exit(0);
        }
    }
    HepRandom::setTheSeed(rseed);
    //start by printing some information to comment lines
    fprintf(stream, "# File generated by %s.\n", argv[0]);
    fprintf(stream, "# Ransom seed for generator = %ld.\n", rseed);
    
    double cosTheta, theta, azimuth; // angles used
    unsigned int i;
    double bf; //Mn54 decay branching fraction
    double emass = 0.51099892;
    double momentum;
    for( i=0 ; i<nEvents ; i++ ) {
        cosTheta = RandFlat::shoot(-1., 1.);
        azimuth = RandFlat::shoot( 2.0*M_PI );
        theta = acos(cosTheta);

        bf = RandFlat::shoot(0.,1.);
        if(bf<0.999752){  //99.9752% gamma radiation
            p1.setRThetaPhi(1, theta, azimuth);
            p1*=0.834848*MeV;
            fprintf(stream, "1\n");
            fprintf(stream, "1\t22 0 0 %e %e %e 0.0\n", p1.x()/GeV, p1.y()/GeV, p1.z()/GeV );
        } else if (bf<0.999752+0.000222) { //0.000222% K-shell electron emission
            momentum = sqrt((0.828866+emass)*(0.828866+emass)-emass*emass);
            p1.setRThetaPhi(1, theta, azimuth);
            p1*=momentum*MeV;
            fprintf(stream, "1\n");
            fprintf(stream, "1\t11 0 0 %e %e %e 0.51099892e-03\n", p1.x()/GeV, p1.y()/GeV, p1.z()/GeV );   
        } else { // L-shell electron emission
            momentum = sqrt((0.8342+emass)*(0.8342+emass)-emass*emass);
            p1.setRThetaPhi(1, theta, azimuth);
            p1*=momentum*MeV;
            fprintf(stream, "1\n");
            fprintf(stream, "1\t11 0 0 %e %e %e 0.51099892e-03\n", p1.x()/GeV, p1.y()/GeV, p1.z()/GeV );   
        }
    }
    return 0;
}

void ProcessArgs(int argc, char** argv, long *rseed,
        char** outfilename, unsigned int* nevents) {
    int i;
    for( i=1 ; i<argc ; i++ ) {
        if( strcmp(argv[i], "-seed")==0 ) {
            i++;
            sscanf(argv[i], "%ld", rseed);
        } else if( strcmp(argv[i], "-o")==0 ) {
            i++;
            *outfilename = new char[strlen(argv[i]) +1];
            strcpy(*outfilename, argv[i]);
        } else if( strcmp(argv[i], "-n")==0 ) {
            i++;
            sscanf(argv[i], "%ud", nevents);
        } else {
            Usage();
            exit(0);
        }
    }
}

void Usage() {
    printf("Manganese_54_gamma [-seed seed] [-o outputfilename] [-n nevents]\n");
}

