#ifndef XJanchorConstruction_hh
#define XJanchorConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Tubs;
class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class XJanchorConstruction : public IDetElement,
			public ToolBase{

public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);
    virtual double geom_info(const std::string& /* param */);

    XJanchorConstruction(const std::string& name);
    ~XJanchorConstruction();

private:
   void initMaterials();
   void initVariables();

   void makeXJanchorLogical();

private:
    G4VSolid* m_solidXJanchor_up;
    G4VSolid* m_solidXJanchor_down;
    G4VSolid* m_solidXJanchor_ball;

    G4LogicalVolume* m_logicXJanchor;

private:
    G4Material* m_Acrylic;
    G4double m_radLS;
    G4double m_radAcrylic;
    G4double m_xjanchor_pos;
private:
    bool m_simplify_calib_anchor;

};

#endif
