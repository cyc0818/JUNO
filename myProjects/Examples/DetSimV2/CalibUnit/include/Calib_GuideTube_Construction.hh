#ifndef Calib_GuideTube_Construction_hh
#define Calib_GuideTube_Construction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

#include"G4SubtractionSolid.hh"

class G4Tubs;
class G4Torus;
class G4Sphere;
class G4Ellipsoid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class Calib_GuideTube_Construction: public IDetElement,
                             public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    Calib_GuideTube_Construction(const std::string& name);
    ~Calib_GuideTube_Construction();

private:
    void initMaterials();
    void initVariables();

    void makeSurftubeLogical();
	void makevacSurftubeLogical();
	void makevacSurftubePhysical();
	void SetGuideTubeReflectivity();
    void makeSourceEnclosureLogical();
    void makeSourceEnclosurePhysical();

private:
	G4Torus*    solidSurftube;
    G4LogicalVolume* logicSurftube;

    G4Torus*    solidvacSurftube;
    G4LogicalVolume* logicvacSurftube;
    G4VPhysicalVolume* physivacSurftube;

    G4SubtractionSolid* solidSourceEnclosure;
    G4Tubs* solidSourceEnclosurea;
    G4Tubs* solidSourceEnclosureb;
    G4LogicalVolume* logicSourceEnclosure;
    G4VPhysicalVolume* physiSourceEnclosure;

private:
    G4Material* Acrylic;
    G4Material* Teflon;                                                               
    G4Material* SS;                                                               
    G4Material* Vacuum;                                                           
    G4Material* Mylar;  
    G4Material* Steel; 
    G4Material* Air;
    G4Material* Al;
	bool m_use_source;
	double m_theta;

    // Guide Tube design option
    // * V0: Implemented by Yuhang Guo, XJTU
    //       JUNO-doc-1238-v1, JUNO-doc-1240-v1
    // * V1: Implemented by Tao Lin, IHEP, 2019.08.26
    //       In this simplified implementation, a half torus will be constructed.
    //       Then, it will placed into water with two placements.
    //       In order to identify them, the suffix _0 and _1 are used.
    //       * V1_0
    //       * V1_1
    std::string m_guidetube_option;

private:
    //            y ^
    //              | 
    //            \ | /
    //             \|/
    //              -------->
    //                      x
    // Rotate 90.*deg along X, then the torus is in x-z plane 
    //                               Option V0;
    double torus_swipe_phi_start; // 92.*deg;
    double torus_swipe_phi_delta; // 356.*deg;
};

#endif
