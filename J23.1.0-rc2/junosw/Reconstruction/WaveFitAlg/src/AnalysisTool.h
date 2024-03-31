#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TMath.h"
#include "TF1.h"
#include "TLine.h"
#include "TTree.h"
#include "TStyle.h"
#include "TString.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TAxis.h"
#include "TSpectrum.h"
#include "TSpectrum2.h"
#include "TProfile.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TApplication.h"
#include <TSelector.h>
#include <TROOT.h>
#include "FadcReadout.h"

using namespace std;

//const int g_nSamples = 2000;
//TString g_inputname;
//TString g_outputname;

void PlotNewDataWave(FadcWaveform fadcWaveform,int evtIdx,int chnIdx);

//class PmtChannel {
  //public:
    //TrueHits     trueHits;
    //FeeReadout   feeReadout;
    //FadcWaveform fadcWaveform;
//};
