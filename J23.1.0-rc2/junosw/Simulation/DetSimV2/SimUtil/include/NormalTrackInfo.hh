#ifndef NormalTrackInfo_hh
#define NormalTrackInfo_hh

/*
 * Description:
 *   This class is used to store the extended track information. 
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 */

#include "globals.hh"
#include "G4VUserTrackInformation.hh"

#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include <vector>
#include <map>
#include <bitset>

// Ref: http://geant4.slac.stanford.edu/Tips/

class NormalTrackInfo: public G4VUserTrackInformation {
    public:
    // In order to reduce the memory usage, the bit map is used here.
    // Each bit is controlled by the enum value. 
    // 
    // If user needs to extend the flags, just extend following enum. 
    //
    // -- Tao Lin, 11 Feb 2023
    enum {
        kInterestedProcess=0, // bit=1, will be saved
        kPhysMichelElectron,
        kPhysNeutronCapture,
        kPhysDecay,
        kPhotonFromCerenkov,
        kPhotonIsReemission,
        kPhotonIsOriginalOP,
        NFlags // ALWAYS KEEP IT AT END
    };

    public:
        NormalTrackInfo();
        NormalTrackInfo(const G4Track* aTrack);
        NormalTrackInfo(const NormalTrackInfo* aTrackInfo);
        virtual ~NormalTrackInfo(); 


        inline void *operator new(size_t);
        inline void operator delete(void *aTrackInfo);
        inline int operator ==(const NormalTrackInfo& right) const {
            return (this==&right);
        }
    public:
        inline G4int GetOriginalTrackID() const {return originalTrackID;}

        inline void markTracedAncestors(G4int trkid) { tracedAncestors.push_back(trkid); }
        inline const std::vector<G4int>& getTracedAncestors() const { return tracedAncestors; }

        inline G4int getOriginalVertexIDX() const { return originalVertexIDX; }
        inline void setOriginalVertexIDX(G4int vid) { originalVertexIDX = vid; }

        inline void setInterestedProcess() { m_flags[kInterestedProcess] = 1; }
        inline G4int isInterestedProcess() { return m_flags[kInterestedProcess]; }

        inline void setMichelElectron() {m_flags[kPhysMichelElectron] = 1;}
        inline G4int isMichelElectron() {return m_flags[kPhysMichelElectron];}

        inline void setNeutronCapture() {m_flags[kPhysNeutronCapture] = 1;}
        inline G4int isNeutronCapture() {return m_flags[kPhysNeutronCapture];}

        // In the G4, all the decay physics should be implemented in G4Decay. 
        // If we need to store the different decay mode, then define the number schema.
        // By default, we use mode 0 for no decay, 1 for general decay.
        // It could be easily extented by defining different numbers.
        inline void setDecayMode(int mode=1) { m_flags[kPhysDecay] = mode; }
        inline G4int getDecayMode()  { return m_flags[kPhysDecay]; }

        inline void setFromCerenkov() {m_flags[kPhotonFromCerenkov] = true;}
        inline G4bool isFromCerenkov(){return m_flags[kPhotonFromCerenkov];}

        inline void setReemission() {m_flags[kPhotonIsReemission] = true;}
        inline G4bool isReemission(){return m_flags[kPhotonIsReemission];}


        inline void setOriginalOP() { m_flags[kPhotonIsOriginalOP] = true; }
        inline G4bool isOriginalOP() { return m_flags[kPhotonIsOriginalOP]; }
        inline void setOriginalOPStartTime(double t) {m_op_start_time = t;}
        inline double getOriginalOPStartTime() { return m_op_start_time; }

        inline void setBoundaryPos(const G4ThreeVector& pos) { pos_at_boundary = pos; }
        inline const G4ThreeVector& getBoundaryPos() const { return pos_at_boundary; }

        inline void setParentName(const G4String& parent_name) { m_parent_name = parent_name; }
        inline const G4String& getParentName() const { return m_parent_name; }

    private:
        G4int originalTrackID;
        std::vector<G4int> tracedAncestors;
        G4int originalVertexIDX; // the vertex idx in the sim track service. 
                                 // only the track created from the vertex directly are saved. 

        // special physics related
        std::bitset<NFlags> m_flags;

        // G4int phys_michel_electron;
        // G4int phys_neutron_capture;
        // G4int phys_decay_mode;

        // G4bool from_cerenkov;
        // G4bool is_reemission;

        // the original OP info
        // Here, original means the parent of OP is not OP.
        // G4bool m_op_is_original_op;
        G4double m_op_start_time;

        G4ThreeVector pos_at_boundary;

        // info of Parent Track
        G4String m_parent_name;
};

extern G4ThreadLocal G4Allocator<NormalTrackInfo>* aNormalTrackInformationAllocator;


inline void* NormalTrackInfo::operator new(size_t)
{ 
    void* aTrackInfo;
    if (!aNormalTrackInformationAllocator) {
        aNormalTrackInformationAllocator = new G4Allocator<NormalTrackInfo>;
    }
    aTrackInfo = (void*)aNormalTrackInformationAllocator->MallocSingle();
    return aTrackInfo;
}

inline void NormalTrackInfo::operator delete(void *aTrackInfo)
{ aNormalTrackInformationAllocator->FreeSingle((NormalTrackInfo*)aTrackInfo);}

#endif
