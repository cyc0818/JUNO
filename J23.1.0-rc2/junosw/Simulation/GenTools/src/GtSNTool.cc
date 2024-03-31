#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/Task.h"
#include "RootWriter/RootWriter.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "TDatabasePDG.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "TTree.h"
#include "TFile.h"
#include "TDirectory.h"

#include "GtSNTool.h"

DECLARE_TOOL(GtSNTool);

GtSNTool::GtSNTool(const std::string& name)
    : ToolBase(name)
{
    declProp("inputSNFile", m_gst_filename);
    // initialize variables
    gst = 0;
    evttree = 0;
    gst_entry = 0;
    declProp("StartIndex", gst_entry);

    output_gst = 0;
}

GtSNTool::~GtSNTool()
{

}

bool
GtSNTool::configure()
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
    evttree = (TTree*) (gst->Get("SNEvents"));
    if (evttree == 0) {
        return false;
    }
    return initTree();
}

bool
GtSNTool::mutate(HepMC::GenEvent& event) 
{
    if (gst_entry < 0) {
        LogError << "Unknown entry :" << gst_entry << std::endl;
        return false;
    }
    if (not (gst_entry < evttree->GetEntries())) {
        LogWarn << "!!! Can't Load Any Data from the file. Stop the run."
                << std::endl;
        gst_entry = 0;
        // return Incident::fire(*getParent(), "StopRun");
        return dynamic_cast<Task*>(getParent())->stop();
    }
    evttree->GetEntry(gst_entry);

    // create event
    // due to the time of final particles are different, 
    // so they must be different vertex.
    for (size_t i = 0; i < evt.nparticles; ++i){
        HepMC::GenVertex* vertex = 0;
        // Only the time is set here
        vertex = new HepMC::GenVertex(HepMC::FourVector(0,0,0,evt.t[i]*CLHEP::nanosecond));
        event.set_signal_process_vertex(vertex);

        HepMC::GenParticle* particle = 0;
        particle = new HepMC::GenParticle( 
                            HepMC::FourVector(evt.px[i] *CLHEP::MeV, 
                                              evt.py[i] *CLHEP::MeV, 
                                              evt.pz[i] *CLHEP::MeV, 
                                              sqrt(evt.px[i]*evt.px[i]
                                                 + evt.py[i]*evt.py[i] 
                                                 + evt.pz[i]*evt.pz[i]
                                                 + evt.m[i] * evt.m[i])*CLHEP::MeV),
                            evt.pdgid[i],
                            1 /* status */
                        ); 
        vertex->add_particle_out(particle);
    }

    ++gst_entry;
    return true;
}

bool
GtSNTool::initTree()
{
    if (gst==0 or evttree==0) {
        return false;
    }
    evttree->SetBranchAddress("evt", &evt);

    return true;
}
