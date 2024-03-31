#ifndef OnePMTPlacement_hh
#define OnePMTPlacement_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElementPos.h"
#include "globals.hh"
#include <vector>

class OnePMTPlacement: public IDetElementPos,
                       public ToolBase {
public:

    G4bool hasNext();
    G4Transform3D next();

    OnePMTPlacement(const std::string& name);
    ~OnePMTPlacement();

private:
    void init();

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;

    bool m_is_initialized;

    double x;
    double y;
    double z;

    double psi;
    double theta;
    double phi;
};

#endif
