# Save Monte Carlo Truth into user data

As we introduced in [user actions management](framework.html#user-actions-management), analysis element is used to save Monte Carlo Truth into user data. In this part, we will have a quick look at existing analysis elements. Then we will learn to create a new one.

## Quick look at existing analysis elements
* **Location**: `offline/Simulation/DetSimV2/AnalysisCode` or `$ANALYSISCODEROOT`.

In this directory, there are some different analysis elements. They are created for different purposes. Most of them could be enabled or disabled. To save their own user data, a `SNiPER` service called `RootWriter` is used to save histograms or trees into ROOT files.

### Data Model Writer
* **Name**: `DataModelWriter`
* **Description**: 
    * Retrieve hits from hit collection and save hit (Q and t) in event data model format.
    * Save primary track information.

### User data of hit information
* **Name**: `NormalAnaMgr`
* **Description**: 
    * Save hit related Monte Carlo truth.
    * Attach `NormalTrackInfo` to track, so relationship between tracks and hits could be kept.

Please note here, because when a photon is re-emitted, the track id will be different. Using the technique of track information, we could pass the information of parent track to daughter track.

`NormalTrackInfo` is defined in `$SIMUTILROOT`. It will be also used by sensitive detector.

### User data of primary particles information
* **Name**: `GenEvtInfoAnaMgr`
* **Description**: 
    * Save primary tracks Monte Carlo truth.

### User data of deposited energy and visible energy
* **Name**: `DepositEnergyAnaMgr`
* **Description**: 
    * Save the deposited energy.
    * Calculate visible energy after quenching.

### Save Geometry
* **Name**: `GeoAnaMgr`
* **Description**: 
    * Export geometry to GDML.
    * Convert GDML to ROOT `TGeoManager` object.

### Save optical properties
* **Name**: `OpticalParameterAnaMgr`
* **Description**: 
    * Save several materials' optical properties.

### Select physics processes
* **Name**: `InteresingProcessAnaMgr`
* **Description**: 
    * Select neutron capture process.
    * Select muon decay and Michel electron.

### Muon tracks and isotopes
* **Name**: `MuProcessAnaMgr` and `MuIsoProcessAnaMgr`
* **Description**
    * Save muon track information.
    * Save isotopes generation induced by muon.

----

## Create a new analysis element
If you don't setup your own development environment, please have a look at [development environment setup](quickstart3.html#deploy-your-own-environment).

For example, we want to save positions of PMTs. You could get positions of PMTs in different ways. But in this part, we will try to traverse the geometry and then get the PMT's info.

### Count of daughter volume
Because at first, we don't know the name of PMTs, so we will develop an initial version to show count of each daughter volumes.

First, create a new file called `PMTPos.hh` in `offline/Simulation/DetSimV2/AnalysisCode/include`:

    #ifndef PMTPos_hh
    #define PMTPos_hh
    
    #include "SniperKernel/ToolBase.h"
    #include "DetSimAlg/IAnalysisElement.h"
    #include "TTree.h"
    
    class PMTPosAnaMgr: public IAnalysisElement,
                        public ToolBase {
    public:
    
        PMTPosAnaMgr(const std::string& name);
        ~PMTPosAnaMgr();
    
        // Run Action
        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);
        // Event Action
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);
    
    private:
        TTree* tree_pos;
        int copyno;
        double x;
        double y;
        double z;
    };
    
    #endif

Then create a new file called `PMTPos.cc` in `offline/Simulation/DetSimV2/AnalysisCode/src`:

    #include "PMTPos.hh"

    #include "G4Run.hh"
    #include "G4TransportationManager.hh"
    #include "G4Navigator.hh"
    #include "G4PhysicalVolumeStore.hh"
    #include "G4VPhysicalVolume.hh"
    #include "G4LogicalVolume.hh"
    
    #include "SniperKernel/SniperPtr.h"
    #include "SniperKernel/ToolFactory.h"
    #include "SniperKernel/SniperLog.h"
    #include "RootWriter/RootWriter.h"
    
    #include <map>
    
    DECLARE_TOOL(PMTPosAnaMgr);
    
    PMTPosAnaMgr::PMTPosAnaMgr(const std::string& name)
        : ToolBase(name) {
        tree_pos = 0;
        copyno = 0;
        x = 0;
        y = 0;
        z = 0;
    }
    
    PMTPosAnaMgr::~PMTPosAnaMgr() {
    
    }

    void 
    PMTPosAnaMgr::BeginOfRunAction(const G4Run*) {
        G4PhysicalVolumeStore* store = G4PhysicalVolumeStore::GetInstance();
        // try to get inner water
        G4VPhysicalVolume* inner_water = store->GetVolume("pInnerWater");
        LogInfo << "inner water pointer: " << inner_water << std::endl;
        if (!inner_water) {
            LogError << "can't find inner water." << std::endl;
            return;
        }
    
        G4LogicalVolume* lv_inner_water = inner_water->GetLogicalVolume();
        int daughter_count = lv_inner_water->GetNoDaughters();
    
        std::map<G4String, int> lv2cnt;
    
        // try to get PMT
        for (int i = 0; i < daughter_count; ++i) {
            const G4VPhysicalVolume* const physvol = lv_inner_water->GetDaughter(i);
            const G4String& phy_name = physvol->GetName();
    
            ++lv2cnt[phy_name];
    
        }
    
        // list count
        for (std::map<G4String, int>::iterator it = lv2cnt.begin();
                it != lv2cnt.end(); ++it) {
            LogInfo << " + " << it->first << " : " << it->second << std::endl;
        }
    }
    void 
    PMTPosAnaMgr::EndOfRunAction(const G4Run*) {
    
    }
    // Event Action
    void 
    PMTPosAnaMgr::BeginOfEventAction(const G4Event*) {
    
    }
    void 
    PMTPosAnaMgr::EndOfEventAction(const G4Event*) {
    
    }

Then build it:

    $ cd $ANALYSISCODEROOT/cmt
    $ cmt config
    $ source setup.sh
    $ cmt make

We could load this analysis element into simulation, for simplicity, we disable 3inch PMTs:

    python tut_detsim.py --no-gdml --anamgr-list PMTPosAnaMgr --evtmax 1 gun >& log_pmtpos.txt

You will get something looks like that:

    PMTPosAnaMgr.BeginOfRunAction   INFO:  + PMT_3inch_log_phys : 36572
    PMTPosAnaMgr.BeginOfRunAction   INFO:  + lFasteners_phys : 480
    PMTPosAnaMgr.BeginOfRunAction   INFO:  + lLowerChimney_phys : 1
    PMTPosAnaMgr.BeginOfRunAction   INFO:  + lMaskVirtual_phys : 17739
    PMTPosAnaMgr.BeginOfRunAction   INFO:  + lSteel_phys : 480
    PMTPosAnaMgr.BeginOfRunAction   INFO:  + lSurftube_phys : 1
    PMTPosAnaMgr.BeginOfRunAction   INFO:  + pAcylic : 1

So now, you will know the PMT name is `lMaskVirtual_phys`, and the count is 17739.

### Positions of PMTs

Update `PMTPosAnaMgr::BeginOfRunAction`:

    void 
    PMTPosAnaMgr::BeginOfRunAction(const G4Run*) {
        G4PhysicalVolumeStore* store = G4PhysicalVolumeStore::GetInstance();
        // try to get inner water
        G4VPhysicalVolume* inner_water = store->GetVolume("pInnerWater");
        LogInfo << "inner water pointer: " << inner_water << std::endl;
        if (!inner_water) {
            LogError << "can't find inner water." << std::endl;
            return;
        }
    
        // load root writer
        SniperPtr<RootWriter> svc("RootWriter");
        if (svc.invalid()) {
            LogError << "Can't Locate RootWriter. If you want to use it, please "
                     << "enalbe it in your job option file."
                     << std::endl;
            return;
        }
        tree_pos = svc->bookTree("SIMEVT/pmtpos", "PMT positions");
        tree_pos->Branch("pmtID", &copyno);
        tree_pos->Branch("x", &x);
        tree_pos->Branch("y", &y);
        tree_pos->Branch("z", &z);
    
        G4LogicalVolume* lv_inner_water = inner_water->GetLogicalVolume();
        int daughter_count = lv_inner_water->GetNoDaughters();
    
        std::map<G4String, int> lv2cnt;
    
        // try to get PMT
        for (int i = 0; i < daughter_count; ++i) {
            const G4VPhysicalVolume* const physvol = lv_inner_water->GetDaughter(i);
            const G4String& phy_name = physvol->GetName();
    
            ++lv2cnt[phy_name];
    
            // assume we know PMT name is lMaskVirtual_phys
            if (phy_name == "lMaskVirtual_phys") {
                copyno = physvol->GetCopyNo();
                const G4ThreeVector& pos = physvol->GetTranslation();
                x = pos.x();
                y = pos.y();
                z = pos.z();
    
                tree_pos->Fill();
            }
        }
    
        // list count
        for (std::map<G4String, int>::iterator it = lv2cnt.begin();
                it != lv2cnt.end(); ++it) {
            LogInfo << " + " << it->first << " : " << it->second << std::endl;
        }
    }


Then rebuild again. After you run script again, you can check it in user root file:

    $ root -l sample_detsim_user.root 
    root [0] 
    Attaching file sample_detsim_user.root as _file0...
    root [1] .ls
    TFile**         sample_detsim_user.root
     TFile*         sample_detsim_user.root
      KEY: TTree    evt;1   evt
      KEY: TH1I     stepno;1        step number of optical photons
      KEY: TTree    geninfo;1       Generator Info
      KEY: TTree    prmtrkdep;1     Deposit Energy Info for every primary track
      KEY: TTree    michael;1       evt
      KEY: TTree    nCapture;1      Neutron Capture
      KEY: TTree    opticalparam;1  Optical Parameters
      KEY: TTree    pmtpos;1        PMT positions
    root [2] pmtpos->Scan()
    ************************************************************
    *    Row   *     pmtID *         x *         y *         z *
    ************************************************************
    *        0 *         0 * 1065.4116 *         0 * 19470.873 *
    *        1 *         1 * 922.67351 * 532.70580 * 19470.873 *
    *        2 *         2 * 532.70580 * 922.67351 * 19470.873 *

You can compare the result with GDML file.

          <physvol name="lMaskVirtual_phys0x15d6ad0">
            <volumeref ref="lMaskVirtual0x157d180"/>
            <position name="lMaskVirtual_phys0x15d6ad0_pos" unit="mm" x="1065.41160578968" y="0" z="19470.8730700564"/>
            <rotation name="lMaskVirtual_phys0x15d6ad0_rot" unit="deg" x="180" y="3.132" z="180"/>
          </physvol>
          <physvol name="lMaskVirtual_phys0x15d4690">
            <volumeref ref="lMaskVirtual0x157d180"/>
            <position name="lMaskVirtual_phys0x15d4690_pos" unit="mm" x="922.673516100635" y="532.70580289484" z="19470.8730700564"/>
            <rotation name="lMaskVirtual_phys0x15d4690_rot" unit="deg" x="-178.432829276069" y="2.7120535678454" z="-149.962900324933"/>
          </physvol>
