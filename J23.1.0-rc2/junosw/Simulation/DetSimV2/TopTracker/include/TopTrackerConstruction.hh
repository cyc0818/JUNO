#ifndef TopTrackerConstruction_hh
#define TopTrackerConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"

#include <map>
#include <set>

#include "globals.hh"

class G4Box;
class G4Sphere;
class G4Tubs;
class G4SubtractionSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class TopTrackerConstruction : public IDetElement,
                            public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    double geom_info(const std::string&) { return 0.0; }
    double geom_info(const std::string& param,  const int wallnumber);

    TopTrackerConstruction(const std::string& name);
    ~TopTrackerConstruction();

private:

    /// Read TT wall placement from file on include/TopTrackerWallPlacement.txt
    void readWallPlacement(std::string & txt_file);

    void initMaterials();
    void initVariables();

    void makeAirTTLogical();
                                
    void makeStripLogical();
    void makeStripPhysical();
    void makeStripG4Sensitive();
               
    void makeCoatingLogical();
    void makeCoatingPhysical();
                                
    void makePanelLogical();
    void makePanelPhysical(int panel_mask);
                            
    void makePanelTapeLogical();
    void makePanelTapePhysical();
                                
    void makePlaneLogical();
    void makePlanePhysical(int plane_mask);

    void makeWallLogical();
    void makeWallPhysical();

    
private:

    G4Box*              BoxsolidAirTT; 
    G4SubtractionSolid* solidAirTT; //Logic volume containing TT
    G4Tubs*             solidChimney;
   
    G4LogicalVolume*    logicAirTT;

   
                                
                                
                                
    G4Box*              solidBar; //Scintillator
    G4LogicalVolume*    logicBar;
    G4VPhysicalVolume*  physiBar;
                                
    G4Box*              solidCoating; //TiO2
    G4LogicalVolume*    logicCoating;
    G4VPhysicalVolume*  physiCoating[64];
    
    G4Box*              solidPanelTape; //Tape between scintillator and Al
    G4LogicalVolume*    logicPanelTape;
    G4VPhysicalVolume*  physiPanelTape;
                                
    G4Box*              solidPanel; //Aluminium box
    G4LogicalVolume*    logicPanel;
    std::vector<G4VPhysicalVolume*> physiPanel;
                                
    G4Box*              solidPlane; //4 panels
    std::map<int, G4LogicalVolume*> logicPlane;
    std::vector<G4VPhysicalVolume*> physiPlane;
                                
    G4Box*              solidWall;// 2 planes 1 X and 1 Y
    std::map<int, G4LogicalVolume*> logicWall;
    std::vector<G4VPhysicalVolume*> physiWall;
                                
                                
private:

    G4Material* Aluminium;           // TT material
    G4Material* Scintillator;
    G4Material* TiO2Coating;
    G4Material* Adhesive;
    G4Material* Air;
    
private:
    double m_box_x;
    double m_box_y;
    double m_box_z;

    double m_chimney_R;
    double m_chimney_Z;

    double m_wall_x;
    double m_wall_y;
    double m_wall_z;
                                
    double m_lengthBar;
    double m_thicknessBar;
    double m_widthBar;
    double m_spaceBar;
               
    double m_lengthCoating;
    double m_thicknessCoating;
    double m_widthCoating;
                          
    double m_lengthPanelTape;
    double m_thicknessPanelTape;
    double m_widthPanelTape;
                    
    double m_lengthPanel;
    double m_thicknessPanel;
    double m_widthPanel;
    double m_spacePanel;

    double m_lengthPlane;
    double m_thicknessPlane;
    double m_widthPlane;
    double m_vspacePlane;
                        
    //wall positions
    double m_floor_level;

    std::map<int, double> m_wall_posx;
    std::map<int, double> m_wall_posy;
    std::map<int, double> m_wall_posz;
    std::map<int, int>    m_wall_layer;
    std::map<int, int>    m_wall_column;
    std::map<int, int>    m_wall_row;

    double m_panel_posxy[4];
    double m_bar_posxy[64];
    double m_plane_posz[2];
                  

    bool IsOverlap;


    struct wall_pos {
      float xc;
      float yc;
      float zc;
      float angle;
      int panel_mask;
    };
    std::map<int, struct wall_pos> * wall_mapping;

    std::set<int> panel_in_wall_mask_needed;
    std::set<int> panel_in_plane_mask_needed;
};

#endif
