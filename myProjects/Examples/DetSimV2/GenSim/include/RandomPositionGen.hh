
#ifndef RandomPositionGen_hh
#define RandomPositionGen_hh

#include "DYB2PositionGenInterface.hh"

namespace DYB2 {

class RandomPositionGen :public IVector3dGen{
public:
    RandomPositionGen(double x_low, 
                      double x_up,
                      double y_low,
                      double y_up,
                      double z_low,
                      double z_up);
    void setSeed(long);

    G4ThreeVector next();

private:
    double m_x_low; 
    double m_x_up;
    double m_y_low;
    double m_y_up;
    double m_z_low;
    double m_z_up;
};

}

#endif
