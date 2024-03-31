#ifndef GtOPLoaderTool_h
#define GtOPLoaderTool_h

/*  
 *
 * To do the muon simulation, we can have two stages:
 * 1. Do the full simulation, with Scintillation and Cerenkov enabled.
 *    Kill the generated Optical Photon during tracking and save the info.
 * 2. Load info from the file and do the Optical Photon Tracking in CPU or GPU.
 *
 * This class is used to load the root file contains a list of optical photon's
 * information. So we can do the OP Tracking in CPU.
 */

#include <string>

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

class TFile;
class TTree;

class GtOPLoaderTool: public ToolBase,
                      public IGenTool
{
    public:
        GtOPLoaderTool(const std::string& name);
        ~GtOPLoaderTool();

        bool configure();
        bool mutate(HepMC::GenEvent& event);

    private:
        bool initTree();
        bool add_optical_photon(HepMC::GenEvent& event);
    private:
        std::string m_filename;
        TFile* m_file;
        TTree* op_col;

        int m_total_entries;

        int m_chunk_size;  // chunk_size OPs per event
        int m_chunk_index; // 

        int m_index; // impl: the real index in the ROOT file
                    // make sure it is save, should not be greater
                    // than the std::numeric_limits<int>::max().
    private:
        // tree related variables
        int evtid;
        // time and position
        float t;
        float x;
        float y;
        float z;
        // momentum
        float px;
        float py;
        float pz;
        // polarization
        float polx;
        float poly;
        float polz;
};

#endif

