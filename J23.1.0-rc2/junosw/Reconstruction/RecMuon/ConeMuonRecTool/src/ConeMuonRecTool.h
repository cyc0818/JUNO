#ifndef ConeMuonRecTool_h
#define ConeMuonRecTool_h
#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/ToolBase.h"
#include "Math/Minimizer.h"
#include "TProfile.h"
#include "TH2I.h"

class llhCone; 
class ConeMuonRecTool :public IRecMuonTool,  public ToolBase
{
    public:
        ConeMuonRecTool (const std::string& name);
        virtual ~ConeMuonRecTool ();

        std::vector<int>usedTable;
        std::vector<double>fhtTable;
        std::vector<int>typeTable;
        bool reconstruct(RecTrks*); 
        bool configure(const Params*, const PmtTable* ); 

    private:
        double fcnminuit(const std::vector<double>&par); 

        int iniargs(TVector3& r0, double& t0, TVector3& direction, double& length, double dfc);
        int iniWithTruth(TVector3& r0, TVector3& direction, double& length);
        int removePmts(double dfc, TVector3 firstLPMT, double toff, double nPEmean_L, double nPEmean_S, int& remLPMT, int& remSPMT);
        int fastAna(TVector3& firstLPMT, TVector3& firstSPMT, double& dfc, double& t0, double& t0_S, double& t0_fix, int& initLPMTused, int& initSPMTused );
        bool writeUserOutput(TVector3 re_R0, TVector3 re_dir, TVector3 re_Re, double minResult, double minVal);
        bool doTimeSmearing();
        bool doTypeTable();
        std::vector<int> getNeigbours(unsigned int pmtId, int type);

    private:
        double m_LSRadius;      //radius of LS
        double m_PMTRadius;     //radius of PMT center
        double m_PMTfix;        //half radius of PMT
        double m_clight;        //speed of light in vacuum
        double m_nLSlight;      //refraction of LS
        double m_vmuon;         //speed of muon
        bool m_cleaning;        // allow PMT Cleaning or not
        bool m_genUserOut;      // flag to write out small rec result
        std::string m_userOutfile; // small rec result filename
        TProfile* t0prof_L;       // t0 fix profile  
        TProfile* t0prof_S;       // t0 fix profile  
        TH2I* neighbour_LUT_L;    // LUT for PMT neighbours
        TH2I* neighbour_LUT_S;    // LUT for PMT neighbours
        double minResult;           // Minuit exit code
        double minVal;          // Mininmal value
        bool m_smearTimes;      // allow time smearing    
        double m_maxDiff;       // for cleaning: max deviation between ref PMT and neighbour mean
        double m_maxDiff_SPMT;       // for cleaning: max deviation between ref PMT and neighbour mean
        int m_totalpe;
        int m_totalpe_L;
        int m_totalpe_S;
        int m_SPMTcutTime;

        const PmtTable* m_ptable;
        std::string m_f1name; //name of TFile contain fix line; 
        std::string m_f2name; //name of TFile contain fix line; 

        ROOT::Math::Minimizer* minim;
        llhCone* fcn; 

};

#endif
