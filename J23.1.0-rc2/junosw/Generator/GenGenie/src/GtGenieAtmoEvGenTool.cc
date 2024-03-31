#include "GtGenieAtmoEvGenTool.h"

using std::string;
using std::vector;
using std::map;
using std::ostringstream;

using namespace genie;
using namespace genie::flux;
// using namespace genie::controls;

// FRAMEWORK
DECLARE_TOOL(GtGenieAtmoEvGenTool);
// END

GtGenieAtmoEvGenTool::GtGenieAtmoEvGenTool(const std::string &name) : ToolBase(name)
{
  // declProp("run", gOptRunNu = 100000000);
  // declProp("nevts", gOptNev = 1000);
  declProp("EvMin",        gOptEvMin = 0.1);
  declProp("EvMax",        gOptEvMax = 20.);
  declProp("Flux",         gOptFluxInput);
  declProp("JUNOTarget",   m_target = 0);
  declProp("TgtMix",       gOptTarget);
  declProp("RanSeed",      gOptRanSeed = -1);
  declProp("InpXSecFile",  gOptInpXSecFile);
  declProp("Tune",         gOptTune);
  declProp("IsotroEnable", m_diriso_enable);
  declProp("FluxFrac",     m_flux_frac);
  declProp("EventGeneratorList", m_event_generator_list);

  output_user = 0;
}

GtGenieAtmoEvGenTool::~GtGenieAtmoEvGenTool()
{
}

bool GtGenieAtmoEvGenTool::configure()
{
  LogInfo << "configure GENIE..." << std::endl;

  Initialize();
  // get flux driver
  GFluxI *flux_driver = GetFlux();

  // get geometry driver
  GeomAnalyzerI *geom_driver = GetGeometry();
  // Set GHEP print level
  GHepRecord::SetPrintLevel(RunOpt::Instance()->EventRecordPrintLevel());
  RunOpt::Instance()->SetEventGeneratorList(m_event_generator_list);

  mcj_driver = new GMCJDriver;
  mcj_driver->SetEventGeneratorList(RunOpt::Instance()->EventGeneratorList());
  mcj_driver->UseFluxDriver(flux_driver);
  mcj_driver->UseGeomAnalyzer(geom_driver);
  mcj_driver->Configure();
  mcj_driver->UseSplines();
  mcj_driver->ForceSingleProbScale();

  return true;
}

bool GtGenieAtmoEvGenTool::mutate(HepMC::GenEvent &hepmc_event)
{
  // The loop of original GenerateEventsAtFixedInitState is migrated to mutate.
  // The event loop is now controlled by sniper.
  EventRecord *event = mcj_driver->GenerateEvent();
  LOG("gevgen_atmo", pNOTICE) << "Generated event: " << *event;

  // =======================================================================
  // CONVERT from EventRecord to HepMC
  // =======================================================================

  user_data_initialize();
  ghep2hepmc(*event, hepmc_event);
  
  //isotropic_direction(hepmc_event);
  
  if(gOptUsingHistFlux && m_diriso_enable) {
    isotropic_direction(hepmc_event);
  }
  
  ghep2userdata(*event, userdata);
  hepmc2userdata(hepmc_event, userdata);
  if(userdata.isoPdg>0 && userdata.isoPdg!= 1000060120) {
    userdata.isoMass = MM.pdg2mass[userdata.isoPdg];
  }

  //-------------next will for deexcitation 
  //-------------------------------

  userdata.Iev = ievent;
  save_user_tree();

  // =======================================================================
  // Done
  // =======================================================================
  ievent++;
  delete event;
  LogInfo << "Done to Generate GENIE Event..." << std::endl;
  return true;
}
void GtGenieAtmoEvGenTool::GetCommandLineArgs(void)
{
  //
  // *** flux
  //
  TString datadir;
  TString nuname[4] = {"[12]", "[14]", "[-12]", "[-14]"};
  TString flux;
  if (gOptFluxInput.size() == 0) {
    datadir="/cvmfs/juno.ihep.ac.cn/sw/GENIE-MC/input-data/groups/atmo-ihep/";
    //if (m_data_dir.size() == 0) {
      //std::cout << "Error: please input data dir" << std::endl;
      //exit(1);
    //}
    for (int jj = 0; jj < 4; jj++) {
      if (jj == 0) {
        flux = "HAKKM:";
        flux += datadir;
        flux += "/juno-ally-20-12-solmax.txt";
        flux += nuname[jj];
      } else {
        flux += ",";
        flux += datadir;
        flux += "/juno-ally-20-12-solmax.txt";
        flux += nuname[jj];
      }
    }
    gOptFluxInput = string(flux.Data());
  }

  string::size_type jsimend = gOptFluxInput.find_first_of(":", 0);
  if (jsimend != string::npos) {

    // get flux simulation info (FLUKA,BGLRS) so as to instantiate the
    // appropriate flux driver

    gOptFluxSim = gOptFluxInput.substr(0, jsimend);
    for (string::size_type i = 0; i < gOptFluxSim.size(); i++) {
      gOptFluxSim[i] = toupper(gOptFluxSim[i]);
    }
    if ((gOptFluxSim != "FLUKA") && (gOptFluxSim != "BGLRS") && (gOptFluxSim != "HAKKM")) {
      LOG("gevgen_atmo", pFATAL) << "The flux file source needs to be one of <FLUKA,BGLRS,HAKKM>";
      // PrintSyntax();
      // gAbortingInErr = true;
      exit(1);
    }

    // now get the list of input files and the corresponding neutrino codes.
    gOptFluxInput.erase(0, jsimend + 1);
    vector<string> fluxv = utils::str::Split(gOptFluxInput, ",");
    vector<string>::const_iterator fluxiter = fluxv.begin();
    for (; fluxiter != fluxv.end(); ++fluxiter) {
      string filename_and_pdg = *fluxiter;
      string::size_type open_bracket = filename_and_pdg.find("[");
      string::size_type close_bracket = filename_and_pdg.find("]");
      if (open_bracket == string::npos || close_bracket == string::npos) {
        LOG("gevgen_atmo", pFATAL) << "You made an error in specifying the flux info";
        exit(1);
      }
      string::size_type ibeg = 0;
      string::size_type iend = open_bracket;
      string::size_type jbeg = open_bracket + 1;
      string::size_type jend = close_bracket;
      string flux_filename = filename_and_pdg.substr(ibeg, iend - ibeg);
      string neutrino_pdg = filename_and_pdg.substr(jbeg, jend - jbeg);
      gOptFluxFiles.insert(map<int, string>::value_type(atoi(neutrino_pdg.c_str()), flux_filename));
    }

    if (gOptFluxFiles.size() == 0) {
      LOG("gevgen_atmo", pFATAL) << "You must specify at least one flux file!";
      exit(1);
    }

  } else {
    // does it look like the histogram format
    const char *hstrings[] = {",12[", ",+12[", ",-12[", ",14[", ",+14[", ",-14[", ",16[", ",+16[", ",-16["};
    size_t nh = sizeof(hstrings) / sizeof(const char *);
    for (size_t ih = 0; ih < nh; ++ih) {
      if (gOptFluxInput.find(hstrings[ih]) != std::string::npos) {
        // hey!
        gOptFluxDriver = "genie::flux::GCylindTH1Flux";
        break;
      }
    }  // loop over possible histogram specifiers
    if (gOptFluxDriver == "") {
      LOG("gevgen_atmo", pFATAL) << "You need to specify the flux file source";
      exit(1);
    }
  }
  gOptUsingHistFlux = (gOptFluxDriver == "genie::flux::GCylindTH1Flux");
  if (gOptUsingHistFlux) ParseFluxHst(gOptFluxInput);

  //
  // *** target
  //
  string geom = "";
  if (gOptTarget.size() == 0) {
    geom = string(targetcomposition[m_target].Data());
  } else {
    geom = gOptTarget;
  }
  // User has specified a target mix.
  // Decode the list of target pdf codes & their corresponding weight fraction
  // (specified as 'pdg_code_1[fraction_1],pdg_code_2[fraction_2],...')
  // See documentation on top section of this file.
  //
  gOptTgtMix.clear();
  vector<string> tgtmix = utils::str::Split(geom, ",");
  if (tgtmix.size() == 1) {
    int pdg = atoi(tgtmix[0].c_str());
    double wgt = 1.0;
    gOptTgtMix.insert(map<int, double>::value_type(pdg, wgt));
  } else {
    vector<string>::const_iterator tgtmix_iter = tgtmix.begin();
    for (; tgtmix_iter != tgtmix.end(); ++tgtmix_iter) {
      string tgt_with_wgt = *tgtmix_iter;
      string::size_type open_bracket = tgt_with_wgt.find("[");
      string::size_type close_bracket = tgt_with_wgt.find("]");
      if (open_bracket == string::npos || close_bracket == string::npos) {
        LOG("gevgen_atmo", pFATAL) << "You made an error in specifying the target mix";
        exit(1);
      }
      string::size_type ibeg = 0;
      string::size_type iend = open_bracket;
      string::size_type jbeg = open_bracket + 1;
      string::size_type jend = close_bracket;
      int pdg = atoi(tgt_with_wgt.substr(ibeg, iend - ibeg).c_str());
      double wgt = atof(tgt_with_wgt.substr(jbeg, jend - jbeg).c_str());
      LOG("gevgen_atmo", pDEBUG) << "Adding to target mix: pdg = " << pdg << ", wgt = " << wgt;
      gOptTgtMix.insert(map<int, double>::value_type(pdg, wgt));

    }  // tgtmix_iter
  }    // >1 materials in mix

  //
  // *** cross-sections
  //
  if (gOptInpXSecFile.size() == 0) {
    TString xsec = datadir;
    xsec += "/xsec/v3_02_00/G18_10b_02_11b.xml";
    gOptInpXSecFile = string(xsec.Data());
  }

  LOG("gevgen_atmo", pINFO) << "Reading cross-section file" << "\t" << gOptInpXSecFile;

  if (gOptTune.size() == 0) {
    gOptTune = "G18_10b_02_11b";
  }
}

//________________________________________________________________________________________
GFluxI *GtGenieAtmoEvGenTool::GetFlux(void)
{
  GFluxI *flux_driver = 0;

#ifdef __GENIE_FLUX_DRIVERS_ENABLED__

  if (!gOptUsingHistFlux) {
    // Instantiate appropriate concrete flux driver
    GAtmoFlux *atmo_flux_driver = 0;
    if (gOptFluxSim == "FLUKA") {
      GFLUKAAtmoFlux *fluka_flux = new GFLUKAAtmoFlux;
      atmo_flux_driver = dynamic_cast<GAtmoFlux *>(fluka_flux);
    } else if (gOptFluxSim == "BGLRS") {
      GBGLRSAtmoFlux *bartol_flux = new GBGLRSAtmoFlux;
      atmo_flux_driver = dynamic_cast<GAtmoFlux *>(bartol_flux);
    } else if (gOptFluxSim == "HAKKM") {
      GHAKKMAtmoFlux *honda_flux = new GHAKKMAtmoFlux;
      atmo_flux_driver = dynamic_cast<GAtmoFlux *>(honda_flux);
    } else {
      LOG("gevgen_atmo", pFATAL) << "Uknonwn flux simulation: " << gOptFluxSim;
      // gAbortingInErr = true;
      exit(1);
    }
    // Configure GAtmoFlux options (common to all concrete atmospheric flux drivers)
    // set min/max energy:
    atmo_flux_driver->ForceMinEnergy(gOptEvMin * units::GeV);
    atmo_flux_driver->ForceMaxEnergy(gOptEvMax * units::GeV);
    // set flux files:
    map<int, string>::const_iterator file_iter = gOptFluxFiles.begin();
    for (; file_iter != gOptFluxFiles.end(); ++file_iter) {
      int neutrino_code = file_iter->first;
      string filename = file_iter->second;
      atmo_flux_driver->AddFluxFile(neutrino_code, filename);
    }
    if (!atmo_flux_driver->LoadFluxData()) {
      LOG("gevgen_atmo", pFATAL) << "Error loading flux data. Quitting...";
      //gAbortingInErr = true;
      exit(1);
    }
    
    //atmo_flux_driver->LoadFluxData();
    // configure flux generation surface:
    atmo_flux_driver->SetRadii(1, 1);
    // Cast to GFluxI, the generic flux driver interface
    flux_driver = dynamic_cast<GFluxI *>(atmo_flux_driver);
  } else {
    //
    // *** Using fluxes from histograms (for all specified neutrino species)
    //
    flux_driver = genie::flux::GFluxDriverFactory::Instance().GetFluxDriver(gOptFluxDriver);
    genie::flux::GCylindTH1Flux *hist_flux_driver = dynamic_cast<genie::flux::GCylindTH1Flux *>(flux_driver);
    if (!hist_flux_driver) {
      LOG("gevgen_fnal", pFATAL) << "Failed to get GCylinderTH1Flux driver from GFluxDriverFactory\n"
                                 << "when using " << gOptFluxDriver;
      exit(1);
    }

    // creating & configuring a generic GCylindTH1Flux flux driver
    TVector3 bdir(0, 0, 1);  // dir along +z
    TVector3 bspot(0, 0, 0);

    hist_flux_driver->SetNuDirection(bdir);
    hist_flux_driver->SetBeamSpot(bspot);
    hist_flux_driver->SetTransverseRadius(-1);
    map<int, TH1D *>::iterator it = gOptFluxHst.begin();
    for (; it != gOptFluxHst.end(); ++it) {
      int pdg_code = it->first;
      TH1D *spectrum = it->second;
      hist_flux_driver->AddEnergySpectrum(pdg_code, spectrum);
      // once the histogram has been added to the GCylindTH1Flux driver
      // it is owned by the driver and it is up to the the driver
      // to clean up (i.e. delete it).
      // remove it from this map to avoid double deletion.
      it->second = 0;
    }
  }

#else
  LOG("gevgen_atmo", pFATAL) << "You need to enable the GENIE flux drivers first!";
  LOG("gevgen_atmo", pFATAL) << "Use --enable-flux-drivers at the configuration step.";
  // gAbortingInErr = true;
  exit(1);
#endif

  return flux_driver;
}

//------------------------------------------
GeomAnalyzerI *GtGenieAtmoEvGenTool::GetGeometry(void)
{
  GeomAnalyzerI *geom_driver = 0;
#ifdef __GENIE_GEOM_DRIVERS_ENABLED__
  //
  // *** Using a 'point' geometry with the specified target mix
  // *** ( = a list of targets with their corresponding weight fraction)
  //
  // creating & configuring a point geometry driver
  geometry::PointGeomAnalyzer *pgeom = new geometry::PointGeomAnalyzer(gOptTgtMix);
  // casting to the GENIE geometry driver interface
  geom_driver = dynamic_cast<GeomAnalyzerI *>(pgeom);
#else
  LOG("gevgen_atmo", pFATAL) << "You need to enable the GENIE geometry drivers first!";
  LOG("gevgen_atmo", pFATAL) << "Use --enable-geom-drivers at the configuration step.";
  exit(1);
#endif
  return geom_driver;
}

void GtGenieAtmoEvGenTool::Initialize()
{
  GetCommandLineArgs();
  RunOpt::Instance()->SetTuneName(gOptTune);
  if (!RunOpt::Instance()->Tune()) {
    LOG("gmkspl", pFATAL) << " No TuneId in RunOption";
    exit(-1);
  }

  RunOpt::Instance()->BuildTune();
  // Iinitialization of random number generators, cross-section table, messenger, cache etc...
  utils::app_init::MesgThresholds(RunOpt::Instance()->MesgThresholdFiles());
  utils::app_init::CacheFile(RunOpt::Instance()->CacheFile());
  utils::app_init::RandGen(gOptRanSeed);
  //std::cout << "!!!!!!Jie test !!! gOptInpXSecFile  " << gOptInpXSecFile << std::endl; 
  utils::app_init::XSecTable(gOptInpXSecFile, true);
}

void GtGenieAtmoEvGenTool::ParseFluxHst(string flux)
{
  // Using flux from histograms
  // Extract the root file name & the list of histogram names & neutrino
  // species (specified as 'filename,histo1[species1],histo2[species2],...')
  // See documentation on top section of this file.
  vector<string> fluxv = utils::str::Split(flux, ",");
  if (fluxv.size() < 2) {
    LOG("gevgen_atmo", pFATAL) << "You need to specify both a flux ntuple ROOT file "
                               << " _AND_ a flux hist";
    // PrintSyntax();
    exit(1);
  }

  gOptFluxFile = fluxv[0];
  bool accessible_flux_file = !(gSystem->AccessPathName(gOptFluxFile.c_str()));
  if (!accessible_flux_file) {
    LOG("gevgen_atmo", pFATAL) << "Can not access flux file: " << gOptFluxFile;
    // PrintSyntax();
    exit(1);
  }

  if (m_flux_frac.size() > 0) {
    string pattern = ",";
    string fluxfrac = m_flux_frac;
    fluxfrac += pattern;
    size_t pos = fluxfrac.find(pattern);
    LogInfo << "Please check the fractions of each neutrino flavor: ";
    while (pos != fluxfrac.npos) {
      string temp = fluxfrac.substr(0, pos);
      char *tempname = new char[temp.size() + 1];
      strcpy(tempname, temp.c_str());
      gOptFluxFrac.push_back(atof(tempname));
      std::cout << atof(tempname) << " ";
      fluxfrac = fluxfrac.substr(pos + 1, fluxfrac.size());
      pos = fluxfrac.find(pattern);
    }
    std::cout << std::endl;
    if (gOptFluxFrac.size() != fluxv.size() - 1) {
      LOG("gevgen_atmo", pFATAL) << "Please input right flux fractions";
      // PrintSyntax();
      exit(1);
    }
  }

  // Extract energy spectra for all specified neutrino species
  TFile flux_file(gOptFluxFile.c_str(), "read");
  for (unsigned int inu = 1; inu < fluxv.size(); inu++) {
    string nutype_and_histo = fluxv[inu];
    string::size_type open_bracket = nutype_and_histo.find("[");
    string::size_type close_bracket = nutype_and_histo.find("]");
    if (open_bracket == string::npos || close_bracket == string::npos) {
      LOG("gevgen_atmo", pFATAL) << "You made an error in specifying the flux histograms";
      // PrintSyntax();
      exit(1);
    }
    string::size_type ibeg = 0;
    string::size_type iend = open_bracket;
    string::size_type jbeg = open_bracket + 1;
    string::size_type jend = close_bracket;
    string nutype = nutype_and_histo.substr(ibeg, iend - ibeg);
    string histo = nutype_and_histo.substr(jbeg, jend - jbeg);
    // access specified histogram from the input root file
    TH1D *ihst = (TH1D *)flux_file.Get(histo.c_str());
    if (!ihst) {
      LOG("gevgen_atmo", pFATAL) << "Can not find histogram: " << histo << " in flux file: " << gOptFluxFile;
      // PrintSyntax();
      exit(1);
    }
    // create a local copy of the input histogram
    TString histname = histo.c_str();
    histname += "_new";
    // TH1D *spectrum = new TH1D(histo.c_str(), histo.c_str(), ihst->GetNbinsX(), ihst->GetXaxis()->GetXmin(), ihst->GetXaxis()->GetXmax());
    TH1D *spectrum = new TH1D(histname, histname, ihst->GetNbinsX(), ihst->GetXaxis()->GetXmin(), ihst->GetXaxis()->GetXmax());
    spectrum->SetDirectory(0);
    int bin_start = ihst->FindBin(gOptEvMin);
    int bin_end = ihst->FindBin(gOptEvMax);
    for (int ibin = bin_start; ibin <= bin_end; ibin++) {
      spectrum->SetBinContent(ibin, ihst->GetBinContent(ibin));
    }

    if(m_flux_frac.size() > 0) {
      spectrum->Scale(gOptFluxFrac[inu-1]/spectrum->Integral());
    }
    // convert neutrino name -> pdg code
    int pdg = atoi(nutype.c_str());
    if (!pdg::IsNeutrino(pdg) && !pdg::IsAntiNeutrino(pdg)) {
      LOG("gevgen_atmo", pFATAL) << "Unknown neutrino type: " << nutype;
      // PrintSyntax();
      exit(1);
    }
    // store flux neutrino code / energy spectrum
    LOG("gevgen_atmo", pDEBUG) << "Adding energy spectrum for flux neutrino: pdg = " << pdg;
    gOptFluxHst.insert(map<int, TH1D *>::value_type(pdg, spectrum));
  }  // inu

  if (gOptFluxHst.size() < 1) {
    LOG("gevgen_atmo", pFATAL) << "You have not specified any flux histogram!";
    // PrintSyntax();
    exit(1);
  }

  flux_file.Close();
}

void GtGenieAtmoEvGenTool::save_user_tree()
{

  if (output_user == 0) {
    SniperPtr<RootWriter> svc(*getParent(), "RootWriter");
    if (svc.invalid()) {
      LogWarn << "Can't Find RootWriter" << std::endl;
      return;
    }
    output_user = svc->bookTree(*m_par, "SIMEVT/atmgen", "Atmospheric Info File");
    if (not output_user) {
      LogWarn << "Can't book Tree atmgen" << std::endl;
      return;
    }
    output_user->Branch("iev", &userdata.Iev, "iev/I");
    output_user->Branch("pPdg", &userdata.pPdg, "pPdg/I");
    output_user->Branch("tPdg", &userdata.tPdg, "tPdg/I");
    output_user->Branch("interType", &userdata.interType, "interType/I");
    output_user->Branch("ctag", &userdata.ctag, "ctag/I");
    output_user->Branch("pEn", &userdata.pEn, "pEn/D");
    output_user->Branch("pPx", &userdata.pPx, "pPx/D");
    output_user->Branch("pPy", &userdata.pPy, "pPy/D");
    output_user->Branch("pPz", &userdata.pPz, "pPz/D");
    output_user->Branch("isoPdg", &userdata.isoPdg, "isoPdg/I");
    output_user->Branch("isoPx", &userdata.isoPx, "isoPx/D");
    output_user->Branch("isoPy", &userdata.isoPy, "isoPy/D");
    output_user->Branch("isoPz", &userdata.isoPz, "isoPz/D");
    output_user->Branch("isoMass", &userdata.isoMass, "isoMass/D");
    output_user->Branch("Npars", &userdata.Npars, "Npars/I");
    output_user->Branch("pdg", userdata.pdg, "pdg[Npars]/I");
    output_user->Branch("deextag", userdata.deextag, "deextag[Npars]/I");
    output_user->Branch("px", userdata.px, "px[Npars]/D");
    output_user->Branch("py", userdata.py, "py[Npars]/D");
    output_user->Branch("pz", userdata.pz, "pz[Npars]/D");
    output_user->Branch("energy", userdata.energy, "energy[Npars]/D");
    output_user->Branch("mass", userdata.mass, "mass[Npars]/D");
    output_user->Branch("weight", &userdata.weight, "weight/D");
  }

  output_user->Fill();
  return;
}

void GtGenieAtmoEvGenTool::user_data_initialize()
{

  userdata.Iev = -1;
  userdata.pPdg = 0;
  userdata.tPdg = 0;
  userdata.interType = 0;  // 1---QEL; 2---RES; 3---COH; 4---DIS; 5---2p2h(MEC)
  userdata.ctag = 0;       // 1---NC; 2---CC

  userdata.pEn = 0.;
  userdata.pPx = 0.;
  userdata.pPy = 0.;
  userdata.pPz = 0.;

  userdata.isoPdg = 0;
  userdata.isoPx = 0.;
  userdata.isoPy = 0.;
  userdata.isoPz = 0.;
  userdata.isoMass = 0.;

  userdata.Npars = 0;
  for (int i = 0; i < 250; i++) {
    userdata.pdg[i] = 0;
    userdata.deextag[i] = 0;  // 0---from interaction, 1---from deex
    userdata.px[i] = 0.;
    userdata.py[i] = 0.;
    userdata.pz[i] = 0.;
    userdata.energy[i] = 0.;
    userdata.mass[i] = 0.;
  }
  userdata.weight = 0.;
}

