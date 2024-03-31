#ifndef testLocalToGlobal_hh
#define testLocalToGlobal_hh

#include "G4AffineTransform.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

class TestLocalToGlobal {
public:
    TestLocalToGlobal();

    void pointLTG();
    void vectorLTG();
private:

    void quichshow(G4AffineTransform*, G4ThreeVector&);

    // test 1:
    // only with rotation
    G4AffineTransform* m_at_1;
    G4RotationMatrix* m_rm_1;

    // test 2:
    // only with translation
    G4AffineTransform* m_at_2;
    G4ThreeVector* m_tv_2;

    // test 3:
    // rotation first, then translate
    G4AffineTransform* m_at_3;
    G4RotationMatrix* m_rm_3;
    G4ThreeVector* m_tv_3;

};

#endif
