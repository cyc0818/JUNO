#include <boost/python.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "GtPositionerTool.h"

// geant4 Related
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "Randomize.hh"                                                         
#include "G4GeometryTolerance.hh"                                               
                                                                                
#include "G4VoxelLimits.hh"                                                     
#include "G4AffineTransform.hh"                                                 
#include "G4VisExtent.hh"  
#include "G4Material.hh"

DECLARE_TOOL(GtPositionerTool);

GtPositionerTool::GtPositionerTool(const std::string& name)
    : ToolBase(name)
{
    m_init_detector = true;
    declProp("GenInVolume", m_gen_volume); // Physical Volume's Name or User-Friendly name
    declProp("RealVolumes", m_real_volumes); // remap the user-friendly name
    declProp("Material", m_material);

    declProp("PositionMode", m_position_mode="Random");
    declProp("Positions", m_particlePositions);
    declProp("RadiusCut", m_particleRadiusCuts); // limit the position

    declProp("ZCut", m_particleZCuts);
    declProp("XCut", m_particleXCuts);
    declProp("YCut", m_particleYCuts);

    declProp("RockDepthCut", m_particleRockDepthCuts);

}

GtPositionerTool::~GtPositionerTool()
{

}

bool
GtPositionerTool::configure()
{
    if (m_position_mode == "Random" 
            and (m_material.size() == 0 or m_gen_volume.size() == 0)) {
        return false;
    }
    if (m_position_mode == "GenInGlobal") {
        // check the position 
        const std::vector<double>& pos = m_particlePositions;
        if (pos.size() != 3) {
            return false;
        }
        // the position is ok now
    }
    m_ExpHallGeomPar.InitExpHallVariables();
    if(m_particleRockDepthCuts.size() > 0 && m_gen_volume.compare("pTopRock") != 0){
        // Rock Depth Cut only works with TopRock!
        return false;
    }
    if(m_particleRockDepthCuts.size() == 1 && m_gen_volume.compare("pTopRock") == 0){
        // Calculate volume of rock used for producing decays so user can do
        // the appropriate user normalisation of the volume.
        double depth = m_particleRockDepthCuts[0];
        double expHallXLen     = m_ExpHallGeomPar.GetExpHallXLength();
        double expHallYLen     = m_ExpHallGeomPar.GetExpHallYLength();
        double expHallZLenSide = m_ExpHallGeomPar.GetExpHallZLengthSide();
        double expHall_domeRadius = m_ExpHallGeomPar.GetExpHallRLengthDome();
        double domeCenter_offsetZ = m_ExpHallGeomPar.GetExpHallDomeZOffset();

        double S_side = dome_side_surf(expHallYLen+2*depth, expHallZLenSide, expHall_domeRadius+depth, domeCenter_offsetZ);
        double S_empty = dome_side_surf(expHallYLen, expHallZLenSide, expHall_domeRadius, domeCenter_offsetZ);
        double rock_volume = S_side * depth * 2 + (S_side-S_empty) * expHallXLen;

        // 1e9 factor to convert from mm^3 to m^3
        LogInfo << "Rock depth option enabled for pTopRock. Rock volume used for distributing vertexes: " << rock_volume/1e9 << " m^3." << std::endl;
    }
    return true;
}

bool
GtPositionerTool::mutate(HepMC::GenEvent& event)
{
    // Get Fix/Random Position
    G4ThreeVector local_pos; 

    if (m_position_mode == "GenInGlobal") {
        const std::vector<double>& pos = m_particlePositions;
        local_pos = G4ThreeVector(pos[0], pos[1], pos[2]);
    } else { 
        if (m_init_detector) {
            if (init_detector()) {
                m_init_detector = false;
            } else {
                return false;
            }
            // after the initialize, print the current info
            LogDebug << "TransCache: " << std::endl;
            for (TransCache::iterator it = m_TC.begin();
                    it != m_TC.end(); ++it) {
                LogDebug << "+ " << it->first << std::endl;
                LogDebug << "++ " << (it->second).size() << std::endl;
            }
        }

        int total = m_TC[m_gen_volume].size();

        // Note: move the folloing three lines to the while-loop. 
        //       The problem is that we only select PMT once,
        //       the generated positions will be never in the range,
        //       then there will be dead-loop. 
        //       -- Tao Lin, 8th Aug 2022

        // int index = total*G4UniformRand();

        // G4AffineTransform g2l = m_TC[m_gen_volume][index];
        // G4AffineTransform l2g = g2l.Inverse();
        while (true) {
            // transform the position
            int index = total*G4UniformRand();

            const G4AffineTransform& g2l = std::get<0>(m_TC[m_gen_volume][index]);
            auto lv = std::get<1>(m_TC[m_gen_volume][index]);

            local_pos = get_pos(lv);
            LogDebug << "-- Local Position: " << local_pos.x() 
                                       << ", " << local_pos.y()
                                       << ", " << local_pos.z()
                                       << std::endl;

            G4AffineTransform l2g = g2l.Inverse();
            l2g.ApplyPointTransform(local_pos);
            LogDebug << "-- Global Position: " << local_pos.x() 
                                       << ", " << local_pos.y()
                                       << ", " << local_pos.z()
                                       << std::endl;
            // the local_pos is global now
            
            // select the material 
            if (is_that_material(local_pos) and is_in_range(local_pos)) {
                break;
            }
        }
    }

    // set the position in GenEvent
    HepMC::GenEvent::vertex_iterator vtx, done = event.vertices_end();
    HepMC::FourVector pos(local_pos.x(), local_pos.y(), local_pos.z(), 0);
    for (vtx = event.vertices_begin(); vtx != done; ++vtx) {
        HepMC::FourVector position4d = pos;
        position4d.setT((*vtx)->position().t());
        (*vtx)->set_position(position4d);
    }
    return true;
}

bool
GtPositionerTool::init_detector()
{
    G4Navigator* gNavigator =
        G4TransportationManager::GetTransportationManager()
        ->GetNavigatorForTracking();

    G4VPhysicalVolume* pworld = gNavigator->GetWorldVolume();
    G4LogicalVolume* lworld = pworld->GetLogicalVolume();

    // save PV name of world
    m_pv_name_cache.insert(pworld->GetName());

    traverse(lworld, 0);

    // go through all the volumes
    // Note: because some detector elements encoded copyno in names,
    //       so it's better to filter such name.
    //       The filter pattern:
    //         .+?\d+$
    //       \D: non-number, \d: number
    // const char* pattern = "(\\D+)\\d+";
    const char* pattern = "^(.+?)\\d+$";
    boost::regex pv_regex(pattern);
    boost::smatch what;

    std::map<std::string, int> counter_pv;

    LogInfo << "All available physical volumes: " << std::endl;
    for (LVMatCache::iterator it = m_pv_name_cache.begin();
	 it != m_pv_name_cache.end(); ++it) {

        if (regex_match(*it, what, pv_regex)) {
            if (what.size()) ++counter_pv[ what[1] ];
        } else {
	    LogInfo << " --> " << *it << std::endl;
        }
    }
    for (std::map<std::string, int>::iterator it = counter_pv.begin();
         it != counter_pv.end(); ++it) {
        LogInfo << " --> prefix: " << it->first << " repeated: " << it->second << std::endl;
    }

    // Check the material name in logical volumes
    if (m_lv_cache.size() == 0) {
        LogError << "Can not find any logical volumes for generating positions. " << std::endl;
        return false;
    }

    // as there are multiple LVs, need to check whether materials could be found in any of LVs.
    bool found_mat_in_LVs = false;
    for (auto& [name, selected_volume]: m_lv_cache) {
        // dump all the materials in selected_volume
        LogInfo << " All the materials in " << m_gen_volume << std::endl;
        m_lv_mat_cache.clear();
        traverse_mat(selected_volume);
        for (auto matname: m_lv_mat_cache) {
            LogInfo << " --> " << matname << std::endl;
        }
        if (m_lv_mat_cache.count(m_material)) {
            found_mat_in_LVs = true;
        }
    }

    if (!found_mat_in_LVs) {
        LogError << "Can't find material " << m_material
                 << " in volume " << m_gen_volume
                 << std::endl;
        return false;
    }

    return true;
}

bool
GtPositionerTool::traverse(G4LogicalVolume* lv, int depth, bool mute) 
{
    G4AffineTransform at_start;
    if (depth == 0) {
        m_path_cache.push_back(at_start);
    } else {
        at_start = m_path_cache.back();
    }

    // current volume
    std::string tmp_indent = "+";
    for (int i = 0; i < depth; ++i) {
        tmp_indent += "+";
    }
    if (not mute) {
    LogDebug << tmp_indent << " " << lv->GetName() << std::endl;
    }
    // daughter
    int daughter_count = lv->GetNoDaughters();

    G4String last_pv_name;
    int counter_pvs = 0;
    std::string tmp_indent_2 = "-";
    for (int j = 0; j < depth; ++j) { tmp_indent_2 += "-"; }

    for (int i = 0; i < daughter_count; ++i) {
        const G4VPhysicalVolume* const physvol = lv->GetDaughter(i);
        G4LogicalVolume* daughter = physvol->GetLogicalVolume();

	if (!m_pv_name_cache.count(physvol->GetName())) {
	    m_pv_name_cache.insert(physvol->GetName());
	}

        G4AffineTransform current = at_start;
        current.InverseProduct(
                    G4AffineTransform(at_start),
                    G4AffineTransform(
                        physvol->GetRotation(), physvol->GetTranslation()
                    )
                );

        // push
        m_path_cache.push_back(current);


        if (last_pv_name != physvol->GetName()) {
            if (counter_pvs > 1) {
                if (not mute) {
                LogDebug << tmp_indent_2 << ": total: " << counter_pvs << std::endl;
                }
            }
            if (not mute) {
            LogDebug << tmp_indent_2 << " [" 
                                     << physvol->GetName() 
                                     << "]" << std::endl;
            }
            last_pv_name = physvol->GetName();
            counter_pvs = 0;
        }

        ++counter_pvs;

        if (m_gen_volume.size() and physvol->GetName() == G4String(m_gen_volume)) {
            // find one of the volume
            // Save the Affine Transform
            m_TC[m_gen_volume].push_back(std::make_tuple(current, daughter));

            if (m_lv_cache.count(m_gen_volume) > 0) {
                assert ( daughter == m_lv_cache[m_gen_volume] );
            } else {
                m_lv_cache[m_gen_volume] = daughter;
            }
        } else if (m_gen_volume.size() and m_real_volumes.size()) {
            // if m_gen_volume is a user-friendly name, and there are m_real_volumes,
            // we need to compare the current name with m_real_volumes.
            for (auto real_volname: m_real_volumes) {
                if (physvol->GetName() == G4String(real_volname)) {
                    LogInfo << " ---> matching " << m_gen_volume << " with " << real_volname << std::endl;
                    // even though the logical volume could be different,
                    // we cache the affine transform in the same vector. 
                    // the key is user-friendly name
                    m_TC[m_gen_volume].push_back(std::make_tuple(current, daughter));

                    // for lv_cache, we use the real PV names as keys.
                    if (m_lv_cache.count(real_volname) > 0) {
                        if ( daughter != m_lv_cache[real_volname] ) {
                            LogError << "Mismatch LVs for the same name: " << real_volname << std::endl;
                            return false;
                        }
                    } else {
                        m_lv_cache[real_volname] = daughter;
                    }


                }
            }
        }


        bool tmp_mute = mute;
        if (counter_pvs > 1) {
            tmp_mute = true;
        }
        traverse(daughter, depth+1, tmp_mute);

        // pop
        m_path_cache.pop_back();
    }
    // end of loop, check the final count
    if (daughter_count > 1 and counter_pvs > 1) {
        if (not mute) {
        LogDebug << tmp_indent_2 << ": total: " << counter_pvs << std::endl;
        }
    }
    return true;
}

bool
GtPositionerTool::traverse_mat(G4LogicalVolume* lv) {

    G4String mat_name = lv->GetMaterial()->GetName();
    if (!m_lv_mat_cache.count(mat_name)) {
        m_lv_mat_cache.insert(mat_name);
    }

    int daughter_count = lv->GetNoDaughters();
    for (int i = 0; i < daughter_count; ++i) {
        const G4VPhysicalVolume* const physvol = lv->GetDaughter(i);
        G4LogicalVolume* daughter = physvol->GetLogicalVolume();

        traverse_mat(daughter);

    }

    return true;
}

G4ThreeVector
GtPositionerTool::get_pos(G4LogicalVolume* cur_lv) {
    // return local position
    // Get the solid of that LV
    // G4LogicalVolume* cur_lv = m_lv_cache[m_gen_volume];
    assert(cur_lv);
    G4VSolid* cur_solid = cur_lv->GetSolid();

    // Copy from G4VSolid.cc
    G4double px,py,pz,minX,maxX,minY,maxY,minZ,maxZ;
    G4VoxelLimits limit;                // Unlimited 
    EInside in;
    G4AffineTransform origin; 

    cur_solid->CalculateExtent(kXAxis,limit,origin,minX,maxX);                         
    cur_solid->CalculateExtent(kYAxis,limit,origin,minY,maxY);                         
    cur_solid->CalculateExtent(kZAxis,limit,origin,minZ,maxZ); 

    G4ThreeVector pos;
    G4double dX=maxX-minX;                                                        
    G4double dY=maxY-minY;                                                        
    G4double dZ=maxZ-minZ; 
    while (true) {
        px = minX+dX*G4UniformRand();
        py = minY+dY*G4UniformRand();
        pz = minZ+dZ*G4UniformRand();

        pos  = G4ThreeVector(px,py,pz);
        in = cur_solid->Inside(pos);

        if (in != kOutside) {
            return pos;
        }
    }

}

bool
GtPositionerTool::is_that_material(const G4ThreeVector& global_pos)
{
    G4Navigator* gNavigator =
        G4TransportationManager::GetTransportationManager()
        ->GetNavigatorForTracking();
    G4VPhysicalVolume* pv;
    pv = gNavigator ->
            LocateGlobalPointAndSetup(global_pos, 0, true);
    G4LogicalVolume* pv_log = pv->GetLogicalVolume();
    G4Material* pv_mat = pv_log->GetMaterial();
    return pv_mat->GetName() == G4String(m_material);

}

bool
GtPositionerTool::is_in_range(const G4ThreeVector& global_pos)
{
    bool isOK = false;

    int size = m_particleRadiusCuts.size();
    double r = global_pos.getR();
    if (size == 0) {
        // no cut
        isOK = true;
    } else if (size == 1) {
        // r < max
        if (r<m_particleRadiusCuts[0]) {
            isOK = true;
        } else {
            isOK = false;
        }
    } else if (size == 2) {
        double r_min = std::min(m_particleRadiusCuts[0], m_particleRadiusCuts[1]);
        double r_max = std::max(m_particleRadiusCuts[0], m_particleRadiusCuts[1]);

        if (r_min<=r and r<r_max) {
            isOK = true;
        } else {
            isOK = false;
        }
    }

    // now, check z cut
    size = m_particleZCuts.size();
    if (size == 2) {
        double z_min = std::min(m_particleZCuts[0], m_particleZCuts[1]);
        double z_max = std::max(m_particleZCuts[0], m_particleZCuts[1]);
        if (z_min <= global_pos.z() and global_pos.z() <= z_max) {
            isOK = isOK and true;
        } else {
            isOK = isOK and false;
        }
    }

    // now, check x cut
    size = m_particleXCuts.size();
    if (size == 2) {
        double x_min = std::min(m_particleXCuts[0], m_particleXCuts[1]);
        double x_max = std::max(m_particleXCuts[0], m_particleXCuts[1]);
        if (x_min <= global_pos.x() and global_pos.x() <= x_max) {
            isOK = isOK and true;
        } else {
            isOK = isOK and false;
        }
    }
    // now, check y cut
    size = m_particleYCuts.size();
    if (size == 2) {
        double y_min = std::min(m_particleYCuts[0], m_particleYCuts[1]);
        double y_max = std::max(m_particleYCuts[0], m_particleYCuts[1]);
        if (y_min <= global_pos.y() and global_pos.y() <= y_max) {
            isOK = isOK and true;
        } else {
            isOK = isOK and false;
        }
    }

    // now, check depth
    size = m_particleRockDepthCuts.size();
    if(size == 1 && isOK){
        double depth = m_particleRockDepthCuts[0];
        double m_expHallXLen     = m_ExpHallGeomPar.GetExpHallXLength();
        double m_expHallYLen     = m_ExpHallGeomPar.GetExpHallYLength();
        double m_expHallZLenSide = m_ExpHallGeomPar.GetExpHallZLengthSide();
        double m_expHall_domeRadius = m_ExpHallGeomPar.GetExpHallRLengthDome();
        double m_expHall_offsetX = m_ExpHallGeomPar.GetExpHallXOffset();
        double m_expHall_offsetY = m_ExpHallGeomPar.GetExpHallYOffset();
        double m_expHall_lowestZ = m_ExpHallGeomPar.GetExpHallZLowest();
        double m_domeCenter_offsetZ = m_ExpHallGeomPar.GetExpHallDomeZOffset();

        if(fabs(global_pos.x() - m_expHall_offsetX) > m_expHallXLen/2 + depth){
            isOK = false;
        }
        else if(fabs(global_pos.y() - m_expHall_offsetY) > m_expHallYLen/2 + depth){
            isOK = false;
        }
        else if(global_pos.z() > m_expHall_lowestZ + m_expHallZLenSide){
            // Vertex in dome...
            float z_dist = global_pos.z() - m_expHall_lowestZ - m_domeCenter_offsetZ;
            float r_dome = sqrt(pow(global_pos.y() - m_expHall_offsetY,2) + pow(z_dist,2));
            if( r_dome > m_expHall_domeRadius + depth) isOK = false;
        }
    }

    return isOK;
}

double
GtPositionerTool::dome_side_surf(double D, double H, double R, double dh){
    double dr = R - sqrt( pow(H-dh,2) + pow(D/2, 2) );
    double th = atan(D/(2*(H-dh)));
    double dth = th - asin(D/(2*R));
    double S_square = D * H;
    double S_arc    = th * pow(R,2);
    double S_triangle = D * (H - dh) / 2;
    double S_extra_arc = R * ((dth * R) - ((R-dr)*sin(dth))) / 2;
    return S_square + S_arc - S_triangle - 2 * S_extra_arc;
}
