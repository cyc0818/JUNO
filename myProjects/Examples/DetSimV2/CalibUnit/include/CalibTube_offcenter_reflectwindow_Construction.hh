#ifndef CalibTube_offcenter_reflectwindow_Construction_hh
#define CalibTube_offcenter_reflectwindow_Construction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Tubs;
class G4Ellipsoid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;


class CalibTube_offcenter_reflectwindow_Construction: public IDetElement,
                             public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    CalibTube_offcenter_reflectwindow_Construction(const std::string& name);
    ~CalibTube_offcenter_reflectwindow_Construction();

private:
    void initMaterials();
    void initVariables();

    void makeCTLogical();

private:
    G4Tubs*    solidCT;
    G4LogicalVolume*    logicCT;

    G4Tubs*    solidTube1;
    G4LogicalVolume* logicTube1;
    G4VPhysicalVolume* physiTube1;
    
    G4Tubs*    vacTube1;
    G4LogicalVolume* logicvacTube1;
    G4VPhysicalVolume* physivacTube1;
  
    G4Tubs*    solidTube2;
    G4LogicalVolume* logicTube2;
    G4VPhysicalVolume* physiTube2;
    
    G4Tubs*    vacTube2;
    G4LogicalVolume* logicvacTube2;
    G4VPhysicalVolume* physivacTube2;
    
    G4Ellipsoid*   solidWindow;
    //G4Tubs*   solidWindow;
    G4LogicalVolume* logicWindow;
    G4VPhysicalVolume* physiWindow;
    
    G4Ellipsoid*   LSWindow;
    G4LogicalVolume* logicLSWindow;
    G4VPhysicalVolume* physiLSWindow;

  


    G4Material* Acrylic;
    G4Material* LS;                                                               
    G4Material* SS;                                                               
    G4Material* Vacuum;                                                           
    G4Material* Mylar;  

    double m_calibTubeLength1; // SS section                                      
    double m_calibTubeLength2; // acrylic section                                 
    double m_calibTubeOuterRadius;                                                
    double m_calibTubeThickness;                                                  
                                                                                  
    double m_calibWindowDistortionHeight; // window (ellipsoid shape and cut)     
    double m_calibWindowThickness; //      
};

#endif
