
#include "OMILREC/IRecTool.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include <fstream>
#include <vector>
#include <string>


class PMTRealInputTool: public ToolBase, public IRecInputTool {

public:
    PMTRealInputTool(const std::string& name): ToolBase(name),
                                               m_evt(nullptr), 
                                               m_sevt(nullptr) {
    }

    ~PMTRealInputTool() {

    }
    

    const std::list<JM::CalibPmtChannel*>& Get_chhlist() {
        return m_evt->calibPMTCol();;
    }
    const std::list<JM::CalibPmtChannel*>& Get_schhlist() {
        return m_sevt->calibPMTCol();;
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
        return 0; //No nPMT for the real case
    }
    Int_t  Get_PMTID(const int k) {
        return 0; //No PMTID for the real case
    }
    double Get_hitTime_Toy(const int k) {
        return 0.; //No hitTime_Toy for the real case
    }
 
    int    Get_nDNphotons() {
        return 0.; //No nDNphotons for the real case
    }
    int    Get_DNpmtID(const int k) {
        return 0.; //No DNpmtID for the real case
    }
    float  Get_DNhitQ(const int k) {
        return 0.; //No DNhitQ for the real case
    }
    float  Get_DNhitTime(const int k) {
        return 0.; //No DNhitTime for the real case
    }
    double Get_Buff_PEQ(const int k) { // d_a2
        return 0.; //No Buff_PEQ for the real case
    }

    void Set_LHIT_Charge(const int pmtid, const double charge);
    void Set_SHIT_Charge(const int pmtid, const double charge);
    void Set_LHIT_Time(const int pmtid, const double time);
    void Set_SHIT_Time(const int pmtid, const double time);
    

    bool init() {
        PedThres = 0.1;

        // Event navigator
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
        TotalLPMT = rgSvc->getCdGeom()->findPmt20inchNum();
        TotalSPMT = rgSvc->getCdGeom()->findPmt3inchNum();
 

        LogInfo << "Hello! I am in the PMTRealInputTool ! "<< std::endl;
        return true;
    } 
    bool LoadPMTInput(IRecParaTool* m_pmtpara_tool) {
                            

        nav = m_buf->curEvt();
        // Read CalibHit data

        auto chcol = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav);
        auto schcol = JM::getHeaderObject<JM::CdSpmtCalibHeader>(nav);

        if (!chcol) {
            LogWarn << "The CdLpmtCalibHeader does not exist in this event. " << std::endl;
            return false;
        }
        if (!chcol->event()) {
            LogWarn << "The CdLpmtCalibEvent does not exist in this event. " << std::endl;
            return false;
        }
        if (!schcol) {
            LogWarn << "The CdSpmtCalibHeader does not exist in this event. " << std::endl;
            return false;
        }
        if (!schcol->event()) {
            LogWarn << "The CdSpmtCalibEvent does not exist in this event. " << std::endl;
            return false;
        }

        m_evt = chcol->event();
        m_sevt = schcol->event();

        // Read hit information
        const auto& chhlist = m_evt->calibPMTCol();
        const auto& schhlist = m_sevt->calibPMTCol();


        LHIT_Charge.clear();
        LHIT_Time.clear();
        LHIT_NPE.clear();
        for(int j=0;j<TotalLPMT;++j) {
            LHIT_Charge.push_back(0.);
            LHIT_Time.push_back(0.);
            LHIT_NPE.push_back(0.);
        }
        SHIT_Charge.clear();
        SHIT_Time.clear();
        for(int j=0;j<TotalSPMT;++j) {
            SHIT_Time.push_back(0.);
            SHIT_Charge.push_back(0.);
        }

        TotalPE = 0.;
        TotalSPE = 0.;
        FiredPMT = 0.;
        std::list<JM::CalibPmtChannel*>::const_iterator chit = chhlist.begin();

        while (chit!=chhlist.end()) {
            const JM::CalibPmtChannel  *calib = *chit++;

            unsigned int pmtId = calib->pmtId();
            Identifier id = Identifier(pmtId);
            if (CdID::is20inch(id)) {
                unsigned int pmtid = CdID::module(id);
                double firstHitTime = calib->firstHitTime();
                LHIT_Time[pmtid] = firstHitTime;
 
                double q = calib->nPE();
                LHIT_Charge.at(pmtid) = q;


                double Q1 = m_pmtpara_tool->GetLGain(pmtid);
                q /= Q1;
                // LHIT_NPE is meaningful for time usage in 
                // the time likelihood, the special setting here 
                // need to be corrected when waveform reconstruction 
                // is optimized.
                if(q<PedThres) LHIT_Charge.at(pmtid) = 0;
                if(q<PedThres) LHIT_NPE[pmtid] = 0;
                else if(q<1.5) LHIT_NPE[pmtid] = 1;
                else if(q<2.5) LHIT_NPE[pmtid] = 2;
                else if(q<3.5) LHIT_NPE[pmtid] = 3;
                else if(q<4.5) LHIT_NPE[pmtid] = 4;
                else if(q<5.5) LHIT_NPE[pmtid] = 5;
                else if(q<6.5) LHIT_NPE[pmtid] = 6;
                else if(q<7.5) LHIT_NPE[pmtid] = 6; //!
                else if(q<8.5) LHIT_NPE[pmtid] = 6; //!
                else if(q<9.5) LHIT_NPE[pmtid] = 6; //!
                else if(q<10.5) LHIT_NPE[pmtid] = 6; //!
                else LHIT_NPE[pmtid] = 6;
                TotalPE += q;
                if(q!=0) FiredPMT++;
            } 
        }
        std::list<JM::CalibPmtChannel*>::const_iterator schit = schhlist.begin();
        while (schit!=schhlist.end()) {
            const JM::CalibPmtChannel  *calib = *schit++;

            unsigned int pmtId = calib->pmtId();
            Identifier id = Identifier(pmtId);
            if (CdID::is3inch(id)) {
                unsigned int pmtid = CdID::module(id);
                double firstHitTime = calib->firstHitTime();
                SHIT_Time[pmtid-TotalLPMT] = firstHitTime;

                double q = calib->nPE();
                SHIT_Charge.at(pmtid-TotalLPMT) = q;
                TotalSPE += q;
            }
        } 
        LogDebug  << "Done to read CalibPMT:" << '\t' << TotalPE << '\t' << FiredPMT << '\t' << TotalSPE <<std::endl;
 
        return true;
    }

private:
    JM::NavBuffer* m_buf;

    double PedThres;
    double TotalLPMT;
    double TotalSPMT;
    double TotalPE;
    double TotalSPE;
    double FiredPMT;
    // FIXME: Instead of copy the whole list, cache the corresponding event
    // Tao Lin, 2022.03.08

    JM::CdLpmtCalibEvt* m_evt;
    JM::CdSpmtCalibEvt* m_sevt;
    // std::list<JM::CalibPMTChannel*> chhlist;
    // 
    JM::EvtNavigator* nav;

    std::vector<double> LHIT_Charge;
    std::vector<double> LHIT_Time;
    std::vector<double> LHIT_NPE;

    std::vector<double> SHIT_Charge;
    std::vector<double> SHIT_Time;



};

double PMTRealInputTool::Get_LHIT_Charge(const int pmtid) {
    return LHIT_Charge.at(pmtid);
}
double PMTRealInputTool::Get_LHIT_Time(const int pmtid) {
    return LHIT_Time.at(pmtid);
}
double PMTRealInputTool::Get_LHIT_NPE(const int pmtid) {
    return LHIT_NPE.at(pmtid);
}

double PMTRealInputTool::Get_SHIT_Charge(const int pmtid) {
    return SHIT_Charge.at(pmtid);
}
double PMTRealInputTool::Get_SHIT_Time(const int pmtid) {
    return SHIT_Time.at(pmtid);
}


void PMTRealInputTool::Set_LHIT_Charge(const int pmtid, const double charge) {
    LHIT_Charge[pmtid] = charge;
}
void PMTRealInputTool::Set_LHIT_Time(const int pmtid, const double time) {
    LHIT_Time[pmtid] = time;
}
void PMTRealInputTool::Set_SHIT_Charge(const int pmtid, const double charge) {
    SHIT_Charge[pmtid] = charge;
}
void PMTRealInputTool::Set_SHIT_Time(const int pmtid, const double time) {
    SHIT_Time[pmtid] = time;
}


DECLARE_TOOL(PMTRealInputTool);
