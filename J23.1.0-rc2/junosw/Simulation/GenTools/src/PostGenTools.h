#ifndef PostGenTools_h
#define PostGenTools_h

#include "SniperKernel/AlgBase.h"
#include "TTree.h"

class PostGenTools: public AlgBase
{
    public:
        PostGenTools(const std::string& name);
        ~PostGenTools();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        // Evt Data
        TTree* m_evt_tree;
        int m_eventID;
        int m_init_nparticles; // The total number of primary tracks

        Int_t m_init_pdgid[50000];
        float m_init_x[50000];
        float m_init_y[50000];
        float m_init_z[50000];
        float m_init_px[50000];
        float m_init_py[50000];
        float m_init_pz[50000];
        float m_init_kine[50000];
        float m_init_mass[50000];
        float m_init_time[50000];
};

#endif

