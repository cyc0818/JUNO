#ifndef CalibSourceConstruction_hh
#define CalibSourceConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Tubs;
class G4Sphere;
class G4Ellipsoid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class CalibSourceConstruction: public IDetElement,
                             public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    CalibSourceConstruction(const std::string& name);
    ~CalibSourceConstruction();

private:
    void initMaterials();
    void initVariables();

    void makeCTLogical();

private:
	G4Sphere* solidWorld;
	G4LogicalVolume* logicWorld;

    G4Material* Acrylic;
    G4Material* LS;                                                               
    G4Material* SS;                                                               
    G4Material* Vacuum;                                                           
    G4Material* Mylar;  
    G4Material* Steel; 
    G4Material* Air;
    G4Material* Al;

   double m_SteelWeight_rad;
   double m_SteelWeight_height;
   double m_SteelWeight_distance;

   double m_Acrylic_rad;
   double m_AcrylicCylinder_height;
   double m_SteelPipe_height;
   double m_SteelPipe_rad ;
   double m_SteelPipe_thickness;
   double m_Al_rad ;
   double m_Air_rad;
};

#endif
