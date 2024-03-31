#ifndef DataModelWriter_hh
#define DataModelWriter_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IAnalysisElement.h"

#include "junoHit_PMT.hh"
#include "junoHit_PMT_muon.hh"
#include "Event/SimHeader.h"
#include "Geometry/IPMTParamSvc.h"
#include "Geometry/PMT.h"
#include "junoHit_TT.hh"

class ISimTrackSvc;
class G4Event;
namespace JM {
    class SimEvt;
}

class Task;

class DataModelWriterWithSplit : public IAnalysisElement,
                       public ToolBase{

public:
    DataModelWriterWithSplit(const std::string& name);
    ~DataModelWriterWithSplit();

    // Run Action
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    // Event Action
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
private:

    // fill hits
    void fill_hits(JM::SimEvt* dst, const G4Event* evt);
    template<typename T>
    void fill_hits_tmpl(G4THitsCollection<T>* col, JM::SimEvt* dst) {
        int n_hit = col->entries();
        // m_nPhotons = n_hit;
        int count = 0;
        for (int i = m_start_idx; (i < n_hit and count < m_hitcol_max); 
                ++m_start_idx, ++i, ++count) {
            // create new hit
            // The PMT Hit can be from WP (Water Pool) or CD (Central
            // Detector). 
            // Please use the copy no to distinguish the PMT.
            int copyno = (*col)[i]->GetPMTID();
            JM::SimPMTHit* jm_hit = 0;
            // FIXME: hard code the copy no
            if (PMT::IsCD(copyno)) {
                // TODO because in current Data Model, the 3inch and the 20inch
                // PMTs are in the same collection.
                jm_hit = dst->addCDHit();
            } else if (PMT::IsWP(copyno)) {
                jm_hit = dst->addWPHit();
            }
            jm_hit->setPMTID( (*col)[i]->GetPMTID() );
            jm_hit->setNPE( (*col)[i]->GetCount() );
            jm_hit->setHitTime( (*col)[i]->GetTime() );
            jm_hit->setTrackID( (*col)[i]->GetProducerID() );
            jm_hit->setLocalTheta( (*col)[i]->GetTheta() );
            jm_hit->setLocalPhi( (*col)[i]->GetPhi() );            

        }
    }
    // fill tracks
    void collect_primary_track(const G4Event* evt);
    void fill_tracks(JM::SimEvt* dst);
private:
    int m_start_idx;
    int m_hitcol_max;
    bool m_disable_split;
    bool m_savesimtrack;
private:
    std::string iotaskname;
    Task* iotask;
    ISimTrackSvc* simtracksvc;
private:
    IPMTParamSvc*  m_pmt_param_svc;

    // Fill hits TopTracker
    int channel;
    double edep;
    double time;

    template<typename T>
    void fill_hits_tt(G4THitsCollection<T>* col_tt, JM::SimEvt* dst) {
      int nhit = col_tt->entries();

      for(int kk=0; kk<nhit; kk++)
        {
          channel = (*col_tt)[kk]->GetChannelID();
          edep = (*col_tt)[kk]->GetEdep();
          time = (*col_tt)[kk]->GetTime();

          auto tt_hit = dst->addTTHit();
          tt_hit->setChannelID(channel);
          tt_hit->setEdep(edep);
          tt_hit->setTime(time);

        }
    }
};

#endif
