#ifndef GlobalGeomInfo_hh
#define GlobalGeomInfo_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class GlobalGeomInfo: public IDetElement,
                      public ToolBase {
public:
    G4LogicalVolume* getLV() {return 0;};
    bool inject(std::string /* motherName */, IDetElement* /* other */, IDetElementPos* /* pos */) {return false;}

    // FIXME: 
    // how the user check the info is right or not???
    double geom_info(const std::string& /* param */ );
    double geom_info(const std::string& /* param */ , int /* idx */);

    GlobalGeomInfo(const std::string& /* name */);
    ~GlobalGeomInfo();

private:
    void init_variables();

private:
    // = World =
    // == Experiment Hall ==
    double m_expHallXLen;
    double m_expHallXLen_gap;
    double m_expHallXLen_tot;
    double m_expHallYLen;
    double m_expHallZLen;
    double m_expHallZLen_tot;
    double m_expHall_domeHeight;
    double m_expHall_domeRadius;
    double m_expHallXmax;
    double m_expHallYmax;
    double m_expHallZmax;
    double m_expHallXmin;
    double m_expHallYmin;
    double m_expHallZmin;

    // == Water Pool ==
    double m_waterPoolRadius;
    double m_waterPoolHeight;
    double m_waterPoolConcreteHeight;
    double m_poolLiningThickness;
    //Rock
    double m_rockThickness;
    double m_topRockXLen;
    double m_topRockYLen;
    double m_topRockZLen;
    double m_topRockZLen_tot;
    double m_topRock_domeHeight;
    double m_topRock_domeRadius;
    double m_btmRockHeight;
    double m_btmRockRadius;
    // == Central Detector ==

    // == material ==
    double m_LS_abslen_at430;
    double m_LS_raylen_at430;
    double m_LS_light_yield;

    // = load from other service =
    bool m_use_param_svc;
};

#endif
