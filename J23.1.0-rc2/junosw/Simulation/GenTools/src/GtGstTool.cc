#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "RootWriter/RootWriter.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "TDatabasePDG.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "TTree.h"
#include "TFile.h"
#include "TDirectory.h"

#include "GtGstTool.h"

DECLARE_TOOL(GtGstTool);

GtGstTool::GtGstTool(const std::string& name)
    : ToolBase(name)
{
    declProp("inputGstFile", m_gst_filename);
    // initialize variables
    gst = 0;
    evt = 0;
    gst_entry = 0;
    declProp("GstStartIndex", gst_entry);

    declProp("GstVertexXYZEnabled", m_gst_vertex_xyz_enabled=false);
    declProp("GstVertexTEnabled", m_gst_vertex_t_enabled=false);

    declProp("ScaleXYZ", m_scale_xyz=10.);
    declProp("ScaleT", m_scale_t=1.);

    output_gst = 0;
}

GtGstTool::~GtGstTool()
{

}

bool
GtGstTool::configure()
{
    if (m_gst_filename.size()==0) {
        return false;
    }
    const char* preDir = gDirectory->GetPath();
    gst = TFile::Open(m_gst_filename.c_str());
    gDirectory->cd(preDir);
    if (gst == 0) {
        return false;
    }
    evt = (TTree*) (gst->Get("gst"));
    if (evt == 0) {
        return false;
    }
    return initTree();
}

bool
GtGstTool::mutate(HepMC::GenEvent& event) 
{
    if (not (gst_entry < evt->GetEntries())) {
        LogWarn << "!!! Can't Load Any Data from the gst file."
                << "!!! It will start from 0."
                << std::endl;
        gst_entry = 0;
    }
    evt->GetEntry(gst_entry);

    // create event
    // get the vertex 
    HepMC::GenVertex* vertex = event.signal_process_vertex();
    if (not vertex) {

        // if disabled, don't use the vertex info from gst
        if (not m_gst_vertex_xyz_enabled) {
            brVtxX = 0;
            brVtxY = 0;
            brVtxZ = 0;
        }
        if (not m_gst_vertex_t_enabled) {
            brVtxT = 0;
        }

        vertex = new HepMC::GenVertex(HepMC::FourVector(brVtxX*m_scale_xyz*CLHEP::meter,
                                                        brVtxY*m_scale_xyz*CLHEP::meter,
                                                        brVtxZ*m_scale_xyz*CLHEP::meter,
                                                        brVtxT*m_scale_t*CLHEP::second));
        event.set_signal_process_vertex(vertex);
    }

    HepMC::GenParticle* particle = 0;
    // == lepton
    particle = new HepMC::GenParticle( 
                        HepMC::FourVector(brPxl*CLHEP::GeV, 
                                          brPyl*CLHEP::GeV, 
                                          brPzl*CLHEP::GeV, 
                                          brEl*CLHEP::GeV),
                        brFSPrimLept,
                        1 /* status */
                    ); 
    vertex->add_particle_out(particle);
    // == hadronic
    for (int i = 0; i < brNf; ++i) {
        particle = new HepMC::GenParticle( 
                            HepMC::FourVector(brPxf[i] *CLHEP::GeV, 
                                              brPyf[i] *CLHEP::GeV, 
                                              brPzf[i] *CLHEP::GeV, 
                                              brEf [i] *CLHEP::GeV),
                            brPdgf[i],
                            1 /* status */
                        ); 
        vertex->add_particle_out(particle);
    }

    save_the_gst_partial();

    ++gst_entry;
    return true;
}

bool
GtGstTool::initTree()
{
    if (gst==0 or evt==0) {
        return false;
    }

    evt->SetBranchAddress("iev", &brIev);
    evt->SetBranchAddress("neu", &brNeutrino);
    evt->SetBranchAddress("tgt", &brTarget);
    evt->SetBranchAddress("qel", &brQel);
    evt->SetBranchAddress("res", &brRes);
    evt->SetBranchAddress("dis", &brDis);
    evt->SetBranchAddress("coh", &brCoh);
    evt->SetBranchAddress("dfr", &brDfr);
    evt->SetBranchAddress("imd", &brImd);
    evt->SetBranchAddress("nuel", &brNuel);
    evt->SetBranchAddress("cc", &brCC);
    evt->SetBranchAddress("nc", &brNC);
    // neutrino
    evt->SetBranchAddress("Ev", &brEv);
    evt->SetBranchAddress("pxv", &brPxv);
    evt->SetBranchAddress("pyv", &brPyv);
    evt->SetBranchAddress("pzv", &brPzv);

    // lepton
    evt->SetBranchAddress("fspl", &brFSPrimLept);
    evt->SetBranchAddress("El", &brEl);
    evt->SetBranchAddress("pxl", &brPxl);
    evt->SetBranchAddress("pyl", &brPyl);
    evt->SetBranchAddress("pzl", &brPzl);
    evt->SetBranchAddress("pl", &brPl); 
    evt->SetBranchAddress("cthl", &brCosthl);
    // hadronic                         
    evt->SetBranchAddress("nf", &brNf); 
    evt->SetBranchAddress("pdgf", brPdgf);
    evt->SetBranchAddress("Ef", brEf); 
    evt->SetBranchAddress("pxf", brPxf);
    evt->SetBranchAddress("pyf", brPyf);
    evt->SetBranchAddress("pzf", brPzf);
    evt->SetBranchAddress("pf", brPf); 
    evt->SetBranchAddress("cthf", brCosthf);

    // vertex
    evt->SetBranchAddress("vtxx", &brVtxX);
    evt->SetBranchAddress("vtxy", &brVtxY);
    evt->SetBranchAddress("vtxz", &brVtxZ);
    evt->SetBranchAddress("vtxt", &brVtxT);
                                        
    return true;                        
}

void
GtGstTool::save_the_gst_partial() {
    if (output_gst==0) {
        SniperPtr<RootWriter> svc(*getParent(), "RootWriter");
        if (svc.invalid()) {
            LogWarn << "Can't Find RootWriter" << std::endl;
            return;
        }
        output_gst = svc->bookTree(*m_par, "SIMEVT/pgst", "Partial GST File");
        if (not output_gst) {
            LogWarn << "Can't book Tree pgst" << std::endl;
            return;
        }
        output_gst->Branch("iev",           &brIev,           "iev/I"         );          
        output_gst->Branch("neu",           &brNeutrino,      "neu/I"         ); 
        output_gst->Branch("tgt",           &brTarget,        "tgt/I"         );
        output_gst->Branch("qel",           &brQel,           "qel/O"         );
        output_gst->Branch("res",           &brRes,           "res/O"         );
        output_gst->Branch("dis",           &brDis,           "dis/O"         );
        output_gst->Branch("coh",           &brCoh,           "coh/O"         );
        output_gst->Branch("dfr",           &brDfr,           "dfr/O"         );
        output_gst->Branch("imd",           &brImd,           "imd/O"         );
        output_gst->Branch("nuel",          &brNuel,          "nuel/O"        );
        output_gst->Branch("cc",            &brCC,            "cc/O"          );
        output_gst->Branch("nc",            &brNC,            "nc/O"          );
        output_gst->Branch("Ev",            &brEv,            "Ev/D"          );          
        output_gst->Branch("pxv",           &brPxv,           "pxv/D"         );          
        output_gst->Branch("pyv",           &brPyv,           "pyv/D"         );          
        output_gst->Branch("pzv",           &brPzv,           "pzv/D"         );  
        // lepton
        output_gst->Branch("fspl", &brFSPrimLept, "fspl/I");
        output_gst->Branch("El", &brEl, "El/D");
        output_gst->Branch("pxl", &brPxl, "pxl/D");
        output_gst->Branch("pyl", &brPyl, "pyl/D");
        output_gst->Branch("pzl", &brPzl, "pzl/D");
        output_gst->Branch("pl", &brPl, "pl/D");
        output_gst->Branch("cthl", &brCosthl, "cthl/D");
        output_gst->Branch("nf", &brNf, "nf/I");
        output_gst->Branch("pdgf", brPdgf, "pdgf[nf]/I");
        output_gst->Branch("Ef", brEf, "Ef[nf]/D");
        output_gst->Branch("pxf", brPxf, "pxf[nf]/D");
        output_gst->Branch("pyf", brPyf, "pyf[nf]/D");
        output_gst->Branch("pzf", brPzf, "pzf[nf]/D");
        output_gst->Branch("pf", brPf, "pf[nf]/D");
        output_gst->Branch("cthf", brCosthf, "cthf[nf]/D");

        // vertex
        output_gst->Branch("vtxx", &brVtxX, "vtxx/D");
        output_gst->Branch("vtxy", &brVtxY, "vtxy/D");
        output_gst->Branch("vtxz", &brVtxZ, "vtxz/D");
        output_gst->Branch("vtxt", &brVtxT, "vtxt/D");
    }

    output_gst->Fill();

    return;
}
