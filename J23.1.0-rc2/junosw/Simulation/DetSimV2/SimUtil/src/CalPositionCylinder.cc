#include "CalPositionCylinder.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include <cassert>
using namespace CLHEP;
namespace JUNO {
namespace Cylinder {

CalPositionCylinder::CalPositionCylinder(G4double cyl_r,
                                 G4double cyl_h,
                                 G4double pmt_r,
                                 G4double pmt_h,
                                 G4double pmt_interval) 
{
    m_cyl_r = cyl_r;
    m_cyl_h = cyl_h;
    m_pmt_r = pmt_r;
    m_pmt_h = pmt_h;
    m_pmt_interval = pmt_interval;

    initialize();
    calculate();

    m_position_iter = m_position.begin();
}

CalPositionCylinder::~CalPositionCylinder() {

}

void CalPositionCylinder::initialize() {
  /*G4int n_max =*/ GetMaxiumNinCircle(
                                    m_cyl_r,
                                    m_pmt_interval);
  assert(m_pmt_interval > 2 * m_pmt_r);

}

// Helper:
G4int CalPositionCylinder::GetMaxiumNinCircle(G4double r_circle,
    G4double interval)
{
  assert(r_circle >= 0);//keep r_circle >= 0
  G4int N = 0;
  N = int(r_circle*2*pi/interval);
  if(r_circle == 0) N = 1;
  return N;
}

G4Transform3D CalPositionCylinder::next() {
    return *(m_position_iter++);
}

void CalPositionCylinder::calculate() {
  G4int n_pmt = 0;
 // pmts on ball surface
  double ball_r = 20.6*m;
  double m_ball_circle_r = ball_r;
  double pmt_interval  = 1.66*m;
  double delta_theta = pmt_interval/m_ball_circle_r;
  /////////////////// //ball pmts////////////////
  G4int n_ball_ring = 0;
  G4int n_ball_pmt = 0;
  double regionA_theta[1]  ={2.36};
  double regionA_phi[1]    ={18};
  double regionB_theta[5]  ={5.27, 7.47, 7.47, 9.85,9.85};
  double regionB_phi[5]    ={18,   26.52,9.88, 24.64, 12.18};
  double regionC_theta[11] ={90-75.47, 90-70.54, 90-68.86,90-62.97,90-59.28,90-55.39,90-51.69,
                             90-47.7,90-47.7,90-43.8,90-43.8};
  double regionC_phi[11]   ={6,6,6,6,6,6,6, 
                             8.86,3.3, 8.86, 3.3};
  double regionD_theta[26] ={38.7,38.7, 35.79,35.79, 31.58,31.58, 27.47,27.47,23.15,23.15, 19.04,19.04,
                             14.72,14.72, 10.61,10.61,6.26,6.26, 6.26,2.16,2.16,2.16,-2.16,-2.16,-2.16,-2.16};
  double regionD_phi[26]   ={8.86,3.3,8.86,3.3,8.86,3.3,8.86,3.3,8.86,3.3,8.86,3.3,8.86,3.3,8.86,3.3,
                             8.86, 6, 2.13, 8.86, 6, 2.13,10.39,7.46,4.53,1.61};

  double regionE_theta[28] ={ 96.26,96.26,96.26,96.26,100.61,100.61,100.61,104.72,104.72,104.72,
                             109.04,109.04,109.04,113.15,113.15,113.15,117.47,117.47,117.47,121.68,121.68,121.68,
                             125.79,125.79,125.79,129.70,129.70,129.70};
  double regionE_phi[28]   ={10.39,7.46,4.53,1.61, 9.85,6.0,2.15,9.85,6.0,2.15,
                          9.85,6.0,2.15,9.85,6.0,2.15, 9.85,6.0,2.15,9.85,6.0,2.15, 9.85,6.0,2.15,9.85,6.0,2.15};
  double regionF_theta[11] ={133.8,133.8,137.7,137.7,
                             141.69,145.39,149.28,152.97,158.86,160.54,165.47};
  double regionF_phi[11]   ={8.86,3.3, 8.86, 3.3,
                             6.0,6.0,6.0,6.0,6.0,6.0,6.0};

  double regionG_theta[6]  ={180-9.85, 180-9.85,180-9.85, 180-7.47,180-7.47,180-5.27 };
  double regionG_phi[6]    ={27.57,18.0,8.48,26.52,9.98,18.0 };
  double regionH_theta[1]  ={180-2.36};
  double regionH_phi[1]    ={18};


//////////////////////////////////////////////////////////////////////
 double mb_r;
 for(int iphi=0;iphi<360;iphi=iphi+12){
  if(iphi%72==0) {
      G4double phi = (regionA_phi[0]+iphi)*3.1415926/180;
      G4double d_theta = regionA_theta[0]*3.1415926/180;
      mb_r = m_ball_circle_r*sin(d_theta);
      G4double x = mb_r * cos(phi);
      G4double y = mb_r * sin(phi);
      G4double z = m_ball_circle_r*cos(d_theta);
      //G4cout<<" veto pool pmt bottom position xyz: ("<<x<<", "<<y<<", "<<z<<")"<<G4endl;  

      G4ThreeVector pos(x, y, z);
      G4RotationMatrix rot;
      rot.rotateY(d_theta);
      rot.rotateZ(phi);
      G4Transform3D trans(rot, pos);

      m_position.push_back(trans);
      n_ball_pmt++;
      n_pmt++;
  } 
  if(iphi%36==0) {
   for(int i=0;i<5;i++) {
      G4double phi = (regionB_phi[i]+iphi)*3.1415926/180;
      G4double d_theta = regionB_theta[i]*3.1415926/180;
      mb_r = m_ball_circle_r*sin(d_theta);
      G4double x = mb_r * cos(phi);
      G4double y = mb_r * sin(phi);
      G4double z = m_ball_circle_r*cos(d_theta);
    //  G4cout<<" veto pool pmt bottom position xyz: ("<<x<<", "<<y<<", "<<z<<")"<<G4endl;

      G4ThreeVector pos(x, y, z);
      G4RotationMatrix rot;
      rot.rotateY(d_theta);
      rot.rotateZ(phi);
      G4Transform3D trans(rot, pos);

      m_position.push_back(trans);
      n_ball_pmt++;
      n_pmt++; 
   }//i  
  }//if iphi
   for(int i=0;i<11;i++) {
      G4double phi = (regionC_phi[i]+iphi)*3.1415926/180;
      G4double d_theta = regionC_theta[i]*3.1415926/180;
      mb_r = m_ball_circle_r*sin(d_theta);
      G4double x = mb_r * cos(phi);
      G4double y = mb_r * sin(phi);
      G4double z = m_ball_circle_r*cos(d_theta);
  //    G4cout<<" veto pool pmt bottom position xyz: ("<<x<<", "<<y<<", "<<z<<")"<<G4endl;

      G4ThreeVector pos(x, y, z);
      G4RotationMatrix rot;
      rot.rotateY(d_theta);
      rot.rotateZ(phi);
      G4Transform3D trans(rot, pos);

      m_position.push_back(trans);
      n_ball_pmt++;
      n_pmt++;
   }//i 
   for(int i=0;i<26;i++) {
      G4double phi = (regionD_phi[i]+iphi)*3.1415926/180;
      G4double d_theta = (90-regionD_theta[i])*3.1415926/180;
      mb_r = m_ball_circle_r*sin(d_theta);
      G4double x = mb_r * cos(phi);
      G4double y = mb_r * sin(phi);
      G4double z = m_ball_circle_r*cos(d_theta);
    //  G4cout<<" veto pool pmt bottom position xyz: ("<<x<<", "<<y<<", "<<z<<")"<<G4endl;

      G4ThreeVector pos(x, y, z);
      G4RotationMatrix rot;
      rot.rotateY(d_theta);
      rot.rotateZ(phi);
      G4Transform3D trans(rot, pos);

      m_position.push_back(trans);
      n_ball_pmt++;
      n_pmt++;
   }//i  
  }//for iphi
 G4cout<<" veto pool top half on ball surface: "<<n_ball_pmt<<G4endl;

// for bottom half---------------------------------------
 for(int iphi=0;iphi<360;iphi=iphi+12){
   for(int i=0;i<28;i++) {
      G4double phi = (regionE_phi[i]+iphi)*3.1415926/180;
      G4double d_theta = regionE_theta[i]*3.1415926/180;
      mb_r = m_ball_circle_r*sin(d_theta);
      G4double x = mb_r * cos(phi);
      G4double y = mb_r * sin(phi);
      G4double z = m_ball_circle_r*cos(d_theta);
 //     G4cout<<" veto pool pmt bottom position xyz: ("<<x<<", "<<y<<", "<<z<<")"<<G4endl;

      G4ThreeVector pos(x, y, z);
      G4RotationMatrix rot;
      rot.rotateY(d_theta);
      rot.rotateZ(phi);
      G4Transform3D trans(rot, pos);

      m_position.push_back(trans);
      n_ball_pmt++;
      n_pmt++;
   }//i  
   for(int i=0;i<11;i++) {
      G4double phi = (regionF_phi[i]+iphi)*3.1415926/180;
      G4double d_theta = regionF_theta[i]*3.1415926/180;
      mb_r = m_ball_circle_r*sin(d_theta);
      G4double x = mb_r * cos(phi);
      G4double y = mb_r * sin(phi);
      G4double z = m_ball_circle_r*cos(d_theta);
//      G4cout<<" veto pool pmt bottom position xyz: ("<<x<<", "<<y<<", "<<z<<")"<<G4endl;
      G4ThreeVector pos(x, y, z);
      G4RotationMatrix rot;
      rot.rotateY(d_theta);
      rot.rotateZ(phi);
      G4Transform3D trans(rot, pos);
      m_position.push_back(trans);
      n_ball_pmt++;
      n_pmt++;
   }//i  
if(iphi%36==0) {
   for(int i=0;i<6;i++) {
      G4double phi = (regionG_phi[i]+iphi)*3.1415926/180;
      G4double d_theta = regionG_theta[i]*3.1415926/180;
      mb_r = m_ball_circle_r*sin(d_theta);
      G4double x = mb_r * cos(phi);
      G4double y = mb_r * sin(phi);
      G4double z = m_ball_circle_r*cos(d_theta);
  //    G4cout<<" veto pool pmt bottom position xyz: ("<<x<<", "<<y<<", "<<z<<")"<<G4endl;
      G4ThreeVector pos(x, y, z);
      G4RotationMatrix rot;
      rot.rotateY(d_theta);
      rot.rotateZ(phi);
      G4Transform3D trans(rot, pos);
      m_position.push_back(trans);
      n_ball_pmt++;
      n_pmt++;
   }//i
 }  
 if(iphi%72==0) { 
      G4double phi = (regionH_phi[0]+iphi)*3.1415926/180;
      G4double d_theta = regionH_theta[0]*3.1415926/180;
      mb_r = m_ball_circle_r*sin(d_theta);
      G4double x = mb_r * cos(phi);
      G4double y = mb_r * sin(phi);
      G4double z = m_ball_circle_r*cos(d_theta);
    //  G4cout<<" veto pool pmt bottom position xyz: ("<<x<<", "<<y<<", "<<z<<")"<<G4endl;
      G4ThreeVector pos(x, y, z);
      G4RotationMatrix rot;
      rot.rotateY(d_theta);
      rot.rotateZ(phi);
      G4Transform3D trans(rot, pos);
      m_position.push_back(trans);
      n_ball_pmt++;
      n_pmt++;
    }
   
  }//for iphi
  G4cout<<"total veto pool pmt : "<<n_pmt<<G4endl;
}
G4bool CalPositionCylinder::hasNext() {
    return m_position_iter != m_position.end();
}


}
}
