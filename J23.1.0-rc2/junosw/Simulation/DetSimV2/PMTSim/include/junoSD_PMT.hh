//--------------------------------------------------------------------------
//                            junoSD_PMT
//
// PMTs are difined as sensitive detector. They collect hits on them.
// The data members of hits are set up here using the information of G4Step.
// -------------------------------------------------------------------------
// Author: Liang Zhan, 2006/01/27
// Modified by: Weili Zhong, 2006/03/01
// -------------------------------------------------------------------------

#ifndef junoSD_PMT_h
#define junoSD_PMT_h 1

#include "G4VSensitiveDetector.hh"
#include "G4ThreeVector.hh"
#include "junoHit_PMT.hh"
#include "IToolForSD_PMT.h"
#include <map>

//////////////////////////////////////////////////////////////////////////

class G4Step;
class G4HCofThisEvent;

class junoSD_PMT : public G4VSensitiveDetector, public IToolForSD_PMT
{
  public:
    junoSD_PMT(const std::string& name);
    ~junoSD_PMT();

    void Initialize(G4HCofThisEvent*HCE);
    G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
    void EndOfEvent(G4HCofThisEvent*HCE);

    void clear();
    void DrawAll();
    void PrintAll();
    void SimpleHit( const ParamsForSD_PMT& );

    void setMergeFlag(bool f) {
        m_merge_flag = f;
    }
    void setMergeWindows(double t) {
        m_time_window = t;
    }
  private:
    // * true: merge success
    // * false: 
    bool doMerge( const ParamsForSD_PMT& );
  private:
    junoHit_PMT_Collection* hitCollection;
  private:
    bool m_debug;
    bool m_merge_flag;
    // the time window is used when merge is enabled.
    double m_time_window;

    typedef std::multimap<int, int> PMTID2COLIDS;
    typedef std::pair< PMTID2COLIDS::iterator, PMTID2COLIDS::iterator > PMTITER;
    PMTID2COLIDS m_pmtid2idincol;
    PMTITER m_pmt_iter;
};

#endif

