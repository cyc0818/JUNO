#ifndef RandomPositionTubeGen_hh
#define RandomPositionTubeGen_hh

namespace DYB2 {

namespace Tube {

class RandomPositionTubeGen : public IVector3dGen {
public:
    RandomPositionTubeGen(double tube_r, double tube_h);
    void setSeed(long);
    G4ThreeVector next();

private:
    IVector3dGen* m_gen;
    double m_tube_r;
    double m_tube_h;

private:
    bool isOk(G4ThreeVector pos);

};

}

}

#endif
