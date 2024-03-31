#ifndef ATMNUGEN
#define ATMNUGEN


#include <iostream>
#include <fstream>
#include <cassert>
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <TROOT.h>
#include <TRandom.h>
#include <vector>
#include <TSystem.h>
#include <TFile.h>
#include <TH1.h>
#include <TTree.h>
#include <cstdlib>
#include <TChain.h>
#include <TString.h>
#include "deex.h"
#include <map>
#include <algorithm>
#include "Params.h"

#include "TDatabasePDG.h"
#include "TParticlePDG.h"

using namespace std;
using namespace CLHEP;
void Usage();

void ProcessArgs(int argc, char** argv, Params& params);
void PrintParams(Params a, FILE * stream);
#endif
