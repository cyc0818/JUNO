#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "TTree.h"
#include "TFile.h"
#include "TDirectory.h"

#include "GtOPLoaderTool.h"

DECLARE_TOOL(GtOPLoaderTool);

GtOPLoaderTool::GtOPLoaderTool(const std::string& name)
    : ToolBase(name)
{
    declProp("inputFile", m_filename);
    declProp("ChunkSize", m_chunk_size=1024);
    declProp("ChunkIndex", m_chunk_index=0);
    m_file = 0;
    op_col = 0;
}

GtOPLoaderTool::~GtOPLoaderTool()
{

}

bool
GtOPLoaderTool::configure()
{
    // = check =
    // == check the file and tree ==
    if (m_filename.size() == 0) {
        LogError << "Please set the input filename." << std::endl;
        return false;
    }
    const char* previousDir = gDirectory->GetPath();
    m_file = TFile::Open(m_filename.c_str());
    gDirectory->cd(previousDir);
    if (m_file == 0) {
        LogError << "Can't open file " << m_filename << std::endl;
        return false;
    }
    op_col = (TTree*) (m_file->Get("opticalphoton"));
    if (op_col == 0) {
        LogError << "Can't file the op col" << std::endl;
        return false;
    }
    // == total entries in the file ==
    m_total_entries = op_col -> GetEntries();
    // == calculate the real index ==
    // === check index and size ===
    if (m_chunk_size<=0) {
        LogWarn << "The current chunk size is " << m_chunk_size << std::endl;
        m_chunk_size = 1024;
        LogWarn << "Reset it to " << m_chunk_size << std::endl;
    }
    if (m_chunk_index<0) {
        LogWarn << "The current chunk index is " << m_chunk_index << std::endl;
        m_chunk_index = 0;
        LogWarn << "Reset it to " << m_chunk_index << std::endl;
    }
    m_index = m_chunk_index*m_chunk_size;
    if (m_index >= m_total_entries) {
        LogError << "The ChunkIndex*ChunkSize is bigger than the total entries."
                 << std::endl;
        return false;
    }
    return initTree();
}

bool
GtOPLoaderTool::mutate(HepMC::GenEvent& event)
{
    // please note, the OPs are in different positions, so the vertex should be
    // different for every OP.
    if (m_index >= m_total_entries) {
        LogWarn << "Can't load data any more!" << std::endl;
        return false;
    }
    int index_limit = m_index + m_chunk_size;
    if (index_limit > m_total_entries) {
        index_limit = m_total_entries;
    }
    for (; m_index < index_limit; ++m_index) {
        add_optical_photon(event);
    }
    return true;
}

bool
GtOPLoaderTool::initTree()
{
    op_col->SetBranchAddress("evtID", &evtid);
    op_col->SetBranchAddress("t", &t);
    op_col->SetBranchAddress("x", &x);
    op_col->SetBranchAddress("y", &y);
    op_col->SetBranchAddress("z", &z);

    op_col->SetBranchAddress("px", &px);
    op_col->SetBranchAddress("py", &py);
    op_col->SetBranchAddress("pz", &pz);

    op_col->SetBranchAddress("polx", &polx);
    op_col->SetBranchAddress("poly", &poly);
    op_col->SetBranchAddress("polz", &polz);
    return true;
}

bool 
GtOPLoaderTool::add_optical_photon(HepMC::GenEvent& event)
{
    // = load the new OP =
    op_col -> GetEntry(m_index);
    // = check the event id (TODO) =
    // = create OP =
    double p_mom = std::sqrt(px*px+py*py+pz*pz);
    HepMC::GenParticle* particle = new HepMC::GenParticle(
                                        HepMC::FourVector(px,py,pz, p_mom),
                                        20022, // optical photon
                                        1);
    particle->set_polarization(HepMC::Polarization(
                                HepMC::ThreeVector(polx,poly,polz)));
    // = create Vertex =
    HepMC::GenVertex* vertex = new HepMC::GenVertex(HepMC::FourVector(x,y,z,t));
    vertex->add_particle_out(particle);
    // = =
    event.set_signal_process_vertex(vertex);
    return true;
}
