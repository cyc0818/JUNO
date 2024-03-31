#ifndef DepEnAnaMgr_hh
#define DepEnAnaMgr_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IAnalysisElement.h"
#include "TTree.h"
#include <vector>
#include <map>
#include <set>

class BirksLawCalculator;

class DepEnAnaMgr: public IAnalysisElement,
                               public ToolBase{
public:

    DepEnAnaMgr(const std::string& name);
    ~DepEnAnaMgr();
    // Run Action
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    // Event Action
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    virtual void PreUserTrackingAction(const G4Track* aTrack);
    virtual void PostUserTrackingAction(const G4Track* aTrack);

    virtual void UserSteppingAction(const G4Step* step);

private:
    void isotope_initialize();
private:
    BirksLawCalculator* m_birksLawCalculator;

    // Evt Data
    int m_eventID;
    
    // - neutron capture in LS? TODO: Only select the specific material
    // -- detron + gamma (2.2MeV)
    // -- C13    + gamma (4.95MeV)
    // -- C13    + gamma (3.68MeV)
    //           + gamma (1.26MeV)

    TTree* m_AN_tree;
    static const int maxParticleN=100;
    int m_ParticleMult;
    int m_maxParticleMult;
    double m_Edep;
    double m_QEdep;
    double m_Edep_x;
    double m_Edep_y;
    double m_Edep_z;
    double m_QEdep_x;
    double m_QEdep_y;
    double m_QEdep_z;
    double m_edep_step;
    double m_Qedep_step;
    double m_Water_Edep;
    double m_vetoWater_Edep;
    int m_InitPDGID[100];
    double m_InitPosx[100];
    double m_InitPosy[100];
    double m_InitPosz[100];
    double m_ExitPosx[100];
    double m_ExitPosy[100];
    double m_ExitPosz[100];
    double m_InitPx[100];
    double m_InitPy[100];
    double m_InitPz[100];
    double m_TrackLength[100];
    double m_InitKineticE[100];
    double m_InitEdepInLS[100];
    double m_InitQEdepInLS[100];
    double m_InitLossEInLS[100];
    double m_InitTrackLengthInLS[100];
    double m_InitEdepOutLS[100];
    double m_InitTrackLengthOutLS[100];
    double m_InitEdepInVetoWater[100];
    double m_InitTrackLengthInVetoWater[100];
    double m_InitEdepInCDWater[100];
    double m_InitTrackLengthInCDWater[100];
    int m_SecondaryMult[100];
    static const int maxSecondaryMult=500;
    std::vector<std::vector<int> > m_SecPDGID;
    std::vector<std::vector<double> > m_SecKineticE;
    std::vector<std::vector<double> > m_SecGlobalT;
    std::vector<std::vector<double> > m_SecLocalT;
    std::vector<std::vector<double> > m_SecStartPosx;
    std::vector<std::vector<double> > m_SecStartPosy;
    std::vector<std::vector<double> > m_SecStartPosz;
    std::vector<std::vector<int> > m_SecParentMaterial;
    std::vector<std::vector<int> > m_SecParentName;
    std::vector<std::vector<int> > m_SecProcessName;
    std::vector<int> InitMult;
    std::vector<double> InitMult_temp;
    std::vector<int> SecPDG;
    std::vector<int> SecMaterial;
    std::vector<int> SecParName;
    std::vector<int> SecProName;
    std::vector<double> SecE;
    std::vector<double> SecGT;
    std::vector<double> SecLT;
    std::vector<double> Secx;
    std::vector<double> Secy;
    std::vector<double> Secz;

    unsigned int m_nSecondaries; 
    std::set<TString> m_isotopes;
    std::map<int, TString> m_IsotopeMotherName;

    TTree* m_AN_Isotope_tree;
    double m_IsoX;
    double m_IsoY;
    double m_IsoZ;
    double m_IsoMomX;
    double m_IsoMomY;
    double m_IsoMomZ;
    double m_IsoKineE;
    double m_IsoLocalTime;
    double m_IsoGlobalTime;
    std::vector<TString>* m_IsoName;
    std::vector<TString>* m_IsoGenProcName;
    std::vector<TString>* m_IsoGenMaterialName;
    std::vector<TString>* m_IsoPostProcName;
    std::vector<TString>* m_IsoMotherName;
    int m_IsoSecndaryNum;
    std::vector<double>* m_IsoSecKineE;
    std::vector<TString>* m_IsoSecName; 
    std::vector<double>* m_IsoSecLocalTime;
    std::vector<double>* m_IsoSecGlobalTime;
 
};

#endif
