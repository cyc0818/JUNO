#include <iostream>
#include "testLocalToGlobal.hh"
#include "globals.hh"

TestLocalToGlobal::TestLocalToGlobal() {
    // test 1:
    m_rm_1 = new G4RotationMatrix();
    m_rm_1->rotateZ(45*deg);
    m_at_1 = new G4AffineTransform(*m_rm_1);

    // test 2:
    m_tv_2 = new G4ThreeVector();
    m_tv_2->setX(2.0);
    m_at_2 = new G4AffineTransform(*m_tv_2);

    // test 3:
    m_rm_3 = new G4RotationMatrix();
    m_rm_3->rotateZ(45*deg);
    m_tv_3 = new G4ThreeVector();
    m_tv_3->setX(2.0);
    m_at_3 = new G4AffineTransform(*m_rm_3, *m_tv_3);
}

void
TestLocalToGlobal::pointLTG() {

}

void
TestLocalToGlobal::vectorLTG() {
    // test 1:
    G4ThreeVector v(0, 1, 0);
    quichshow(m_at_1, v);

    // test 2:
    G4ThreeVector v_2(0, 1, 0);
    quichshow(m_at_2, v_2);

    // test 3:
    G4ThreeVector v_3(0, 1, 0);
    quichshow(m_at_3, v_3);
}

void
TestLocalToGlobal::quichshow(G4AffineTransform* at, G4ThreeVector& v) {
    std::cout << v.x() << ", " << v.y() << ", " << v.z() << std::endl;
    at->ApplyAxisTransform(v);
    std::cout << v.x() << ", " << v.y() << ", " << v.z() << std::endl;
    at->Inverse().ApplyAxisTransform(v);
    std::cout << v.x() << ", " << v.y() << ", " << v.z() << std::endl;
}
