/// This is a simple program to generate the Ni-60 gammas that result from
/// Co-60 decays. This program considers only the 2-gamma case (1.173237 MeV
/// and 1.332501 MeV) which totally dominates.
/// The angular correlation is correct.
///
/// \author Chris Jillings
/// jillings@caltech.edu, May 10, 2006

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

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
  Hep3Vector p1, p2; // the two gamma momentum vectors. Unit GeV/c
  
  FILE* stream = stdout;
  if( outFilename!=NULL ) {
    stream = fopen(outFilename, "w");
    if( stream==NULL ) {
      printf("Please enetr a valid filename.\n");
      Usage();
      exit(0);
    }
  }
  HepRandom::setTheSeed(rseed);
  //start by printing some information to comment lines
  fprintf(stream, "# File generated by %s.\n", argv[0]);
  fprintf(stream, "# Ransom seed for generator = %ld.\n", rseed);
  

  double cosTheta, cth2, theta, azimuth; // angles used
  bool ok;
  double rejectVariable;
  unsigned int i;

  for( i=0 ; i<nEvents ; i++ ) {
    p1.setX(0); p1.setY(0); p1.setZ(1);
    ok = false;
    while( !ok ) {
      cosTheta = RandFlat::shoot(-1, 1);
      cth2 = cosTheta * cosTheta;
      rejectVariable = RandFlat::shoot(1.167);
      if( rejectVariable < (1 + 0.125*cth2 + 0.042*cth2*cth2) ) ok = true;
    }
    azimuth = RandFlat::shoot( 2.0*M_PI );
    p2.setRThetaPhi(1, acos(cosTheta), azimuth);
    // p1 and p2 have correct angu;ar correlation. Rotate them randomly
    cosTheta = RandFlat::shoot(-1, 1);
    theta = acos(cosTheta);
    p1.rotateY(theta);
    p2.rotateY(theta);
    azimuth = RandFlat::shoot( 2.0*M_PI );
    p1.rotateZ(azimuth);
    p2.rotateZ(azimuth);
    p1 *= 1.173237e-3*GeV; // GeV
    p2 *= 1.332501e-3*GeV; // GeV

    fprintf(stream, "2\n");
    fprintf(stream, "1\t22 0 0 %e %e %e 0.0\n", p1.x()/GeV, p1.y()/GeV, p1.z()/GeV );
    fprintf(stream, "1\t22 0 0 %e %e %e 0.0\n", p2.x()/GeV, p2.y()/GeV, p2.z()/GeV );

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
  printf("Cobalt-60-gammas [-seed seed] [-o outputfilename] [-n nevents]\n");
}

