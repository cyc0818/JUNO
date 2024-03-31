#ifndef RelationFinder_hh
#define RelationFinder_hh

#include "SniperKernel/AlgBase.h"
#include <vector>
#include "TVector3.h"

class TTree;
namespace JM {
    class SimHeader;
    class SimEvt;
}

class RelationFinder: public AlgBase {
    public:
        RelationFinder(const std::string& name);
        ~RelationFinder();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        bool load_sim_event();
        bool calculate_edep_pos();
        bool calculate_theta();
        bool calculate_theta_hits();

        // debug mode
        bool debug_counting();

    private:
        JM::SimHeader* m_simheader;
        JM::SimEvt* m_simevent;
        TTree* m_evt_tree;

        int Total_num_PMT;
        std::vector<TVector3> ALL_PMT_pos;


        float m_edep_x;
        float m_edep_y;
        float m_edep_z;
        float r_vertex;
        TVector3 m_vertex;

        // theta between the PMT and vertex
        std::vector<double> ALL_PMT_theta; 
        float theta;
        float hittime;

        // mean nPE
        TTree* m_npe;
        std::vector<int> ALL_PMT_npe;
        int npe;

        // save geometry
        bool m_output_geom_enable;
        TTree* m_geom;
        int m_geom_id;
        int m_geom_type;
        float m_geom_x;
        float m_geom_y;
        float m_geom_z;

        // using optical photon as input
        // true: the input is optical photon
        // false: gamma or electron
        //
        // if the input is optical photon, use init position 
        // instead of edep position
        bool m_photons_only;

        // used for counting
        Long64_t entry;
        bool m_counting_only;
};

#endif
