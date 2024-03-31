#ifndef ChimneyPlacement_hh
#define ChimneyPlacement_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElementPos.h"
#include "globals.hh"
#include <vector>

class UpperChimneyPlacement: public IDetElementPos,
                          public ToolBase {
public:

    G4bool hasNext();
    G4Transform3D next();

    UpperChimneyPlacement(const std::string& name);
    ~UpperChimneyPlacement();

private:
    void init();

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;

    bool m_is_initialized;

    double m_TopToWater;
};

#endif
