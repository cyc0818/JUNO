#ifndef GtGenieEvGenTool_h
#define GtGenieEvGenTool_h

/*
 * Description:
 *   This gentool is based on the example from GENIE:
 *     - Generator-R-3_00_06/src/Apps/gEvGen.cxx
 * 
 *   It gives an example to show how to integrate an external applications with SNiPER.
 *
 * Author: 
 *   Tao Lin <lintao AT ihep.ac.cn>
 */

// ===========================================================================
// BELOW IS FOR GenTool
// ===========================================================================
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

#include <memory>

// ===========================================================================
// END FRAMEWORK PART.
// ===========================================================================

// ===========================================================================
// BELOW IS FOR GENIE
// ===========================================================================
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TVector3.h>
#include <TH1.h>
#include <TF1.h>

#include "Framework/Conventions/XmlParserStatus.h"
#include "Framework/Conventions/GBuild.h"
#include "Framework/Conventions/Controls.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/EventGen/GFluxI.h"
#include "Framework/EventGen/GEVGDriver.h"
#include "Framework/EventGen/GMCJDriver.h"
#include "Framework/EventGen/GMCJMonitor.h"
#include "Framework/Interaction/Interaction.h"
#include "Framework/Messenger/Messenger.h"
#include "Framework/Ntuple/NtpWriter.h"
#include "Framework/Ntuple/NtpMCFormat.h"
#include "Framework/Numerical/RandomGen.h"
#include "Framework/Numerical/Spline.h"
#include "Framework/ParticleData/PDGCodes.h"
#include "Framework/ParticleData/PDGUtils.h"
#include "Framework/Utils/AppInit.h"
#include "Framework/Utils/RunOpt.h"
#include "Framework/Utils/XSecSplineList.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/PrintUtils.h"
#include "Framework/Utils/SystemUtils.h"
#include "Framework/Utils/CmdLnArgParser.h"


// ===========================================================================
// END GENIE PART.
// ===========================================================================


// ===========================================================================
// BELOW IS FOR CURRENT PACKAGE
// ===========================================================================

#include "ghep2hepmc.h"

// ===========================================================================
// END CURRENT PACKAGE PART.
// ===========================================================================


class GtGenieEvGenTool: public ToolBase,
                        public IGenTool {
public:
    GtGenieEvGenTool(const std::string& name);
    ~GtGenieEvGenTool();

    bool configure();
    bool mutate(HepMC::GenEvent& event);

private:

    void Initialize         (void);

private:
    //Default options (override them using the command line arguments):
    int           kDefOptNevents   = 0;       // n-events to generate
    genie::NtpMCFormat_t kDefOptNtpFormat = genie::kNFGHEP; // ntuple format
    Long_t        kDefOptRunNu     = 0;       // default run number

    //User-specified options:
    int             gOptNevents;      // n-events to generate
    double          gOptNuEnergy;     // neutrino E, or min neutrino energy in spectrum
    double          gOptNuEnergyRange;// energy range in input spectrum
    int             gOptNuPdgCode;    // neutrino PDG code
    map<int,double> gOptTgtMix;       // target mix (each with its relative weight)
    Long_t          gOptRunNu;        // run number
    string          gOptFlux;         //
    bool            gOptWeighted;     //
    bool            gOptUsingFluxOrTgtMix = false;
    long int        gOptRanSeed;      // random number seed
    string          gOptInpXSecFile;  // cross-section splines
    string          gOptOutFileName;  // Optional outfile name
    string          gOptStatFileName; // Status file name, set if gOptOutFileName was set.

    string          gOptTune;

private:
    // Breaking GenerateEventsAtFixedInitState into two parts: configure and mutate. 
    // These variables are used to share between them.
    // The c++ shared_ptr is used to manage the objects.
    std::shared_ptr<genie::GEVGDriver> evg_driver;
    int ievent = 0;

    TLorentzVector nu_p4;

};


#endif
