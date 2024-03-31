//--------------------------------------------------------------------------
//                            junoSD_PMT_v2
//
// PMTs are difined as sensitive detector. They collect hits on them.
// The data members of hits are set up here using the information of G4Step.
// -------------------------------------------------------------------------
// Author: Liang Zhan, 2006/01/27
// Modified by: Weili Zhong, 2006/03/01
// -------------------------------------------------------------------------

#ifndef junoSD_PMT_v2_h
#define junoSD_PMT_v2_h 1

#include "G4VSensitiveDetector.hh"
#include "G4ThreeVector.hh"
#include "junoHit_PMT.hh"
#include "junoHit_PMT_muon.hh"
#include "IToolForSD_PMT.h"
#include "PMTHitMerger.hh"
#include <map>
#include <vector>
#include <TF1.h>
#include "Geometry/PMTParamSvc.h"
#include "IPMTSimParamSvc/IPMTSimParamSvc.h"
//#include "PMTSimParamSvc/PMTSimParamSvc.h"
//////////////////////////////////////////////////////////////////////////

class G4Step;
class G4Track;
class G4HCofThisEvent;

#ifdef WITH_G4OPTICKS
class PMTEfficiency ; 
class PMTEfficiencyTable ; 
#endif

class junoSD_PMT_v2 : public G4VSensitiveDetector, public IToolForSD_PMT
{
    public:
        junoSD_PMT_v2(const std::string& name, int opticksMode);
        ~junoSD_PMT_v2();

        void Initialize(G4HCofThisEvent*HCE);
        G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
        void EndOfEvent(G4HCofThisEvent*HCE);
        void EndOfEvent_Opticks(G4HCofThisEvent*HCE);

        void clear();
        void DrawAll();
        void PrintAll();
        void SimpleHit( const ParamsForSD_PMT& );

        void setCEMode(const std::string& mode);
        void setCEFlatValue(double v) {m_ce_flat_value = v;}
        void setMergeFlag(bool f) { m_merge_flag = f; }
        void setMergeWindows(double t) { m_time_window = t; }
        void setMerger(PMTHitMerger* phm) { m_pmthitmerger=phm; }
        void setMergerOpticks(PMTHitMerger* phm) { m_pmthitmerger_opticks=phm; }
        void setPMTParamSvc(PMTParamSvc* para){ m_PMTParamsvc=para; }
        PMTParamSvc* getPMTParamSvc() const { return m_PMTParamsvc ; }
        void setPMTSimParamSvc(IPMTSimParamSvc* para){ m_PMTSimParsvc=para; }
        IPMTSimParamSvc* getPMTSimParamSvc() const { return m_PMTSimParsvc ; }

        void setHitType(int i) { m_hit_type = i; }
        int getHitType() { return m_hit_type; }

        void disableSD() { m_disable = true; }
        void enableSD() { m_disable = false; }

        void setCEFunc(const std::string& func, const std::vector<double>& param);
        double getEfficiencyScale() const ; 

        void enableOpticalModel() { m_enable_optical_model = true; }

    private:
        int get_pmtid(G4Track*);
        double get_ce(const std::string& volname, const G4ThreeVector& localpos, bool pmt_type, bool qe_type);
    private:
        junoHit_PMT_Collection* hitCollection;
        junoHit_PMT_muon_Collection* hitCollection_muon;
        junoHit_PMT_Collection* hitCollection_opticks ;
    private:
        bool m_debug;
        std::string m_ce_mode;
        double m_qescale ; 
        double m_angle_response ; 

        // if flat mode enabled, this is used to set the fixed number
        double m_ce_flat_value;
        double MCP20inch_m_ce_flat_value;
        double MCP8inch_m_ce_flat_value;
        double Ham20inch_m_ce_flat_value;
        double Ham8inch_m_ce_flat_value;
        double HZC9inch_m_ce_flat_value;

        double MCP20inch_m_EAR_value;
        double MCP8inch_m_EAR_value;
        double Ham20inch_m_EAR_value;
        double Ham8inch_m_EAR_value;
        double HZC9inch_m_EAR_value;

        // 20inchfunc mode, function mode
        std::string m_ce_func_str;
        std::vector<double> m_ce_func_params;
        TF1* m_ce_func;

        //  20inch mode, interpolation mode
        double eff_ce_Hamamatsu = 0.872;
        double eff_ce_NNVT_Normal = 0.916;
        double eff_ce_NNVT_HiQE = 0.957;

        // flag to enable/disable the sensitive detector
        // disable is true, means disable the SD.
        bool m_disable;

        bool m_enable_optical_model;
    private:
        // ========================================================================
        // merge related
        // ========================================================================
        // = merge flag 
        bool m_merge_flag;
        int  m_merge_count ; 
        // = the time window is used when merge is enabled.
        double m_time_window;

        typedef std::multimap<int, int> PMTID2COLIDS;
        typedef std::pair< PMTID2COLIDS::iterator, PMTID2COLIDS::iterator > PMTITER;
        PMTID2COLIDS m_pmtid2idincol;

        // new merger
        PMTHitMerger* m_pmthitmerger;
        PMTHitMerger* m_pmthitmerger_opticks;
        PMTParamSvc* m_PMTParamsvc;
        IPMTSimParamSvc* m_PMTSimParsvc;
        // ========================================================================
        // Hit Collection switcher
        // * normal, junoHit_PMT
        // * for muon, junoHit_PMT_muon
        // ========================================================================
        G4int m_hit_type;
        G4int m_opticksMode; 
#ifdef WITH_G4OPTICKS
    private:
        PMTEfficiency*      m_PMTEfficiency ;  
        PMTEfficiencyTable* m_PMTEfficiencyTable ;  
    public:
        double              getQuantumEfficiencyScale() const ;
        double              getQuantumEfficiency(int pmtID) const ;
        double              getCollectionEfficiency(double theta, int pmtID) const ;  
        double              getDetectionEfficiency(double theta, int pmtID) const ;  
        PMTEfficiencyTable* getPMTEfficiencyTable() const  ;
#endif
};

#endif

