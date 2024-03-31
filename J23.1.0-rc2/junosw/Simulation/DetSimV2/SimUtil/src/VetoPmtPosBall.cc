
#include "VetoPmtPosBall.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/Task.h"
#include "Geometry/IPMTParamSvc.h"

#include <istream>
#include <fstream>
#include <sstream>
#include <cassert>
using namespace CLHEP;

namespace JUNO {

namespace Ball {

VetoPmtPosBall::VetoPmtPosBall(Task* scope)
    : m_scope(scope){
    initialize();
    m_position_iter = m_position.begin();
}

VetoPmtPosBall::~VetoPmtPosBall() {

}

G4bool 
VetoPmtPosBall::hasNext() {
    return m_position_iter != m_position.end();
}

G4Transform3D
VetoPmtPosBall::next() {
    return *(m_position_iter++);
}


void
VetoPmtPosBall::initialize() {
    SniperPtr<IPMTParamSvc> pmtParamSvc(*m_scope, "PMTParamSvc");
    if (pmtParamSvc.invalid())
    {
        LogError << "Can't Locate  PMTParamSvc." << std::endl;
        assert(0);
    }

    for (int i = kOFFSET_WP_PMT; i<kOFFSET_WP_PMT+pmtParamSvc->get_NTotal_WP_LPMT(); i++) // kOFFSET_WP_PMT=30000
    {
        G4int copyno; 
        G4double theta; // degree
        G4double phi; // degree
        G4double psi; // degree
        G4double x;   // x
        G4double y;   // y
        G4double z;   // z
        

        copyno = i;
        x = pmtParamSvc->getPMTX(i);
        y = pmtParamSvc->getPMTY(i);
        z = pmtParamSvc->getPMTZ(i);
        theta = pmtParamSvc->getPMTTheta(i);
        phi = pmtParamSvc->getPMTPhi(i);
        
        //std::cout <<"veto pmt number " <<copyno << " " << x << " " << y << " " << z <<" "<< theta<<" "<< phi << std::endl;
       
        theta = theta * deg;
        phi = phi * deg;
        psi = psi * deg;
        // Positions and rotations for regular PMT
        // construct G4Transform3D
        G4ThreeVector pos(x, y, z);
        G4RotationMatrix rot;
        rot.rotateY(theta);
        rot.rotateZ(phi);
        G4Transform3D trans(rot, pos);
        m_position.push_back(trans);
    }
}

// void
// VetoPmtPosBall::initialize() {
//     std::ifstream pmtsrc(m_filename.c_str());
//     std::string tmp_line;
//     G4int copyno; 
//     G4double theta; // degree
//     G4double phi; // degree
//     G4double psi; // degree
//     G4double x;   // x
//     G4double y;   // y
//     G4double z;   // z

//     // Positions and rotations for lpmt
//     if(lpmt_flag){
//     while (pmtsrc.good()) {
//         std::getline(pmtsrc, tmp_line);
//         if (pmtsrc.fail()) {
//             break;
//         }
//         std::stringstream ss;
//         ss << tmp_line;

//         ss >> copyno >> x >> y >> z >> theta >> phi;
//          std::cout <<"veto pmt number " <<copyno << " " << x << " " << y << " " << z <<" "<< theta<<" "<< phi << std::endl;
//        // std::cout<<std::endl;
//         if (ss.fail()) {
//             continue;
//         }

//         theta = theta * deg;
//         phi = phi * deg;
//         psi = psi * deg;
//         // Positions and rotations for regular PMT
//         // construct G4Transform3D
//         G4ThreeVector pos(x, y, z);
//         G4RotationMatrix rot;
//         rot.rotateY(theta);
//         rot.rotateZ(phi);
//         G4Transform3D trans(rot, pos);
//         m_position.push_back(trans);

//      }
//     }
//     else{
    
//     while (pmtsrc.good()) {
//         std::getline(pmtsrc, tmp_line);
//         if (pmtsrc.fail()) {
//             break;
//         }

//         std::stringstream ss;
//         ss << tmp_line;

//        ss >> copyno >> theta >> phi;
 //       std::cout <<"veto pmt number " <<copyno <<" "<< theta<<" "<< phi << std::endl; 
 //      if (ss.fail()) {
 //           continue;
 //       }

//         theta = theta * deg;
//         phi = phi * deg;
//         psi = psi * deg;

//         x = (m_ball_r) * sin(theta) * cos(phi);
//         y = (m_ball_r) * sin(theta) * sin(phi);
//         z = (m_ball_r) * cos(theta);
//         G4ThreeVector pos(x, y, z);
//         G4RotationMatrix rot;
//         rot.rotateY(theta);
//         rot.rotateZ(phi);
//         G4Transform3D trans(rot, pos);
//         m_position.push_back(trans);
//         }


        
    
    
//     }

//     pmtsrc.close();
// }

}

}
