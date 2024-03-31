#include "PMTinPrototypePos.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include <cmath>
using namespace CLHEP;
namespace JUNO {
namespace Prototype {

// MCP 20 inch
MCPPMT20inchPos::MCPPMT20inchPos(double ball_r_MCP20inch) {
    double thetas[2] = {43.59*deg, 136.41*deg};
    //double thetas[2] = {45*deg, 135*deg};
    double phis[6] = {0};
    double x,y,z;
    for (int i = 0; i < 6; ++i) {
        phis[i] = 60 * i * deg;
    }

    for (int j = 0; j < 2 ; ++j) {
        for (int i = 0; i < 6; ++i) {
            double theta = thetas[j];
            double phi = phis[i];
            if((theta==43.59*deg&&i==1)||(theta==43.59*deg&&i==2)||(theta==43.59*deg&&i==4)||(theta==43.59*deg&&i==5)||(theta==136.41*deg&&i==0)||(theta==136.41*deg&&i==1)||(theta==136.41*deg&&i==3)||(theta==136.41*deg&&i==4)){
            //if((theta==45*deg&&i==1)||(theta==45*deg&&i==2)||(theta==45*deg&&i==4)||(theta==45*deg&&i==5)||(theta==135*     deg&&i==0)||(theta==135*deg&&i==1)||(theta==135*deg&&i==3)||(theta==135*deg&&i==4)){
            x = (ball_r_MCP20inch) * sin(theta) * cos(phi);
            y = (ball_r_MCP20inch) * sin(theta) * sin(phi);
            z = (ball_r_MCP20inch) * cos(theta);
            }
            else {continue;}
            G4ThreeVector pos(x, y, z);
            G4RotationMatrix rot;
            rot.rotateY(pi + theta);
            rot.rotateZ(phi);
            G4Transform3D trans(rot, pos);

            // Append trans
            m_position.push_back(trans);
        }
    }
    m_position_iter = m_position.begin();
}

G4bool MCPPMT20inchPos::hasNext() {
    return m_position_iter != m_position.end();
}

G4Transform3D MCPPMT20inchPos::next() {
    return *(m_position_iter++);
}

// Ham. 20 inch
HamPMT20inchPos::HamPMT20inchPos(double ball_r_Ham20inch) {
    double thetas[2] = {43.59*deg, 136.41*deg};
    //double thetas[2] = {45*deg, 135*deg};
    double phis[6] = {0};
    double x,y,z;
    for (int i = 0; i < 6; ++i) {
        phis[i] = 60 * i * deg;
    }

    for (int j = 0; j < 2 ; ++j) {
        for (int i = 0; i < 6; ++i) {
            double theta = thetas[j];
            double phi = phis[i];
            if((theta==43.59*deg&&i==0)||(theta==43.59*deg&&i==3)||(theta==136.41*deg&&i==2)||(theta==136.41*deg&&i==5)){
            //if((theta==45*deg&&i==0)||(theta==45*deg&&i==3)||(theta==135*deg&&i==2)||(theta==135*deg&&i==5)){
            x = (ball_r_Ham20inch) * sin(theta) * cos(phi);
            y = (ball_r_Ham20inch) * sin(theta) * sin(phi);
            z = (ball_r_Ham20inch) * cos(theta);
            }
            else {continue;}
            G4ThreeVector pos(x, y, z);
            G4RotationMatrix rot;
            rot.rotateY(pi + theta);
            rot.rotateZ(phi);
            G4Transform3D trans(rot, pos);

            // Append trans
            m_position.push_back(trans);
        }
    }
    m_position_iter = m_position.begin();
}

G4bool HamPMT20inchPos::hasNext() {
    return m_position_iter != m_position.end();
}
G4Transform3D HamPMT20inchPos::next() {
    return *(m_position_iter++);
}

// MCP  8 inch
MCPPMT8inchPos::MCPPMT8inchPos(double r, double pmt_r, double d) {
    int n = 16;
    double ball_r_MCP8inch = sqrt(r*r+d*d);    
    double thetas[2] = {76.62*deg, 103.38*deg};
    double phis[16] = {0};
    for (int i = 0; i < 16; ++i) {
        phis[i] = 360.*deg/n * i;
    }
    double x,y,z;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 16; ++j) {
        double theta = thetas[i];
        double phi = phis[j];
        if((theta==76.62*deg&&j==3)||(theta==76.62*deg&&j==5)||(theta==76.62*deg&&j==7)||(theta==76.62*deg&&j==9)||(theta==76.62*deg&&j==11)||(theta==76.62*deg&&j==13)||(theta==76.62*deg&&j==15)||(theta==103.38*deg&&j==1)||(theta==103.38*deg&&j==3)||(theta==103.38*deg&&j==5)||(theta==103.38*deg&&j==7)||(theta==103.38*deg&&j==9)||(theta==103.38*deg&&j==11)||(theta==103.38*deg&&j==13)||(theta==103.38*deg&&j==15)) {
            x = (ball_r_MCP8inch) * sin(theta) * cos(phi);
            y = (ball_r_MCP8inch) * sin(theta) * sin(phi);
            z = (ball_r_MCP8inch) * cos(theta);
         }
         else {continue;}
            G4ThreeVector pos(x, y, z);
            G4RotationMatrix rot;
            rot.rotateY(pi * 1.5);
            rot.rotateZ(phi);
            G4Transform3D trans(rot, pos);
            m_position.push_back(trans);
      }  
    }
    m_position_iter = m_position.begin();

}

G4bool MCPPMT8inchPos::hasNext() {
    return m_position_iter != m_position.end();
}

G4Transform3D MCPPMT8inchPos::next() {
    return *(m_position_iter++);
}

// Ham.  8 inch
HamPMT8inchPos::HamPMT8inchPos(double r, double pmt_r, double d) {
    int n = 16;
    double ball_r_Ham8inch = sqrt(r*r+d*d);
    double thetas[2] = {76.62*deg, 103.38*deg};
    double phis[16] = {0};
    for (int i = 0; i < 16; ++i) {
        phis[i] = 360.*deg/n * i;
    }
    double x,y,z;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 16; ++j) {
        double theta = thetas[i];
        double phi = phis[j];
        if((theta==76.62*deg&&j==2)||(theta==76.62*deg&&j==6)||(theta==76.62*deg&&j==10)||(theta==76.62*deg&&j==14)||(theta==103.38*deg&&j==2)||(theta==103.38*deg&&j==6)||(theta==103.38*deg&&j==10)||(theta==103.38*deg&&j==14)) {
            x = (ball_r_Ham8inch) * sin(theta) * cos(phi);
            y = (ball_r_Ham8inch) * sin(theta) * sin(phi);
            z = (ball_r_Ham8inch) * cos(theta);
         }
         else {continue;}
            G4ThreeVector pos(x, y, z);
            G4RotationMatrix rot;
            rot.rotateY(pi * 1.5);
            rot.rotateZ(phi);
            G4Transform3D trans(rot, pos);
            m_position.push_back(trans);
        }
    }
    m_position_iter = m_position.begin();

}

G4bool HamPMT8inchPos::hasNext() {
    return m_position_iter != m_position.end();
}

G4Transform3D HamPMT8inchPos::next() {
    return *(m_position_iter++);
}

// HZC  9 inch
HZCPMT9inchPos::HZCPMT9inchPos(double r, double pmt_r, double d) {
    int n = 16;
    double ball_r_HZC9inch = sqrt(r*r+d*d);
    double thetas[2] = {76.62*deg, 103.38*deg};
    double phis[16] = {0};
    for (int i = 0; i < 16; ++i) {
        phis[i] = 360.*deg/n * i;
    }
    double x,y,z;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 16; ++j) {
        double theta = thetas[i];
        double phi = phis[j];
        if((theta==76.62*deg&&j==0)||(theta==76.62*deg&&j==1)||(theta==76.62*deg&&j==4)||(theta==76.62*deg&&j==8)||(theta==76.62*deg&&j==12)||(theta==103.38*deg&&j==0)||(theta==103.38*deg&&j==4)||(theta==103.38*deg&&j==8)||(theta==103.38*deg&&j==12)) {
            x = (ball_r_HZC9inch) * sin(theta) * cos(phi);
            y = (ball_r_HZC9inch) * sin(theta) * sin(phi);
            z = (ball_r_HZC9inch) * cos(theta);
         }
         else {continue;}   
            G4ThreeVector pos(x, y, z);
            G4RotationMatrix rot;
            rot.rotateY(pi * 1.5);
            rot.rotateZ(phi);
            G4Transform3D trans(rot, pos);
            m_position.push_back(trans);
        }
    }
    m_position_iter = m_position.begin();

}

G4bool HZCPMT9inchPos::hasNext() {
    return m_position_iter != m_position.end();
}

G4Transform3D HZCPMT9inchPos::next() {
    return *(m_position_iter++);
}

//bottom 8inch MCP PMT
MCPPMT8inchPos_BTM::MCPPMT8inchPos_BTM(double r, double pmt_r) {
    double plane_ball_d = 220*mm;
    double phis[7]={30.*deg,90.*deg,150.*deg,210.*deg,270.*deg,330.*deg,0.*deg};
    double x,y,z;
        for (int j = 0; j < 7; j++) {
            if(phis[j]==90.*deg||phis[j]==270.*deg) {
            x = plane_ball_d * cos(phis[j]);
            y = plane_ball_d * sin(phis[j]);
            z = -r;
            }
            else if(phis[j]==0.*deg) {
             x = 0.;
             y = 0.;
             z = -r;
            }           
            else {continue;}
            G4ThreeVector pos(x, y, z);
            G4RotationMatrix rot;
           // rot.rotateY(pi * 1.5);
            rot.rotateZ(phis[j]);
            G4Transform3D trans(rot, pos);

            // Append trans
            m_position.push_back(trans);
        }
    m_position_iter = m_position.begin();

}
G4bool MCPPMT8inchPos_BTM::hasNext() {
    return m_position_iter != m_position.end();
}
G4Transform3D MCPPMT8inchPos_BTM::next() {
    return *(m_position_iter++);
}

//bottom 8inch Ham. PMT
HamPMT8inchPos_BTM::HamPMT8inchPos_BTM(double r, double pmt_r) {
    double plane_ball_d = 220*mm;
    double phis[6]={30.*deg,90.*deg,150.*deg,210.*deg,270.*deg,330.*deg};
    double x,y,z;
        for (int j = 0; j < 6; ++j) {
            if(phis[j]==150.*deg||phis[j]==330.*deg) {
            x = plane_ball_d * cos(phis[j]);
            y = plane_ball_d * sin(phis[j]);
            z = -r;
            }
            else {continue;}
            G4ThreeVector pos(x, y, z);
            G4RotationMatrix rot;
           // rot.rotateY(pi * 1.5);
            rot.rotateZ(phis[j]);
            G4Transform3D trans(rot, pos);

            // Append trans
            m_position.push_back(trans);
      }
    m_position_iter = m_position.begin();

}
G4bool HamPMT8inchPos_BTM::hasNext() {
    return m_position_iter != m_position.end();
}
G4Transform3D HamPMT8inchPos_BTM::next() {
    return *(m_position_iter++);
}

//bottom 9inch HZC PMT
HZCPMT9inchPos_BTM::HZCPMT9inchPos_BTM(double r, double pmt_r) {
    double plane_ball_d = 220*mm;
    double phis[6]={30.*deg,90.*deg,150.*deg,210.*deg,270.*deg,330.*deg};
    double x,y,z;
        for (int j = 0; j < 6; ++j) {
            if(phis[j]==30.*deg||phis[j]==210.*deg) {
            x = plane_ball_d * cos(phis[j]);
            y = plane_ball_d * sin(phis[j]);
            z = -r;
            }
            else {continue;}
            G4ThreeVector pos(x, y, z);
            G4RotationMatrix rot;
           // rot.rotateY(pi * 1.5);
            rot.rotateZ(phis[j]);
            G4Transform3D trans(rot, pos);

            // Append trans
            m_position.push_back(trans);
       }
    m_position_iter = m_position.begin();

}
G4bool HZCPMT9inchPos_BTM::hasNext() {
    return m_position_iter != m_position.end();
}
G4Transform3D HZCPMT9inchPos_BTM::next() {
    return *(m_position_iter++);
}

}
}
