#ifndef GtPositionerTool_h
#define GtPositionerTool_h

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

#include "Parameter/ExpHallGeomPar.h"

#include <map>
#include <vector>
#include <tuple>

#include <G4AffineTransform.hh>

class G4LogicalVolume;
class G4VPhysicalVolume;

class GtPositionerTool: public ToolBase,
                        public IGenTool
{
    public:
        GtPositionerTool(const std::string& name);
        ~GtPositionerTool();
        // The current toolbase don't have configure.
        bool configure();
        bool mutate(HepMC::GenEvent& event);
    private:

        bool init_detector();

        bool traverse(G4LogicalVolume*, int depth, bool mute=false);
        bool traverse_mat(G4LogicalVolume*);

        G4ThreeVector get_pos(G4LogicalVolume*);
        bool is_that_material(const G4ThreeVector& global_pos);
        bool is_in_range(const G4ThreeVector& global_pos);

        // Helper function to calculate volume of rock. This function
        // calculates the surface of the dome/arc side.
        // D = length of the side
        // H = height up to the start of the arc
        // R = radius of the arc
        // dh = height of the center of the radius of the arc
        double dome_side_surf(double D, double H, double R, double dh);
    private:
        bool m_init_detector;
        std::string m_gen_volume;
        std::vector<std::string> m_real_volumes; // this is used to remap user-friendly name to real names
        std::string m_material;

        std::string m_position_mode;
        std::vector<double> m_particlePositions;
        std::vector<double> m_particleRadiusCuts;

        // specify the range [zmin, zmax]
        std::vector<double> m_particleZCuts;
        // specify the range [xmin, xmax]
        std::vector<double> m_particleXCuts;
        // specify the range [ymin, ymax]
        std::vector<double> m_particleYCuts;

        // specify the rock depth to be considered as [dmax]
        std::vector<double> m_particleRockDepthCuts;

        ExpHallGeomPar m_ExpHallGeomPar;

    private:
        // Global to Local!!!
        typedef std::vector<G4AffineTransform> AffineTransVector;

        // 2022.11.26: to support the multiple LVs with the same user friendly name,
        //             all the logical volumes are cached
        typedef std::tuple<G4AffineTransform, G4LogicalVolume*> AffineLV;
        typedef std::vector<AffineLV> AffineLVVector;

        // typedef std::map< std::string, AffineTransVector> TransCache;
        typedef std::map< std::string, AffineLVVector> TransCache;
        // the key is name of physical volume (PV) or user-friendly name
        TransCache m_TC;

        typedef std::map< std::string, G4LogicalVolume*> LVCache;
        LVCache m_lv_cache; // make sure the multi PV -> one LV

        // 2017.03.01 Tao Lin
        // Check material is really in the logical volume.
        // So when logical volume is found, we start to save all the materials
        // in such cache.
        typedef std::set<std::string> LVMatCache;
        LVMatCache m_lv_mat_cache;

        // 2017.06.15 Tao Lin
        // It would be useful to print a list of logical volume
        // and its contained materials.
        LVMatCache m_pv_name_cache; //
        LVMatCache m_lv_mat_cache_for_debug;

        // when traverse the LV, save the current Affine Transform
        // Global to Local!!!
        AffineTransVector m_path_cache;
};

#endif

