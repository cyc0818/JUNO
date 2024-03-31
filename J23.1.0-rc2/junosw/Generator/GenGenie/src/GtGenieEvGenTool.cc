#include "GtGenieEvGenTool.h"

using std::string;
using std::vector;
using std::map;
using std::ostringstream;

using namespace genie;
using namespace genie::controls;


// FRAMEWORK
DECLARE_TOOL(GtGenieEvGenTool);
// END

GtGenieEvGenTool::GtGenieEvGenTool(const std::string& name)
    : ToolBase(name) {
    declProp("NuEnergy",      gOptNuEnergy=42.);
    declProp("NuEnergyRange", gOptNuEnergyRange=1.); // set -1 for mono energy
    declProp("NuPdgCode",     gOptNuPdgCode=16);
    declProp("TgtMix",        gOptTgtMix);
    declProp("Flux",          gOptFlux);
    declProp("Weighted",      gOptWeighted);
    declProp("RanSeed",       gOptRanSeed=42);
    declProp("InpXSecFile",   gOptInpXSecFile);
    declProp("OutFileName",   gOptOutFileName);
    declProp("StatFileName",  gOptStatFileName);
    declProp("Tune",          gOptTune);
}

GtGenieEvGenTool::~GtGenieEvGenTool() {

}

bool
GtGenieEvGenTool::configure() {
    LogInfo << "configure GENIE..." << std::endl;

    Initialize();

    // The initialization part of original GenerateEventsAtFixedInitState is migrated to configure. 
    // 
    int neutrino = gOptNuPdgCode;
    int target   = gOptTgtMix.begin()->first;
    double Ev    = gOptNuEnergy;
    nu_p4 = TLorentzVector(0.,0.,Ev,Ev); // px,py,pz,E (GeV)

    // Create init state
    InitialState init_state(target, neutrino);

    // Create/config event generation driver
    evg_driver = std::make_shared<GEVGDriver>();
    evg_driver->SetEventGeneratorList(RunOpt::Instance()->EventGeneratorList());
    evg_driver->SetUnphysEventMask(*RunOpt::Instance()->UnphysEventMask());
    evg_driver->Configure(init_state);

    LOG("gevgen", pNOTICE)
        << "\n ** Will generate " << gOptNevents << " events for \n"
        << init_state << " at Ev = " << Ev << " GeV";

    // Generate events / print the GHEP record / add it to the ntuple

    return true;
}

bool
GtGenieEvGenTool::mutate(HepMC::GenEvent& hepmc_event) {
    // The loop of original GenerateEventsAtFixedInitState is migrated to mutate. 
    // The event loop is now controlled by sniper.

    LogInfo << "Generating GENIE Event..." << std::endl;

    LOG("gevgen", pNOTICE)
        << " *** Generating event............ " << ievent;

    // generate a single event
    EventRecord * event = evg_driver->GenerateEvent(nu_p4);

    if(!event) {
        LOG("gevgen", pNOTICE)
            << "Last attempt failed. Re-trying....";
        return false;
    }

    LOG("gevgen", pNOTICE)
        << "Generated Event GHEP Record: " << *event;

    // =======================================================================
    // CONVERT from EventRecord to HepMC
    // =======================================================================

    ghep2hepmc(*event, hepmc_event);

    // =======================================================================
    // Done 
    // =======================================================================
    

    ievent++;
    delete event;

    LogInfo << "Done to Generate GENIE Event..." << std::endl;

    return true;
}

void
GtGenieEvGenTool::Initialize() {
    RunOpt::Instance()->SetTuneName(gOptTune);
    if ( ! RunOpt::Instance()->Tune() ) {
        LOG("gevgen", pFATAL) << " No TuneId in RunOption";
        exit(-1);
    }
    RunOpt::Instance()->BuildTune();

    // Initialization of random number generators, cross-section table,
    // messenger thresholds, cache file
    utils::app_init::MesgThresholds(RunOpt::Instance()->MesgThresholdFiles());
    utils::app_init::CacheFile(RunOpt::Instance()->CacheFile());
    utils::app_init::RandGen(gOptRanSeed);
    utils::app_init::XSecTable(gOptInpXSecFile, false);

    // Set GHEP print level
    GHepRecord::SetPrintLevel(RunOpt::Instance()->EventRecordPrintLevel());

}
