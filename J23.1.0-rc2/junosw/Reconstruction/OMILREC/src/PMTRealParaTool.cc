#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"

#include "OMILREC/IRecTool.h"
#include "Geometry/PMTParamSvc.h"
#include "Geometry/PMT.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include <fstream>
#include <vector>
#include <string>


class PMTRealParaTool: public ToolBase, public IRecParaTool {
public:
    PMTRealParaTool(const std::string& name): ToolBase(name) {
        declProp("RecMapPath", RecMapPath);
        declProp("enableUpdatePMTPara", enableUpdatePMTPara);
        declProp("enableTimeOffsetCorr", enableTimeOffsetCorr);
    }

    ~PMTRealParaTool() {

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

        LogInfo << "Hello! I am in the PMTRealParaTool ! "<< std::endl;
        return true;
    } 
    bool LoadPMTPara() {
        std::string PMTDataPath = RecMapPath + "/CalibPMTPara";

        SniperPtr<PMTParamSvc> pmtParamSvc(*getParent(), "PMTParamSvc");
        if (pmtParamSvc.invalid()) {
            LogError << "Can't Locate  PMTParamSvc."
                     << std::endl;
            return false;
        }

        SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
        if (calSvc.invalid()) {
            LogError << "Can't Locate  PMTCalibSvc."
                     << std::endl;
            return false;
        }

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
 
        // Read PMT parameters from CalibSVC
        // lpmt
        LogDebug<<"Loading PMTPara from SVC(database) ..."<<std::endl;
        if((calSvc->getGain()).at(0)>1e-8) LGain = calSvc->getGain();
        LogDebug<<"LGain[0]:"<<LGain[0]<<std::endl;

        if((calSvc->getDarkRate()).at(0)>1e-8) LDNR = calSvc->getDarkRate();
        LogDebug<<"LDNR[0]:"<<LDNR[0]<<std::endl;

        if((calSvc->getRelativeDE()).at(0)>1e-8) LPDE = calSvc->getRelativeDE();
        LogDebug<<"LPDE[0]:"<<LPDE[0]<<std::endl;

        if((calSvc->getTimeOffset()).at(0)>1e-8) TimeOffset = calSvc->getTimeOffset();
        LogDebug<<"TimeOffset[0]:"<<TimeOffset[0]<<std::endl;

        // spmt
        if((calSvc->getSPMTRelativeDE()).at(0)>1e-8) SDNR = calSvc->getSPMTDarkRate();
        LogDebug<<"SDNR[0]:"<<SDNR[0]<<std::endl;

        if((calSvc->getSPMTRelativeDE()).at(0)>1e-8) SPDE = calSvc->getSPMTRelativeDE();
        LogDebug<<"SPDE[0]:"<<SPDE[0]<<std::endl;


        // Todo replace SPEs with getTimeOffset()).at(#)
        SPEs = calSvc->getChargeSpec();
        for(int i=0;i<int(SPEs.size());i++) {
            SPEs[i]->Fit("gaus", "RQ0", "", 0.5, 1.5);
            TF1* fitfunc = SPEs[i]->GetFunction("gaus");
            LGain[i] = fitfunc->GetParameter(1);
            LQRes[i] = fitfunc->GetParameter(2);
            if(!IsDyn[i]) LGain[i] = SPEs[i]->GetMean();
            delete fitfunc;
        }
        // Update PMT paras
        // User can specify the enableUpdatePMTPara if
        // PMT paras are updated after software release
        if(enableUpdatePMTPara) {
            double id_temp = 0.;
            double gain_temp = 0.;
            double meangain_temp = 0.;
            double time_temp = 0.;
            double dr_temp = 0.;
            double de_temp = 0.;
            std::ifstream InputGainPara;
            InputGainPara.open( PMTDataPath + "/CalibPMTPara.txt", std::ios::in);
            for(int i=0;i<TotalLPMT;i++) {
                InputGainPara>>id_temp;
                InputGainPara>>de_temp;
                InputGainPara>>gain_temp;
                InputGainPara>>time_temp;
                InputGainPara>>dr_temp;
                InputGainPara>>meangain_temp;

                LPDE[i] = de_temp;
                LDNR[i] = dr_temp;
                TimeOffset[i] =time_temp;
            }
            InputGainPara.close();


            SPEs.clear();
            TFile* CalibSPEs_m = new TFile((PMTDataPath + "/CalibPMTSPEs.root").c_str(), "READ");
            for(int i=0;i<TotalLPMT;i++) {
                TH1D* spes_temp = (TH1D*)CalibSPEs_m->Get(Form("ch%d_charge_spec", i));
                SPEs.push_back(spes_temp);

                SPEs[i]->Fit("gaus", "RQ0", "", 0.5, 1.5);
                TF1* fitfunc = SPEs[i]->GetFunction("gaus");
                LGain[i] = fitfunc->GetParameter(1);
                LQRes[i] = fitfunc->GetParameter(2);
                if(!IsDyn[i]) LGain[i] = SPEs[i]->GetMean();
                delete fitfunc;
            }

            LogDebug<<"enableUpdatePMTPara ..."<<std::endl;
            LogDebug<<"LPDE[0]:"<<LPDE[0]<<"   "<<"LDNR[0]:"<<LDNR[0]<<"   "<<"LGain[0]:"<<LGain[0]<<std::endl;
            LogDebug<<"LPDE[1]:"<<LPDE[1]<<"   "<<"LDNR[1]:"<<LDNR[1]<<"   "<<"LGain[1]:"<<LGain[1]<<std::endl;
        } 
        // Calculate the relative DE and the dark noise light level
        double LPDE_Avg = 0.;
        for(int i=0;i<TotalLPMT;i++) LPDE_Avg += LPDE[i];
        LPDE_Avg /= double(TotalLPMT);
        for(int i=0;i<TotalLPMT;i++) {
            LDarkMu += LDNR[i];
            LPDE[i] /= LPDE_Avg;
        }
        double SPDE_Avg = 0.;
        for(int i=0;i<TotalSPMT;i++) {
            SPDE_Avg += SPDE[i];
        }
        SPDE_Avg /= double(TotalSPMT);
        for(int i=0;i<TotalSPMT;i++) SPDE[i] /= SPDE_Avg;


        LDarkMu *= ADCL;
        LDarkMu /= (1.0e9 * double(TotalLPMT));
        LogDebug<<"LPDE_Avg (PMTCalib): "<<LPDE_Avg<<std::endl;

        LogInfo<<"The dark noise #mu & threshold is around:" << LDarkMu << '\t'
               <<(LDarkMu*double(TotalLPMT))/(ADCL/LFHSBinW)+sqrt((LDarkMu * double(TotalLPMT))/(ADCL/LFHSBinW))<<std::endl;

        LogDebug<<"Final PMTPara ..."<<std::endl;
        LogDebug<<"LGain[0] & Res[0] & LDNR[0] & LPDE[0] & TimeOffset[0]:"<<LGain[0]<<'\t'
                << LQRes[0] <<'\t'<< LDNR[0] <<'\t'<< LPDE[0] <<'\t'<< TimeOffset[0] << std::endl;
        LogDebug<<"LGain[1] & Res[1] & LDNR[1] & LPDE[1] & TimeOffset[1]:"<<LGain[1]<<'\t'
                << LQRes[1] <<'\t'<< LDNR[1] <<'\t'<< LPDE[1] <<'\t'<< TimeOffset[1] << std::endl;
        LogDebug<<"SGain[1] & SRes[1] & DNR[1] & SPDE[1] & STimeOffset[1]:"
                <<'\t'<< SDNR[1] <<'\t'<< SPDE[1]  << std::endl;

        LogDebug<< "///////////////////// PMT info. has been loaded /////////////////////" << std::endl;

        // User can specify the enableTimeOffsetCorr if
        // TimeOffSets are updated after software release
        if(!enableTimeOffsetCorr) {
            for(int i=0;i<TotalLPMT;i++) {
                TimeOffset[i] = 0.;
            }
        }

        return true;
    }

private:
    int ADCL;
    int ZeroSpmtID;
    double LFHSBinW;
    double SFHSBinW;
    std::string RecMapPath;
    bool enableUpdatePMTPara;
    bool enableTimeOffsetCorr;

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


bool PMTRealParaTool::GetIsDyn(const int pmtid) {
    return IsDyn.at(pmtid);
}
TH1D*  PMTRealParaTool::GetSPEs(const int pmtid) {
    return SPEs.at(pmtid);
}
double PMTRealParaTool::GetTimeOffset(const int pmtid) {
    return TimeOffset.at(pmtid);
}
double PMTRealParaTool::GetLTTS(const int pmtid) {
    return LTTS.at(pmtid);
}
double PMTRealParaTool::GetLGain(const int pmtid) {
    return LGain.at(pmtid);
}
double PMTRealParaTool::GetLQRes(const int pmtid) {
    return LQRes.at(pmtid);
}
double PMTRealParaTool::GetLPDE(const int pmtid) {
    return LPDE.at(pmtid);
}
double PMTRealParaTool::GetLDNR(const int pmtid) {
    return LDNR.at(pmtid);
}

double PMTRealParaTool::GetSPDE(const int pmtid) {
    return SPDE.at(pmtid);
}
double PMTRealParaTool::GetSTTS(const int pmtid) {
    return STTS.at(pmtid);
}
double PMTRealParaTool::GetSDNR(const int pmtid) {
    return SDNR.at(pmtid);
}
double PMTRealParaTool::GetLDarkMu() {
    return LDarkMu;
}
double PMTRealParaTool::GetSDarkMu() {
    return SDarkMu;
}

DECLARE_TOOL(PMTRealParaTool);

