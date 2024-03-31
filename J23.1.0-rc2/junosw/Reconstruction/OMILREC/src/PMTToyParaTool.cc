#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"

#include "OMILREC/IRecTool.h"
#include "Geometry/PMTParamSvc.h"
#include "Geometry/PMT.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
//#include "IPMTSimParamSvc/IPMTSimParamSvc.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include <fstream>
#include <vector>
#include <string>


class PMTToyParaTool: public ToolBase, public IRecParaTool {
public:
    PMTToyParaTool(const std::string& name): ToolBase(name) {
        declProp("RecMapPath", RecMapPath);
        declProp("enableUserPMTParaTruth", enableUserPMTParaTruth);
        declProp("enableAddToyQ", enableAddToyQ);
        declProp("enableAddToyTTS", enableAddToyTTS);
        declProp("enableAddToyDN", enableAddToyDN);
    }

    ~PMTToyParaTool() {

    }
    

    bool GetIsDyn(const int pmtid);
    TH1D* GetSPEs(const int pmtid);
    double GetTimeOffset(const int pmtid);
    double GetLTTS(const int pmtid);
    double GetLGain(const int pmtid);
    double GetLQRes(const int pmtid);
    double GetLPDE(const int pmtid);
    double GetLDNR(const int pmtid);
    double GetSPDE(const int pmtid);
    double GetSTTS(const int pmtid);
    double GetSDNR(const int pmtid);
    double GetLDarkMu();
    double GetSDarkMu();
    
    bool init() {
        ADCL = 1000;
        ZeroSpmtID = kOFFSET_CD_SPMT;
        LFHSBinW = 5.;
        SFHSBinW = 10.;

 
        SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
        if ( rgSvc.invalid()) {
            LogError << "Failed to get RecGeomSvc instance!" << std::endl;
            return false;
        }
        TotalLPMT = rgSvc->getCdGeom()->findPmt20inchNum();
        TotalSPMT = rgSvc->getCdGeom()->findPmt3inchNum();

        LogInfo << "Hello! I am in the PMTToyParaTool ! "<< std::endl;
        return true;
    } 

    bool LoadPMTPara() {
        std::string PMTDataPath = RecMapPath + "/CalibPMTPara";

        //////////////////////
        // Load PMT parameters
        SniperPtr<PMTParamSvc> pmtParamSvc(*getParent(), "PMTParamSvc");
        if (pmtParamSvc.invalid()) {
            LogError << "Can't Locate  PMTParamSvc."
                     << std::endl;
            return false;
        }
        
        /*
        SniperPtr<IPMTSimParamSvc> simpmtsvc(*getRoot(), "PMTSimParamSvc");
        if (simpmtsvc.invalid()) {
            LogError << "can't find service PMTSimParamSvc" << std::endl;
            assert(0);
        }  else {
            LogInfo << "Retrieve PMTSimParamSvc Successfully. " << std::endl;
            m_simpmtsvc = simpmtsvc.data();
        }*/

        for(int i=0;i<TotalLPMT;i++) {
            IsDyn.push_back(pmtParamSvc->isHamamatsu(i));
            LGain.push_back(1.);
            LQRes.push_back(0.3);
            LDNR.push_back(0.);
            LPDE.push_back(1.);
            LTTS.push_back(0.);
            TimeOffset.push_back(0.);
        }
        for(int i=0;i<TotalSPMT;i++) {
            SPDE.push_back(1.);
            SDNR.push_back(0.);
            STTS.push_back(0.);
        }

        // Read PMT QE form PMTParam_CD_LPMT.root for
        // the reconstruction of ideal case

        
        std::string base = getenv("JUNOTOP");
        std::string workbase = base;
        if(getenv("WORKTOP")) {
            workbase = getenv("WORKTOP");
        }
        double lpdetemp = 0., spdetemp = 0.;
        std::string LPmtDataBase = base + "/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root";
        TFile* LPmtDataFile = TFile::Open( LPmtDataBase.c_str(), "READ");
        TTree* LPmtData = (TTree*)LPmtDataFile->Get("data");
        LPmtData->SetBranchAddress("PDE", &lpdetemp);

        std::string SPmtDataBase = base + "/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root";
        TFile* SPmtDataFile = TFile::Open( SPmtDataBase.c_str(), "READ");
        TTree* SPmtData = (TTree*)SPmtDataFile->Get("data");
        SPmtData->SetBranchAddress("QE", &spdetemp);

        double LPDE_Avg = 0.;
        for(int i=0;i<TotalLPMT;i++) {
            //LPDE[i] = m_simpmtsvc->get_pde(i);
            LPmtData->GetEntry(i);
            LPDE[i] = lpdetemp;
            LPDE_Avg += LPDE[i];
        }
        LPDE_Avg /= double(TotalLPMT);
        for(int i=0;i<TotalLPMT;i++) {LPDE[i] /= LPDE_Avg;}

        double SPDE_Avg = 0.;
        for(int i=0;i<TotalSPMT;i++) {
            //SPDE[i] = m_simpmtsvc->get_QE(i + ZeroSpmtID);
            SPmtData->GetEntry(i);
            SPDE[i] = spdetemp;
            SPDE_Avg += SPDE[i];
        }
        SPDE_Avg /= double(TotalSPMT);
        for(int i=0;i<TotalSPMT;i++) SPDE[i] /= SPDE_Avg;
        LogDebug<<"LPDE_Avg & SPDE_Avg: "<<LPDE_Avg<<'\t'<<SPDE_Avg <<std::endl;

        LogDebug<<"PMTParaTruth ..."<<std::endl;
        LogDebug<<"LGain[0] & Res[0] & LDNR[0] & LPDE[0] & TimeOffset[0]:"<<LGain[0]<<'\t'
                << LQRes[0] <<'\t'<< LDNR[0] <<'\t'<< LPDE[0] <<'\t'<< TimeOffset[0] << std::endl;
        LogDebug<<"LGain[1] & Res[1] & LDNR[1] & LPDE[1] & TimeOffset[1]:"<<LGain[1]<<'\t'
                << LQRes[1] <<'\t'<< LDNR[1] <<'\t'<< LPDE[1] <<'\t'<< TimeOffset[1] << std::endl;

        /////////////////////////
        // For adding toy DN&LTTS
        if(enableAddToyTTS || enableAddToyQ || enableAddToyDN) {
            double ttstemp = 0., dntemp = 0., qrestemp = 0.;
            LPmtData->SetBranchAddress("Resolution", &qrestemp);
            LPmtData->SetBranchAddress("TTS_SS", &ttstemp);
            LPmtData->SetBranchAddress("DCR", &dntemp);
            for(int i=0;i<TotalLPMT;i++) {
                LPmtData->GetEntry(i);
                LQRes[i] = qrestemp;
                LTTS[i] = ttstemp;
                LDNR[i] = dntemp*1.e3;
                LDarkMu += LDNR[i];
            }
            
            SPmtData->SetBranchAddress("darkRate", &dntemp);
            for(int i=0;i<TotalSPMT;i++) {
                SPmtData->GetEntry(i);
                SDNR[i] = dntemp;
                SDarkMu += SDNR[i];
            }

            LDarkMu *= ADCL;
            LDarkMu /= (1.0e9 * double(TotalLPMT));
            LogInfo<<"Adding toy dark noise ..."<<std::endl;
            LogInfo<<"The dark noise mu & threshold of LPMT is around:" << LDarkMu << '\t'
                   <<(LDarkMu*double(TotalLPMT))/(ADCL/LFHSBinW)+sqrt((LDarkMu * double(TotalLPMT))/(ADCL/LFHSBinW))<<std::endl;

            SDarkMu *= ADCL;
            SDarkMu /= (1.0e9 * double(TotalSPMT));
            LogInfo<<"The dark noise mu & threshold of SPMT is around:" << SDarkMu << '\t'
                   <<(SDarkMu*double(TotalSPMT))/(ADCL/SFHSBinW)+sqrt((SDarkMu * double(TotalSPMT))/(ADCL/SFHSBinW))<<std::endl;
        }
        delete LPmtData;
        delete LPmtDataFile;
        delete SPmtData;
        delete SPmtDataFile;

        /*
        if(enableAddToyTTS || enableAddToyQ || enableAddToyDN) {
            for(int i=0;i<TotalLPMT;i++) {
                LQRes[i] = m_simpmtsvc->get_sigmaGain(i);;
                LTTS[i] = m_simpmtsvc->get_tts(i);
                LDNR[i] = m_simpmtsvc->get_dcr(i)*1.e3;
                LDarkMu += LDNR[i];
            }
            for(int i=0;i<TotalSPMT;i++) {
                SDNR[i] = m_simpmtsvc->get_dcr(ZeroSpmtID+i)*1.e3;
                STTS[i] = m_simpmtsvc->get_tts(ZeroSpmtID+i);
                SDarkMu += SDNR[i];
            }

            LDarkMu *= ADCL;
            LDarkMu /= (1.0e9 * double(TotalLPMT));
            LogInfo<<"Adding toy dark noise ..."<<std::endl;
            LogInfo<<"The dark noise mu & threshold of LPMT is around:" << LDarkMu << '\t'
                   <<(LDarkMu*double(TotalLPMT))/(ADCL/LFHSBinW)+sqrt((LDarkMu * double(TotalLPMT))/(ADCL/LFHSBinW))<<std::endl;

            SDarkMu *= ADCL;
            SDarkMu /= (1.0e9 * double(TotalSPMT));
            LogInfo<<"The dark noise mu & threshold of SPMT is around:" << SDarkMu << '\t'
                   <<(SDarkMu*double(TotalSPMT))/(ADCL/SFHSBinW)+sqrt((SDarkMu * double(TotalSPMT))/(ADCL/SFHSBinW))<<std::endl;
        }
        */

        LogDebug<<"Final PMTPara ..."<<std::endl;
        LogDebug<<"LGain[0] & Res[0] & LDNR[0] & LPDE[0] & TimeOffset[0]:"<<LGain[0]<<'\t'
                << LQRes[0] <<'\t'<< LDNR[0] <<'\t'<< LPDE[0] <<'\t'<< TimeOffset[0] << std::endl;
        LogDebug<<"LGain[1] & Res[1] & LDNR[1] & LPDE[1] & TimeOffset[1]:"<<LGain[1]<<'\t'
                << LQRes[1] <<'\t'<< LDNR[1] <<'\t'<< LPDE[1] <<'\t'<< TimeOffset[1] << std::endl;
        LogDebug<< "///////////////////// PMT info. has been loaded /////////////////////" << std::endl;


        return true;
    }

private:
    int ADCL;
    int ZeroSpmtID;
    double LFHSBinW;
    double SFHSBinW;
    std::string RecMapPath;
    //IPMTSimParamSvc* m_simpmtsvc;

    bool enableUserPMTParaTruth;
    bool enableAddToyQ;
    bool enableAddToyTTS;
    bool enableAddToyDN;

    std::vector<bool> IsDyn;
    double TotalLPMT;
    double TotalSPMT;
    double LDarkMu;
    double SDarkMu;
    std::vector<TH1D*>  SPEs;
    std::vector<double> TimeOffset;
    std::vector<double> LTTS;
    std::vector<double> LGain;
    std::vector<double> LQRes;
    std::vector<double> LPDE;
    std::vector<double> LDNR;
    std::vector<double> SPDE;
    std::vector<double> STTS;
    std::vector<double> SDNR; 

};


bool PMTToyParaTool::GetIsDyn(const int pmtid) {
    return IsDyn.at(pmtid);
}
TH1D*  PMTToyParaTool::GetSPEs(const int pmtid) {
    return SPEs.at(pmtid);
}
double PMTToyParaTool::GetTimeOffset(const int pmtid) {
    return TimeOffset.at(pmtid);
}
double PMTToyParaTool::GetLTTS(const int pmtid) {
    return LTTS.at(pmtid);
}
double PMTToyParaTool::GetLGain(const int pmtid) {
    return LGain.at(pmtid);
}
double PMTToyParaTool::GetLQRes(const int pmtid) {
    return LQRes.at(pmtid);
}
double PMTToyParaTool::GetLPDE(const int pmtid) {
    return LPDE.at(pmtid);
}
double PMTToyParaTool::GetLDNR(const int pmtid) {
    return LDNR.at(pmtid);
}

double PMTToyParaTool::GetSPDE(const int pmtid) {
    return SPDE.at(pmtid);
}
double PMTToyParaTool::GetSTTS(const int pmtid) {
    return STTS.at(pmtid);
}
double PMTToyParaTool::GetSDNR(const int pmtid) {
    return SDNR.at(pmtid);
}
double PMTToyParaTool::GetLDarkMu() {
    return LDarkMu;
}
double PMTToyParaTool::GetSDarkMu() {
    return SDarkMu;
}

DECLARE_TOOL(PMTToyParaTool);
