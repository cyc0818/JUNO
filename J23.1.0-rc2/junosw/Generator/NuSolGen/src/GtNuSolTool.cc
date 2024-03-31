#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/Incident.h"
#include "RootWriter/RootWriter.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "CLHEP/Units/SystemOfUnits.h"
#include "TTree.h"
#include "TFile.h"

#include "GtNuSolTool.h"

DECLARE_TOOL(GtNuSolTool);

GtNuSolTool::GtNuSolTool(const std::string& name)
    : ToolBase(name)
{
    m_eventSource = 0;
    m_inputMode = "generateRealTime";
    m_saveVertex = false;
    m_saveRootFileName = "NuSolarGen.root";
    m_treeName = "NuSol";
    m_startIndex = 0;
    m_neutrinoType = "B8";
    // general
    declProp("inputMode", m_inputMode); //fromRootFile|generateRealTime
    declProp("saveVertex", m_saveVertex);
    declProp("saveRootFileName", m_saveRootFileName);
    // for fromRootFileMode
    declProp("inputFile", m_inputFile);
    declProp("treeName", m_treeName);
    declProp("startIndex", m_startIndex);
    // for generateRealTime
    declProp("neutrinoType", m_neutrinoType);
}

GtNuSolTool::~GtNuSolTool()
{

}

bool
GtNuSolTool::configure()
{
    bool ok = true;
    if(m_inputMode=="fromRootFile") {
        m_eventSource = new NuSol::fromRootFile;
        ok*=((NuSol::fromRootFile*)m_eventSource)->SetInputTTree(m_inputFile,m_treeName);
        ok*=((NuSol::fromRootFile*)m_eventSource)->SetStartIndex(m_startIndex);
    }
    else if(m_inputMode=="generateRealTime") {
        m_eventSource = new NuSol::generateRealTime;
        ok*=((NuSol::generateRealTime*)m_eventSource)->Initialize(m_neutrinoType);
    }
    else 
    {
        LogError << "Unknown input mode :"<<m_inputMode<<std::endl;
        LogError << "Possible mode: fromRootFile|generateRealTime"<<std::endl;
        return false;
    }
    if(!ok) return false;
    return true;
}

bool
GtNuSolTool::mutate(HepMC::GenEvent& event) 
{
    if(!m_eventSource) {
        LogError << "Strange error.. event source pointer is 0x0"<<std::endl;
        return false;
    }
    double electron_kinetic_energy = 0;
    LogDebug << " get electron_kinetic_energy ." << std::endl;
    bool ok = m_eventSource->mutate(electron_kinetic_energy);
    LogDebug << " ok: get Ek. " << std::endl;
    if(!ok) {
        LogError << "failed to generate electron Ek." << std::endl;
        return false;
    }
    double emass = 0.510998928*CLHEP::MeV;
    double electron_momentum = sqrt(pow(electron_kinetic_energy+emass,2)-emass*emass);
    double costheta = CLHEP::RandFlat::shoot(-1.0, 1.0);
    double phi = 2 * M_PI * CLHEP::RandFlat::shoot();
    double px = electron_momentum * sqrt(1 - costheta * costheta) * cos(phi); // MeV
    double py = electron_momentum * sqrt(1 - costheta * costheta) * sin(phi); // MeV
    double pz = electron_momentum * costheta; // MeV
    double Ee = electron_kinetic_energy+emass;

    LogDebug << " generate vertex. " << std::endl;
    HepMC::GenVertex* vertex = 
        new HepMC::GenVertex(HepMC::FourVector(0,0,0,0*CLHEP::nanosecond));
    event.set_signal_process_vertex(vertex);


    LogDebug << " generate particle." << std::endl;
    HepMC::GenParticle* particle = new HepMC::GenParticle( 
                                                          HepMC::FourVector(px,py,pz,Ee),
                                                          11, // electron
                                                          1 /* status */
                                                           ); 
    vertex->add_particle_out(particle);
    LogDebug << "done." << std::endl;
    return true;
}

namespace NuSol {
    bool fromRootFile::SetInputTTree(std::string &inputFile,std::string &treeName) {
        TFile *f = TFile::Open(inputFile.c_str());
        if(!f||!(f->IsOpen())) {
            LogError<<"file \""<<inputFile<<"\" not found."<<std::endl;
            LogError<<"Please set gun.property(\"inputFile\").set(\"NuSol.root\");"<<std::endl;
            return false;
        }
        m_tree=(TTree*)(f->Get(treeName.c_str()));
        if(!m_tree) {
            LogError<<"tree \""<<treeName<<"\" not found."<<std::endl;
            LogError<<"Please set gun.property(\"treeName\").set(\"NuSol\");"<<std::endl;
            return false;
        }
        m_tree->SetBranchAddress("electron_kinetic_energy",&m_electron_kinetic_energy);
        m_maxIndex = m_tree->GetEntries();
        //f->Close(); // TODO: the file should be closed
        return true;
    };
    bool fromRootFile::SetStartIndex(int startIndex) {
        m_index = startIndex;
        if((m_index<0)||(m_index>m_maxIndex)) {
            LogError<<"Index "<<m_index<<" unreasonable, and max index is "<<m_maxIndex<<std::endl;
            LogError<<"Please set gun.property(\"startIndex\").set(0);"<<std::endl;
            return false;
        }
        return true;
    };
    bool fromRootFile::mutate(double &electron_kinetic_energy) {
        //LogInfo<<m_index<<" "<<m_maxIndex<<" "<<m_tree<<" "<<m_tree->GetEntries()<<std::endl;
        //m_tree->Scan("*");
        if(m_index>=m_maxIndex) 
            return false;
        m_tree->GetEntry(m_index);
        electron_kinetic_energy = m_electron_kinetic_energy;
        ++m_index;
        return true;
    }
    bool generateRealTime::Initialize(std::string &neutrinoType) {
        return m_solargen.SetNeutrinoType(neutrinoType.c_str());
    };
    bool generateRealTime::mutate(double &electron_kinetic_energy) {
        return m_solargen.GeneratePrimaries(electron_kinetic_energy);
    };
}
