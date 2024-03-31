#include "Minuit2/FCNBase.h"
#include "RecTools/PmtProp.h"
#include "TMath.h"

class TGraph; 
class llhCone : public ROOT::Minuit2::FCNBase
{
    public:
        llhCone (const PmtTable*);
        void setLKDE(TGraph *KDE_c[18],TGraph *KDE_s[18],TGraph *KDE_ch[18],TGraph *KDE_fs[18]); 
        void setSKDE(TGraph *KDE_c[18],TGraph *KDE_s[18],TGraph *KDE_ch[18],TGraph *KDE_fs[18]); 
        void setclight(double c);
        void setnLS(double n);
        void setvmuon(double v);
        void setused(std::vector<int> usedTable);
        void setFhtTable(std::vector<double> fhtTable);
        //void setTypeTable(std::vector<int> typeTable);

        double Up() const{return 0.5;};
        double operator()(const double * par)const;  
        double operator()(const std::vector<double>&par) const;

    private:
        double conefunc(TVector3 r0,// start point of the track
                TVector3 V,             // speed of the track particle
                double t0,              //start time of the track
                double dfc,             // distance from center
                const PmtProp& pmt,     // pmt
                int inum,               // pmtId
                int low,                // identifier for KDE
                int up,                 // identifier for KDE
                int pdf_id              // identifier for scintillation cone or cherenkov cone
                ) const ;
        double spherefunc(TVector3 link,// link between r0 and pmt
                double t0,              //start time of the track
                double dfc,             // distance from center
                const PmtProp& pmt,     // pmt
                int inum,               // pmtId
                int low,                // identifier for KDE
                int up,                 // identifier for KDE
                int pdf_id              // idenifier for backward and forward sphere
                ) const ;
        double fastEval(TGraph* g, double input) const;
        double ASinCont(double x) const;

    private:
        const PmtTable* m_ptable;
        TGraph* m_L_KDE_c[2][18];
        TGraph* m_L_KDE_s[2][18];

        TGraph* m_S_KDE_c[2][18];
        TGraph* m_S_KDE_s[2][18];

        double m_nLSlight;
        double m_clight; //unit: mm/ns
        double m_vmuon;
        std::vector<int> m_usedTable;
        //std::vector<int> m_typeTable;
        std::vector<double> m_fhtTable;
        double ang_c_ls;
        double ang_c_ch;
};
