#ifndef CalibTubePlacement_hh
#define CalibTubePlacement_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElementPos.h"
#include "globals.hh"
#include <vector>

class CalibTubePlacement: public IDetElementPos,
                          public ToolBase {
public:

    G4bool hasNext();
    G4Transform3D next();

    CalibTubePlacement(const std::string& name);
    ~CalibTubePlacement();

private:
    void init();

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;

    bool m_is_initialized;

    double m_offset_in_cd;
};

#endif
