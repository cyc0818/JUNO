
#include "OMILREC/IRecTool.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include <fstream>
#include <vector>
#include <string>
#include "Geometry/PMT.h"

const double PMT_R = 19.434;
const double LS_R = 17.7 + 0.12;
const double c = 2.99792458e8;


class PMTToyInputTool: public ToolBase, public IRecInputTool {
public:
    PMTToyInputTool(const std::string& name): ToolBase(name) {
        declProp("SimFile", SimFile);
        declProp("enableAddToyQ", enableAddToyQ);
        declProp("enableAddToyTTS", enableAddToyTTS);
        declProp("enableAddToyDN", enableAddToyDN);
        declProp("enableSPMTInfo", enableSPMTInfo);
    }

    ~PMTToyInputTool() {

    }
    

    const std::list<JM::CalibPmtChannel*>& Get_chhlist() {
        return chhlist;
    }
    const std::list<JM::CalibPmtChannel*>& Get_schhlist() {
        return schhlist;
    }
    JM::EvtNavigator* Get_nav() {
        return nav;
    }

    double Get_LHIT_Charge(const int pmtid);
    double Get_LHIT_Time(const int pmtid);
    double Get_LHIT_NPE(const int pmtid);

    double Get_SHIT_Charge(const int pmtid);
    double Get_SHIT_Time(const int pmtid);


    Int_t  Get_nPMT() {
        return nPMT;
    }
    Int_t  Get_PMTID(const int k) {
        return PMTID->at(k);
    }
    double Get_hitTime_Toy(const int k) {
        return hitTime_Toy->at(k);
    }

    int    Get_nDNphotons();
    int    Get_DNpmtID(const int k);
    float  Get_DNhitQ(const int k);
    float  Get_DNhitTime(const int k);
    double Get_Buff_PEQ(const int k); //d_a2
    void Set_LHIT_Charge(const int pmtid, double charge);
    void Set_SHIT_Charge(const int pmtid, double charge);
    void Set_LHIT_Time(const int pmtid, double time);
    void Set_SHIT_Time(const int pmtid, double time);

    bool init() {
        m_Evt = -1;
        ADCL = 1000;
        ZeroSpmtID = kOFFSET_CD_SPMT;
        PedThres = 0.1;

        SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
        if ( navBuf.invalid() ) {
            LogError << "cannot get the NavBuffer @ /Event" << std::endl;
            return false;
        }
        m_buf = navBuf.data();

 
        SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
        if ( rgSvc.invalid()) {
            LogError << "Failed to get RecGeomSvc instance!" << std::endl;
            return false;
        }
        m_cdGeom = rgSvc->getCdGeom();
        TotalLPMT = rgSvc->getCdGeom()->findPmt20inchNum();
        TotalSPMT = rgSvc->getCdGeom()->findPmt3inchNum();

        for(int i = 0; i<TotalLPMT; i++){
            unsigned int all_pmt_id = (unsigned int)i;
            Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
            PmtGeom *all_pmt = m_cdGeom->getPmt(all_id);
            if ( !all_pmt ) {
                LogError << "Wrong Pmt Id " << i << std::endl;
            }
            TVector3 all_pmtCenter = all_pmt->getCenter();
            ALL_LPMT_pos.push_back(all_pmtCenter);
        }


        LogDebug << "===> Reading simulation file " << std::endl;
        LogInfo  << SimFile << std::endl;
        sfile = TFile::Open(SimFile.c_str());
        PMTID = new std::vector<int>;
        hitTime_Toy  = new std::vector<double>;
        simevt = (TTree*)sfile->Get("evt");
        simevt->SetBranchAddress("nPhotons",&nPMT);
        simevt->SetBranchAddress("pmtID", &PMTID);
        simevt->SetBranchAddress("hitTime", &hitTime_Toy);
        simevt->SetBranchAddress("edepX", &edepx);
        simevt->SetBranchAddress("edepY", &edepy);
        simevt->SetBranchAddress("edepZ", &edepz);

        LogInfo << "Hello! I am in the PMTToyInputTool ! "<< std::endl;
        return true;
    } 
    bool LoadPMTInput(IRecParaTool* m_pmtpara_tool) {
        nav = m_buf->curEvt();
        ///////////////////////////////////////
        // Initialize hit info.
        // Todo check useless 'for' circulation
        LHIT_Charge.clear();
        LHIT_Time.clear();
        LHIT_NPE.clear();
        Buff_PEQ.clear();

        LTOF.clear();
        for(int j=0;j<TotalLPMT;++j) {
            LHIT_Charge.push_back(0.);
            LHIT_Time.push_back(0.);
            LHIT_NPE.push_back(0.);
            LHIT_Charge_Buffer[j] = 0.;

            LTOF.push_back(0.);
        }
    
        SHIT_Charge.clear();
        SHIT_Time.clear();
        for(int j=0;j<TotalSPMT;++j) {
            SHIT_Time.push_back(0.);
            SHIT_Charge.push_back(0.);
        }

        double sumSpmtPE = 0.;
        double sumLpmtPE = 0.;
    
        m_Evt ++;
        simevt->GetEntry(m_Evt);
        double EvtR = sqrt(edepx*edepx + edepy*edepy + edepz*edepz);
        std::cout<< edepx << '\t' << edepy << '\t' << edepy << std::endl;
        for (int i=0;i<TotalLPMT;i++) {
            double LPMTX = ALL_LPMT_pos.at(i).X();
            double LPMTY = ALL_LPMT_pos.at(i).Y();
            double LPMTZ = ALL_LPMT_pos.at(i).Z();
                
            double Rsp = sqrt( (LPMTX-edepx)*(LPMTX - edepx) + (LPMTY-edepy)*(LPMTY-edepy) + (LPMTZ-edepz)*(LPMTZ-edepz));
            double CosThetaVC = (Rsp*Rsp + PMT_R*PMT_R*1.e6 - EvtR*EvtR)/(Rsp*PMT_R*1.e3*2.);
            double LengthWater = 1.e3*PMT_R*CosThetaVC - 1.e3*sqrt(PMT_R*CosThetaVC*PMT_R*CosThetaVC - PMT_R*PMT_R + LS_R*LS_R) ;

            LTOF[i] = 1.54*(Rsp - LengthWater)*1.e6/c + 1.63*LengthWater*1.e6/c;
        }
 
        for(int j=0;j<nPMT;++j) {
            int pmtid = PMTID->at(j);
            double charge = 1.;
            //if(hitTime_Toy[j]<2000.) continue;

            if(pmtid>TotalLPMT) {
                int spmtid = pmtid-ZeroSpmtID;
                SHIT_Charge[spmtid] += charge;
                sumSpmtPE += charge;
                if(SHIT_Time[spmtid]>=hitTime_Toy->at(j) || SHIT_Time[spmtid]==0.) {
                    SHIT_Time[spmtid] = hitTime_Toy->at(j);
                }
            } else {
                if(enableAddToyTTS) hitTime_Toy->at(j) = gRandom->Gaus(hitTime_Toy->at(j), m_pmtpara_tool->GetLTTS(pmtid));

                if(enableAddToyQ) {
                    charge = gRandom->Gaus(1.,  m_pmtpara_tool->GetLQRes(pmtid));
                    if(charge < PedThres) charge = 0.;
                }
                LHIT_Charge_Buffer[pmtid] += charge;
                sumLpmtPE += charge;
                if(LHIT_Time[pmtid]>=hitTime_Toy->at(j) || LHIT_Time[pmtid]==0.) {
                    LHIT_Time[pmtid] = hitTime_Toy->at(j);
                }
            }
            Buff_PEQ.push_back(charge);
        }
        LogInfo<<"True total SNPE and LNPE: " <<sumSpmtPE<<'\t'<<sumLpmtPE<<std::endl;

        if(enableAddToyDN) {
            // Add DN
            nDNphotons = 0.;
            DNhitTime.clear();
            DNhitQ.clear();
            DNpmtID.clear();
            for(int pmtid = 0; pmtid<TotalLPMT; pmtid ++) {
                double mud = 1.e-9*ADCL*m_pmtpara_tool->GetLDNR(pmtid);
                int    DC  = gRandom->Poisson(mud);
                if(DC==0) continue;

                for(int i=0;i<DC;i++) {
                    double q = 1.;
                    if(enableAddToyQ) {
                        q = gRandom->Gaus(1., m_pmtpara_tool->GetLQRes(pmtid));
                        if(q < PedThres) q = 0.;
                    }
                    nDNphotons ++;
                    DNhitQ.push_back(q);
                    DNhitTime.push_back(gRandom->Rndm()*ADCL + LTOF[pmtid] - 100.);
                    DNpmtID.push_back(pmtid);
                }
            }
            if(enableSPMTInfo) {
                for(int pmtid = 0; pmtid<TotalSPMT; pmtid ++) {
                    double mud = 1.e-9*ADCL*m_pmtpara_tool->GetSDNR(pmtid);
                    int    DC  = gRandom->Poisson(mud);
                    if(DC==0) continue;

                    for(int i=0;i<DC;i++) {
                        double q = 1.;
                        DNhitQ.push_back(q);

                        nDNphotons++;
                        DNhitTime.push_back(gRandom->Rndm()*ADCL);
                        DNpmtID.push_back(pmtid+ZeroSpmtID);
                    }
                }
            }
 
            for(int j=0;j<nDNphotons;j++) {
                int pmtid = DNpmtID[j];
                double charge = DNhitQ[j];
                if (pmtid>TotalLPMT) {
                    int spmtid = pmtid-ZeroSpmtID;
                    SHIT_Charge[spmtid] ++;
                    if(SHIT_Time[spmtid]>=DNhitTime[j] || SHIT_Time[spmtid]==0.) {
                        SHIT_Time[spmtid] = DNhitTime[j];
                    }
                } else {
                    LHIT_Charge_Buffer[pmtid] += charge;

                    if(LHIT_Time[pmtid]>=DNhitTime[j]||LHIT_Time[pmtid]==0.) {
                        LHIT_Time[pmtid] = DNhitTime[j];
                    }
                }
            }
            LogDebug<<"GenDNNum:" << nDNphotons << std::endl;
        }

        TotalPE = 0.;
        FiredPMT = 0;
    
        //std::ofstream npe_out;
        //npe_out.open("npe_check.txt", std::ios::out);
        for(int j=0;j<TotalLPMT;j++) {
            LHIT_Charge[j] = LHIT_Charge_Buffer[j];

            double q = LHIT_Charge[j];
            if(q < PedThres) LHIT_NPE[j] = 0;
            else if(q<1.5)   LHIT_NPE[j] = 1;
            else if(q<2.5)   LHIT_NPE[j] = 2;
            else if(q<3.5)   LHIT_NPE[j] = 3;
            else if(q<4.5)   LHIT_NPE[j] = 4;
            else if(q<5.5)   LHIT_NPE[j] = 5;
            else if(q<6.5)   LHIT_NPE[j] = 6;
            else if(q<7.5)   LHIT_NPE[j] = 7;
            else if(q<8.5)   LHIT_NPE[j] = 8;
            else if(q<9.5)   LHIT_NPE[j] = 9;
            else if(q<10.5)   LHIT_NPE[j] = 10;
            else LHIT_NPE[j] = 11;
            //npe_out<< LHIT_NPE[j] << std::endl;
            TotalPE += q;
            if(LHIT_Charge[j]!=0) FiredPMT++;
        }


        LogDebug  << "Done to read SimPMT:" << '\t' << TotalPE << '\t' << FiredPMT << std::endl;
 
 

        return true;
    }

private:
    static const int MaxHitNum = 2000000;
    static const int MaxLPmtNum = 20000;

    int ADCL;
    int ZeroSpmtID;
    double TotalLPMT;
    double TotalSPMT;
    bool enableAddToyQ;
    bool enableAddToyTTS;
    bool enableAddToyDN;
    bool enableSPMTInfo;
    double PedThres;
    double TotalPE;
    double FiredPMT;
    double LHIT_Charge_Buffer[MaxLPmtNum];

    CdGeom*  m_cdGeom;
    std::vector<TVector3> ALL_LPMT_pos; 
    float edepx;
    float edepy;
    float edepz;
    std::vector<double> LTOF;

    JM::NavBuffer* m_buf;
    std::list<JM::CalibPmtChannel*> chhlist;
    std::list<JM::CalibPmtChannel*> schhlist;
    JM::EvtNavigator* nav;

    std::string SimFile;

    int m_Evt;

    TFile* sfile;
    TTree* simevt;
    Int_t nPMT;
    std::vector<int>* PMTID;
    std::vector<double>* hitTime_Toy;

    std::vector<double> LHIT_Charge;
    std::vector<double> LHIT_Time;
    std::vector<double> LHIT_NPE;

    std::vector<double> SHIT_Charge;
    std::vector<double> SHIT_Time;

    int nDNphotons;
    std::vector<int> DNpmtID;
    std::vector<float> DNhitQ;
    std::vector<float> DNhitTime;
    std::vector<double> Buff_PEQ; // d_a2


};

double PMTToyInputTool::Get_LHIT_Charge(const int pmtid) {
    return LHIT_Charge.at(pmtid);
}
double PMTToyInputTool::Get_LHIT_Time(const int pmtid) {
    return LHIT_Time.at(pmtid);
}
double PMTToyInputTool::Get_LHIT_NPE(const int pmtid) {
    return LHIT_NPE.at(pmtid);
}

double PMTToyInputTool::Get_SHIT_Charge(const int pmtid) {
    return SHIT_Charge.at(pmtid);
}
double PMTToyInputTool::Get_SHIT_Time(const int pmtid) {
    return SHIT_Time.at(pmtid);
}

int PMTToyInputTool::Get_nDNphotons() {
    return nDNphotons;
}
int PMTToyInputTool::Get_DNpmtID(const int k) {
    return DNpmtID.at(k);
}
float PMTToyInputTool::Get_DNhitQ(const int k) {
    return DNhitQ.at(k);
}
float PMTToyInputTool::Get_DNhitTime(const int k) {
    return DNhitTime.at(k);
}
double PMTToyInputTool::Get_Buff_PEQ(const int k) { // d_a2
    return Buff_PEQ.at(k);
}


void PMTToyInputTool::Set_LHIT_Charge(const int pmtid, double charge) {
    LHIT_Charge[pmtid] = charge;
}
void PMTToyInputTool::Set_LHIT_Time(const int pmtid, double time) {
    LHIT_Time[pmtid] = time;
}
void PMTToyInputTool::Set_SHIT_Charge(const int pmtid, double charge) {
    SHIT_Charge[pmtid] = charge;
}
void PMTToyInputTool::Set_SHIT_Time(const int pmtid, double time) {
    SHIT_Time[pmtid] = time;
}

DECLARE_TOOL(PMTToyInputTool);
