#ifndef GtGenieAtmoEvGenTool_h
#define GtGenieAtmoEvGenTool_h

/*
 * Description:
 *   This gentool is based on the example from GENIE:
 *     - Generator-R-3_02_00/src/Apps/gAtmoEvGen.cxx
 *
 *   Integrate an external applications with SNiPER.
 *
 * Author:
 *   Jie Cheng <chengjie AT ncepu.edu.cn>
 */

// ===========================================================================
// BELOW IS FOR GenTool
// ===========================================================================
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "GenTools/IGenTool.h"
#include "RootWriter/RootWriter.h"
#include <boost/python.hpp>
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

#include <memory>

// ===========================================================================
// END FRAMEWORK PART.
// ===========================================================================

// ===========================================================================
// BELOW IS FOR GENIE
// ===========================================================================
#include <cassert>
#include <cstdlib>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <iomanip>
#include <cmath>

#include <TRotation.h>
#include <TMath.h>
#include <TGeoShape.h>
#include <TGeoBBox.h>

#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TVector3.h>
#include <TH1.h>
#include <TF1.h>
#include <TString.h>

#include "Framework/Conventions/Units.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/EventGen/GFluxI.h"
#include "Framework/EventGen/GMCJDriver.h"
#include "Framework/EventGen/GMCJMonitor.h"
#include "Framework/Messenger/Messenger.h"
#include "Framework/Ntuple/NtpWriter.h"
#include "Framework/Ntuple/NtpMCFormat.h"
#include "Framework/Numerical/RandomGen.h"
#include "Framework/ParticleData/PDGCodes.h"
#include "Framework/ParticleData/PDGLibrary.h"
#include "Framework/Utils/XSecSplineList.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/SystemUtils.h"
#include "Framework/Utils/UnitUtils.h"
#include "Framework/Utils/CmdLnArgParser.h"
#include "Framework/Utils/PrintUtils.h"
#include "Framework/Utils/AppInit.h"
#include "Framework/Utils/RunOpt.h"

#ifdef __GENIE_FLUX_DRIVERS_ENABLED__
#include "Tools/Flux/GFLUKAAtmoFlux.h"
#include "Tools/Flux/GBGLRSAtmoFlux.h"
#include "Tools/Flux/GHAKKMAtmoFlux.h"
#include "Tools/Flux/GCylindTH1Flux.h"
#include "Tools/Flux/GFluxDriverFactory.h"
#include "Tools/Flux/GMonoEnergeticFlux.h"
#endif

#ifdef __GENIE_GEOM_DRIVERS_ENABLED__
#include "Tools/Geometry/GeoUtils.h"
#include "Tools/Geometry/ROOTGeomAnalyzer.h"
#include "Tools/Geometry/PointGeomAnalyzer.h"
#endif

// ===========================================================================
// END GENIE PART.
// ===========================================================================

// ===========================================================================
// BELOW IS FOR CURRENT PACKAGE
// ===========================================================================

#include "ghep2hepmc.h"
#include "def.h"
#include "hepmcInfo.h"
#include "userdata.h"

// ===========================================================================
// END CURRENT PACKAGE PART.
// ===========================================================================

class GtGenieAtmoEvGenTool : public ToolBase, 
                             public IGenTool {
public:
    GtGenieAtmoEvGenTool(const std::string& name);
    ~GtGenieAtmoEvGenTool();

    bool configure();
    bool mutate(HepMC::GenEvent& event);

private:
    void                  Initialize(void);
    genie::GFluxI*        GetFlux(void);
    genie::GeomAnalyzerI* GetGeometry(void);
    void                  GetCommandLineArgs(void);
    void                  ParseFluxHst(string flux);

private:
    // User-specified options:
    //
    Long_t           gOptRunNu;                // run number
    string           gOptFluxInput;            // flux input
    string           gOptFluxSim;              // flux simulation (FLUKA, BGLRS or HAKKM)
    map<int, string> gOptFluxFiles;            // neutrino pdg code -> flux file map
    bool             gOptUsingRootGeom = false;// using root geom or target mix?
    bool             gOptUsingHistFlux = false;// using beam flux ntuples or flux from histograms?
    string           gOptFluxDriver = "";      // flux driver class to use
    map<int, TH1D*>  gOptFluxHst;              // flux histos (nu pdg  -> spectrum)  / if not using beam sim ntuples
    string           gOptFluxFile;             // ROOT file with beam flux ntuple

    string           gOptTarget;               // target input
    map<int, double> gOptTgtMix;               // target mix  (tgt pdg -> wght frac) / if not using detailed root geom
    string           gOptRootGeom;             // input ROOT file with realistic detector geometry
    string           gOptRootGeomTopVol = "";  // input geometry top event generation volume
    double           gOptGeomLUnits = 0;       // input geometry length units
    double           gOptGeomDUnits = 0;       // input geometry density units
    string           gOptExtMaxPlXml;          // max path lengths XML file for input geometry
    int              gOptNev = -1;             // exposure - in terms of number of events
    double           gOptKtonYrExposure = -1;  // exposure - in terms of kton*yrs

    double           gOptEvMin;                // minimum neutrino energy
    double           gOptEvMax;                // maximum neutrino energy
    string           gOptEvFilePrefix;         // event file prefix
    long int         gOptRanSeed;              // random number seed
    string           gOptInpXSecFile;          // cross-section splines

    string           gOptTune;

    //string           m_data_dir;               //--- for default flux input and xsec files
    int              m_target;                 //--- 0: JUNO LS target; 1: JUNO water target; 2: JUNO water target
    bool             m_diriso_enable;
    string           m_flux_frac = "";

    vector<double>   gOptFluxFrac;

    string           m_event_generator_list;

    // Defaults:
    //
    string kDefOptGeomLUnits = "mm";     // def geom length units (override with -L)
    string kDefOptGeomDUnits = "g_cm3";  // def geom density units (override with -D)
    double kDefOptEvMin      = 0.5;           // min neutrino energy (override with -E)
    double kDefOptEvMax      = 50.0;          // max neutrino energy (override with -E)

private:
    // create the GENIE monte carlo job driver
    genie::GMCJDriver* mcj_driver;
    int ievent = 0;

    private:
    //------for JUNO target
    TString targetname[3]        = {"LS", "Water", "Rock"};
    TString targetcomposition[3] = {"1000060120[0.8698321],1000060130[0.009408],1000010010[0.120086],1000010020[0.0000138],1000080160[0.00034],1000070140[0.00027],1000160320[0.00005]", "1000080160[0.8879],1000010010[0.1121]", "1000080160[0.4850],1000140280[0.3430],1000130270[0.08],1000260560[0.02],1000120240[0.01],1000200400[0.002],1000110230[0.024],1000190390[0.045]"};

private:
    Data    userdata;
    MassMap MM;
    TTree*  output_user;

private:
    void user_data_initialize();
    void save_user_tree();
};

#endif

