#ifndef Calib_GuideTube_Placement_hh
#define Calib_GuideTube_Placement_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElementPos.h"
#include "globals.hh"
#include <vector>

class Calib_GuideTube_Placement: public IDetElementPos,
                          public ToolBase {
public:

    G4bool hasNext();
    G4Transform3D next();

    Calib_GuideTube_Placement(const std::string& name);
    ~Calib_GuideTube_Placement();

private:
    void init();
    enum {GT_number = 39};

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;

    bool m_is_initialized;

    double m_offsetZ_in_cd;
    double m_offsetX_in_cd;
    double m_offsetY_in_cd;

    double m_phi;
};

#endif
