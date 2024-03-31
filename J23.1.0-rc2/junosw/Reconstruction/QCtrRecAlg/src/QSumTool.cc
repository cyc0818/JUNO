//to do review
#include "QSumTool.h"
#include "SniperKernel/ToolFactory.h"
#include "TFile.h"
#include "TH2.h"
#include "TH1.h"
#include <fstream>

DECLARE_TOOL(QSumTool);

const int ThetaPMTNum= 1440;
const double ADCL = 1000.;

QSumTool::QSumTool(const std::string &name): ToolBase(name){
}

QSumTool::~QSumTool(){
}

bool QSumTool::configure(){
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

    d_lightYield = 0.;
    for(int i=0;i<ThetaPMTNum;i++) {
        TH2F* h2dtemp = (TH2F*)LnPEMapFile->Get(Form("hLMu2D_%d", i));
        TH1D* htemp = (TH1D*)h2dtemp->ProjectionX(Form("hLMu1D_%d", i), 1, h2dtemp->GetYaxis()->GetNbins());
        htemp->Scale(1./h2dtemp->GetYaxis()->GetNbins());
        d_lightYield += htemp->Interpolate(1.e-8);
        delete htemp;
        delete h2dtemp;
    }
    LnPEMapFile->Close();
    delete LnPEMapFile;

    d_lightYield /= ThetaPMTNum;
    LogDebug<< "nPEMap_lightYield per PMT:" << d_lightYield << std::endl;

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

    return true;
}

void QSumTool::rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo){
    double qsum = 0;
    for (auto* thisPMT: pmttable){
        double qtemp = thisPMT->totq/thisPMT->gain;
        qtemp -= thisPMT->dnrate*ADCL/1.e9;
        qsum += qtemp;
    }
    qsum /= (d_lightYield*pmttable.size());
    recinfo.energy = qsum*nHESF;
    LogDebug<<"Energy:"<<recinfo.energy<<"MeV"<<std::endl;
}



