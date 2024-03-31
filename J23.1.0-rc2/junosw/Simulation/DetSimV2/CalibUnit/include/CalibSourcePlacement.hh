#ifndef CalibSourcePlacement_hh
#define CalibSourcePlacement_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElementPos.h"
#include "globals.hh"
#include <vector>

class CalibSourcePlacement: public IDetElementPos,
                          public ToolBase {
public:

    G4bool hasNext();
    G4Transform3D next();

    CalibSourcePlacement(const std::string& name);
    ~CalibSourcePlacement();

private:
    void init();

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;

    bool m_is_initialized;

    double m_offsetZ_in_cd;
    double m_offsetX_in_cd;
    double m_offsetY_in_cd;
};

#endif
