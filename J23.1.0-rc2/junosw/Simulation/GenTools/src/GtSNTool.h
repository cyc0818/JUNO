#ifndef GtSNTool_h
#define GtSNTool_h

/*
 * Load Data from GENIE's gst file.
 */

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

class TTree;
class TFile;

typedef struct
{
    ULong64_t nparticles;
    int pdgid[2];
    double px[2];   //MeV
    double py[2];   //MeV
    double pz[2];   //MeV
    double m[2];    //MeV
    double t[2];    //n
    double nuE;    //MeV
} EVENT;

class GtSNTool: public ToolBase,
                 public IGenTool
{
    public:
        GtSNTool(const std::string& name);
        ~GtSNTool();

        bool configure();
        bool mutate(HepMC::GenEvent& event);

    private:
        bool initTree();
    private:
        std::string m_gst_filename;
        TFile* gst;
        TTree* evttree;

        TTree* output_gst; // save the tree in another file

    private:
        int gst_entry;
        // Tree Branch
        EVENT evt;

};


#endif
