#ifndef RockConstruction_hh
#define RockConstruction_hh

/*  
 * In current implmentation, the Rock is divided into two parts.
 *
 * * Top Rock (Dome + Box)
 *   + Exp. Hall (Dome + Box)
 *
 * * Bottom Rock (Tube)
 *   + Water Pool Lining 
 *     
 *     + Central Detector
 */

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Box;
class G4Sphere;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VSolid;

class TopRockConstruction : public IDetElement,
                            public ToolBase {
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    double geom_info(const std::string& param);
    double geom_info(const std::string& , int) { return 0.0; }

    TopRockConstruction(const std::string& name);
    ~TopRockConstruction();

private:

    void initMaterials();
    void initVariables();
    
    //Top Rock was divided two parts, upper box for dome, under box for Exp. Hall
    void makeTopRockLogical();
    void makeTopRockUpperLogical();//upper box
    void makeTopRockUpperPhysical();
    void makeTopRockDomeLogical(); //dome rock
    void makeTopRockDomePhysical();
    void makeTopRockDomeAirLogical();//semi-cylindircal air
    void makeTopRockDomeAirPhysical();
    void makeTopRockUnderLogical();//under box for ExpHall
    void makeTopRockUnderPhysical();
    
    

    G4VSolid*        solidTopRock;
    G4LogicalVolume* logicTopRock;

    G4VSolid*        solidDomeRockBox;
    G4LogicalVolume* logicDomeRockBox;

    G4VSolid*        solidTopRock_dome;
    G4LogicalVolume* logicTopRock_dome;

    G4VSolid*        solidTopRock_domeAir;
    G4LogicalVolume* logicTopRock_domeAir;

    G4VSolid* solidExpRockBox;
    G4LogicalVolume* logicExpRockBox; 

    G4Material* Rock;
    G4Material* Air;
    G4Material* Galactic;

    //Top Rock
    double m_topRockX;
    double m_topRockY;
    double m_topRockZ;
    double m_topRockZ_tot;
    double m_topRock_offsetX;
    double m_topRock_offsetY;
    double m_topRock_offsetZ; //relative to world

    double m_topRock_domeHeight;
    double m_topRock_domeRadius;
    double m_topRock_domeOffsetZ;

    //Exp Hall dome
    double m_expHallXLen_tot;
    double m_expHall_domeHeight;
    double m_expHall_domeRadius;

    double m_expHall_offsetX;
    //double m_expHall_offsetZ;
};

class BottomRockConstruction : public IDetElement,
                               public ToolBase {
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    BottomRockConstruction(const std::string& name);
    ~BottomRockConstruction();

private:

    void initMaterials();
    void initVariables();

    void makeBottomRockLogical();
    void makePoolLiningLogical();
    void makePoolLiningPhysical();

    G4VSolid*        solidBottomRock;
    G4LogicalVolume* logicBottomRock;

    G4VSolid*        solidPoolLining;
    G4LogicalVolume* logicPoolLining;
    G4VPhysicalVolume* physiPoolLining;

    G4Material* Rock;
    G4Material* Tyvek;

    double m_btmRockR;
    double m_btmRockH;
    double m_btmRockZtop;
    double m_btmRockZbottom;
    double m_poolLiningR;
    double m_poolLiningH;
    double m_poolLiningZtop;
    double m_poolLiningZbottom;
};

#endif
