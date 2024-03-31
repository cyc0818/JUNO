#ifndef Calib_GuideTube_Construction_hh
#define Calib_GuideTube_Construction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

#include <vector>

class G4Tubs;
class G4Torus;
class G4Sphere;
class G4Polycone;
class G4Ellipsoid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VSolid;

class Calib_GuideTube_Construction: public IDetElement,
                             public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    Calib_GuideTube_Construction(const std::string& name);
    ~Calib_GuideTube_Construction();

private:
    int counter;

    void GT_Parameters();
    void initMaterials();
    void initVariables();

    void makeSurftubeLogical();
    void makevacSurftubeLogical();
    void makevacSurftubePhysical();
    void SetGuideTubeReflectivity();
    void makeSourceEnclosureLogical();
    void makeSourceEnclosurePhysical();

private:

    enum {GT_number = 39};
    std::vector<G4Torus*> solidSurftube = std::vector<G4Torus*>(GT_number,0);
    std::vector<G4LogicalVolume*> logicSurftube = std::vector<G4LogicalVolume*>(GT_number,0);

    std::vector<G4Torus*> solidvacSurftube = std::vector<G4Torus*>(GT_number,0);
    std::vector<G4LogicalVolume*> logicvacSurftube = std::vector<G4LogicalVolume*>(GT_number,0);
    std::vector<G4VPhysicalVolume*> physivacSurftube = std::vector<G4VPhysicalVolume*>(GT_number,0);

    G4Polycone* solidSourceEnclosure_shell;
    G4LogicalVolume* logicSourceEnclosure_shell;

    G4VSolid* solidSourceEnclosure_innershell;
    G4Tubs* solidSourceEnclosure_innershell_a;
    G4Tubs* solidSourceEnclosure_innershell_b;

    G4Tubs* solidSourceEnclosure_screw;

    G4VSolid* solidSourceEnclosure_inner_uni1;
    G4VSolid* solidSourceEnclosure_inner_uni2;
    G4VSolid* solidSourceEnclosure_inner_uni3;
    G4VSolid* solidSourceEnclosure_inner;

    G4LogicalVolume* logicSourceEnclosure_inner;

    G4Tubs* solidSourceEnclosure_capsule;
    G4LogicalVolume* logicSourceEnclosure_capsule;

    G4Tubs* solidSourceEnclosure_cavity;
    G4LogicalVolume* logicSourceEnclosure_cavity;

    G4VPhysicalVolume* physiSourceEnclosure_shell;
    G4VPhysicalVolume* physiSourceEnclosure_inner;
    G4VPhysicalVolume* physiSourceEnclosure_capsule;
    G4VPhysicalVolume* physiSourceEnclosure_cavity;

/*

                        source enclosure & capsule

                                 |-8.2mm-|

                                ___________       ___
                               /___     ___\
                              / | |     | | \      |
                             / _| |_____| |_ \     |
                             | |           | |     |
                             | | _________ | |     |
                             | | |_______| | |     |
                        ___  | | |_______| | |     |
                             | | | _____ | | |     |
                         |   | | | |   | | | |   27mm
 Am-C in the center--> 8.5mm | | | |   | | | |     | 
                         |   | | | |___| | | |     |
                        ___  | | |_______| | |     |
                             | |           | |     |
                             | |_   ____  _| |     |
                             \   | |    | |  /     |
                              \  |_|    |_| /      |
                               \___________/      ___

                             |-----13 mm-----|
  
*/

private:
    G4Material* Acrylic;
    G4Material* Teflon;                                                               
    G4Material* StainlessSteel;                                                               
    G4Material* VacuumT;                                                           
    G4Material* Mylar;  
    G4Material* Steel; 
    G4Material* Air;
    G4Material* Al;
    G4Material* Water;
    G4Material* PE_PA;
    bool m_use_source;
    double m_theta;
    double m_rad_source;

    // Guide Tube design option
    // * V0: Implemented by Yuhang Guo, XJTU
    //       JUNO-doc-1238-v1, JUNO-doc-1240-v1
    // * V1: Implemented by Tao Lin, IHEP, 2019.08.26
    //       In this simplified implementation, a half torus will be constructed.
    //       Then, it will placed into water with two placements.
    //       In order to identify them, the suffix _0 and _1 are used.
    //       * V1_0
    //       * V1_1
    //       Modified by Yankai Liu, XJTU, 2022.02.28
    //       In this implementation, a half torus will be divided into 39 segments.
    std::string m_guidetube_option;
    G4double m_rad_guidetube;
private:
    //            y ^
    //              | 
    //            \ | /
    //             \|/
    //              -------->
    //                      x
    // Rotate 90.*deg along X, then the torus is in x-z plane 
    //                               Option V0;
    double m_torus_swipe_phi_start; // 92.*deg;
    double m_torus_swipe_phi_delta; // 356.*deg;

    double m_torus_swipe_phi_Start[GT_number];
    double m_torus_swipe_phi_Delta[GT_number];
    double m_rad_guidetube_Plus[GT_number];

};

#endif
