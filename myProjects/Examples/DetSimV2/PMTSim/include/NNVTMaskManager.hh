#ifndef NNVTMaskManager_hh
#define NNVTMaskManager_hh

/*
 * This mask is designed for NNVT MCT-PMT.
 */

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IPMTElement.h"

#include "globals.hh"

class G4VSolid;
class G4Box;
class G4Sphere;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class NNVTMaskManager : public IPMTElement,
                            public ToolBase{
public:
    G4LogicalVolume* getLV();
    G4double GetPMTRadius();
    G4double GetPMTHeight();
    G4double GetZEquator();
    G4ThreeVector GetPosInPMT();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    NNVTMaskManager(const std::string& name);
    ~NNVTMaskManager();

private:

    void initMaterials();
    void initVariables();

    // + WP: Water Pool
    void makeMaskOutLogical();

    void makeMaskLogical();
    void makeMaskPhysical();
    
private:
    
    G4VSolid*           Top_Virtual ;   
    G4VSolid*           Bottom_Virtual; 
    G4VSolid*           Mask_Virtual;   
    G4VSolid*           SolidMaskVirtual;   
    G4LogicalVolume*    logicMaskVirtual;
   
    G4VSolid*           Top_out ;   
    G4VSolid*           Bottom_out;  
    G4VSolid*           Mask_out;   
    
    G4VSolid*           Top_in ;   
    G4VSolid*           Bottom_in;   
    G4VSolid*           Mask_in;   
    G4VSolid*           solidMask;   
    
    G4LogicalVolume*    logicMask;   
    G4VPhysicalVolume*  physiMask;   

private:
    G4Material* LAB;           
    G4Material* Acrylic;         
    G4Material* Water;
private:
    double mask_radiu_in;
    double mask_radiu_out;
    double mask_radiu_virtual;
    double htop_in;
    double htop_out;
    double htop_thickness;
    double htop_gap;
    double requator_thickness;
    double requator_gap;
    double height_virtual;
    double height_in;
    double height_out;
    double gap;

    double MAGIC_virtual_thickness;

    std::string m_buffer_material;

    IPMTElement* inner_pmt;
};

#endif
