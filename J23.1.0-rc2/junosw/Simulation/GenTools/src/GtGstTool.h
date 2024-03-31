#ifndef GtGstTool_h
#define GtGstTool_h

/*
 * Load Data from GENIE's gst file.
 */

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

class TTree;
class TFile;

class GtGstTool: public ToolBase,
                 public IGenTool
{
    public:
        GtGstTool(const std::string& name);
        ~GtGstTool();

        bool configure();
        bool mutate(HepMC::GenEvent& event);

    private:
        bool initTree();
    private:
        std::string m_gst_filename;
        TFile* gst;
        TTree* evt;

        TTree* output_gst; // save the tree in another file

    private:
        void save_the_gst_partial();

    private:
        // hardon
        static const int kNPmax = 250;
        int gst_entry;
        // Tree Branch
        // The detail info is copied from Genie 2.8.0
        // It is in src/stdapp/gNtpConv.cxx
        int    brIev           ;      // Event number                                
        int    brNeutrino      ;      // Neutrino pdg code                           
        int    brTarget        ;      // Target pdg code (10LZZZAAAI)                           
        bool   brQel           ;      //quasi-elastic scattering event
        bool   brRes           ;      //resonanec neutrino-production event 
        bool   brDis           ;      //deep-inelastic scattering event
        bool   brCoh           ;      //coherent meson production event
        bool   brDfr           ;      //diffractive meson production event
        bool   brImd           ;      //invese muon decay event
        bool   brNuel          ;      //ve- elastic event
        bool   brCC            ;      //CC event
        bool   brNC            ;      //NC event

        int    brFSPrimLept    ;      // Final state primary lepton pdg code     
        double brEl            ;      // Final state primary lepton energy @ LAB     
        double brPxl           ;      // Final state primary lepton px @ LAB         
        double brPyl           ;      // Final state primary lepton py @ LAB         
        double brPzl           ;      // Final state primary lepton pz @ LAB         
        double brPl            ;      // Final state primary lepton p  @ LAB         
        double brCosthl        ;      // Final state primary lepton cos(theta) wrt to neutrino direction

        double brEv            ;      // Neutrino energy @ LAB                       
        double brPxv           ;      // Neutrino px @ LAB                           
        double brPyv           ;      // Neutrino py @ LAB                           
        double brPzv           ;      // Neutrino pz @ LAB         

        int    brNf            ;      // Nu. of final state particles in hadronic system
        int    brPdgf  [kNPmax];       // Pdg code of k^th final state particle in hadronic system
        double brEf    [kNPmax];       // Energy     of k^th final state particle in hadronic system @ LAB
        double brPxf   [kNPmax];       // Px         of k^th final state particle in hadronic system @ LAB
        double brPyf   [kNPmax];       // Py         of k^th final state particle in hadronic system @ LAB
        double brPzf   [kNPmax];       // Pz         of k^th final state particle in hadronic system @ LAB
        double brPf    [kNPmax];       // P          of k^th final state particle in hadronic system @ LAB
        double brCosthf[kNPmax];       // cos(theta) of k^th final state particle in hadronic system @ LAB wrt to neutrino direction

        double brVtxX;                 // Vertex x in detector coord system (SI)
        double brVtxY;                 // Vertex y in detector coord system (SI)
        double brVtxZ;                 // Vertex z in detector coord system (SI)
        double brVtxT;                 // Vertex t in detector coord system (SI)

        // extra flags for the tool
        bool m_gst_vertex_xyz_enabled; // use the vertex x/y/z (default: disabled)
        bool m_gst_vertex_t_enabled;   // use the vertex t (default: disabled)
        double m_scale_xyz;              // scale from units in gst to units in geant4
        double m_scale_t;                // scale from units in gst to units in geant4
};


#endif
