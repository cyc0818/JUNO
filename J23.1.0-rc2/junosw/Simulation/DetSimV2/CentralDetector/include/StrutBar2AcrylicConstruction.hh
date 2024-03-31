#ifndef StrutBar2AcrylicConstruction_hh
#define StrutBar2AcrylicConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Box;
class G4Sphere;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class StrutBar2AcrylicConstruction : public IDetElement,
                            public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    virtual double geom_info(const std::string& /* param */);

    StrutBar2AcrylicConstruction(const std::string& name);
    ~StrutBar2AcrylicConstruction();

private:
    void initMaterials();
    void initVariables();

    // ++Strut 
    void makeStrutLogical();
    void makeStrutOpSurface();

private:
    
    G4Tubs*          solidStrut;
    G4LogicalVolume* logicStrut;

private:
    G4Material* Steel;           // Target material

private:
    double m_radStrut_in ;
    double m_radStrut_out;
    double m_lengthStrut;
    
    double gap;
    double strut_pos;
   
    G4double m_radLS ;
    G4double m_radAcrylic ;
    G4double m_anchorHeight;


private:
    // If m_useRealSurface is true, the PMT and struts should insert into the surface.
    // In order to optimize the speed, just reduce the size of the PMT.
    //
    //        |    -
    //        |  /   \
    //    ------- P M T
    //    ------- 19.434m
    //        |  \   /
    //        |    -
    //      Tyvek  |
    //     19.629m
    bool m_useRealSurface;
    bool m_useStrutSurface;

};

#endif
