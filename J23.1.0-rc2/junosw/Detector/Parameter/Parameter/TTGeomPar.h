#ifndef JUNO_TT_GEOM_PAR_H
#define JUNO_TT_GEOM_PAR_H

#include <iostream>

class TTGeomPar
{
    public:

	TTGeomPar() { std::cout << "TTGeomPar constructor" << std::endl; }
	~TTGeomPar() { std::cout << "TTGeomPar destructor" << std::endl; }

        // Simulation/DetSimV2/TopTracker/src/TopTrackerConstruction.cc
        // --------------------------------------------------------------
        double GetBoxX() { return m_box_x; }
        double GetBoxY() { return m_box_y; }
        double GetBoxZ() { return m_box_z; }
        double GetChimneyR() { return m_chimney_R; }
        double GetChimneyZ() { return m_chimney_Z; }
        double GetLengthBar() { return m_lengthBar; }
        double GetThicknessBar() { return m_thicknessBar; }
        double GetWidthBar() { return m_widthBar; }
        double GetSpaceBar() { return m_spaceBar; }
        double GetLengthCoating() { return m_lengthCoating; }
        double GetThickCoating() { return m_thicknessCoating; }
        double GetWidthCoating() { return m_widthCoating; }
        double GetLengthModuleTape() { return m_lengthModuleTape; }
        double GetThickModuleTape() { return m_thicknessModuleTape; }
        double GetWidthModuleTape() { return m_widthModuleTape; }
        double GetLengthModule() { return m_lengthModule; }
        double GetThickModule() { return m_thicknessModule; }
        double GetWigthModule() { return m_widthModule; }
        double GetSpaceModule() { return m_spaceModule; }
        double GetLengthPlane() { return m_lengthPlane; }
        double GetThickPlane() { return m_thicknessPlane; }
        double GetWidthPlane() { return m_widthPlane; }
        double GetVSpacePlane() { return m_vspacePlane; }
        double GetWallX() { return m_wall_x; }
        double GetWallY() { return m_wall_y; }
        double GetWallZ() { return m_wall_z; }
        double GetZLowerWall() { return m_zlowerwall; }
        double GetZShift() { return m_zshift; }
        double GetZSpace() { return m_zspace; }
        double GetZSpaceChimney() { return m_zspaceChimney; }
        double GetXSpace() { return m_xspace; }
        double GetXSpace_1() { return m_xspace1; }
        double GetYSpace() { return m_yspace; }
        double GetXHole() { return m_xhole; }
        double GetZLowerWallChim() { return m_zlowerwallChimney; }

        void InitTTVariables();

    private:

        // Simulation/DetSimV2/TopTracker/src/TopTrackerConstruction.cc
        // ------------------------------------------------------------
        double m_box_x;
        double m_box_y;
        double m_box_z;

        double m_chimney_R;
        double m_chimney_Z;

        double m_wall_x;
        double m_wall_y;
        double m_wall_z;

        double m_lengthBar;
        double m_thicknessBar;
        double m_widthBar;
        double m_spaceBar;

        double m_lengthCoating;
        double m_thicknessCoating;
        double m_widthCoating;

        double m_lengthModuleTape;
        double m_thicknessModuleTape;
        double m_widthModuleTape;

        double m_lengthModule;
        double m_thicknessModule;
        double m_widthModule;
        double m_spaceModule;

        double m_lengthPlane;
        double m_thicknessPlane;
        double m_widthPlane;
        double m_vspacePlane;

        //wall positions
        double m_zlowerwall;
        double m_zshift;//to access module and space for PMTs
        double m_zspace;//distance between wall layers
        double m_zspaceChimney;//distance between walls above chimney
        double m_xspace;//distance between walls in x
        double m_xspace1;//distance between walls in x central row
        double m_yspace;//distance between walls in y
        double m_xhole;//hole for chimney in x direction
        double m_zshiftMR;//to access module and space for PMTs in middle row with respect to the other rowa
        double m_zlowerwallChimney;

        void SetBoxX(double box_x) { m_box_x = box_x; }
        void SetBoxY(double box_y) { m_box_y = box_y; }
        void SetBoxZ(double box_z) { m_box_z = box_z; }
        void SetChimneyR(double chimney_R) { m_chimney_R = chimney_R; }
        void SetChimneyZ(double chimney_Z) { m_chimney_Z = chimney_Z; }
        void SetLengthBar(double lengthBar) { m_lengthBar = lengthBar; }
        void SetThicknessBar(double thicknessBar) { m_thicknessBar = thicknessBar; }
        void SetWidthBar(double widthBar) { m_widthBar = widthBar; }
        void SetSpaceBar(double spaceBar) { m_spaceBar = spaceBar; }
        void SetLengthCoating(double lengthCoating) { m_lengthCoating = lengthCoating; }
        void SetThickCoating(double thicknessCoating) { m_thicknessCoating = thicknessCoating; }
        void SetWidthCoating(double widthCoating) { m_widthCoating = widthCoating; }
        void SetLengthModuleTape(double lengthModuleTape) { m_lengthModuleTape = lengthModuleTape; }
        void SetThickModuleTape(double thicknessModuleTape) { m_thicknessModuleTape = thicknessModuleTape; }
        void SetWidthModuleTape(double widthModuleTape) { m_widthModuleTape = widthModuleTape; }
        void SetLengthModule(double lengthModule) { m_lengthModule = lengthModule; }
        void SetThickModule(double thicknessModule) { m_thicknessModule = thicknessModule; }
        void SetWidthModule(double widthModule) { m_widthModule = widthModule; }
        void SetSpaceModule(double spaceModule) { m_spaceModule = spaceModule; }
        void SetLengthPlane(double lengthPlane) { m_lengthPlane = lengthPlane; }
        void SetThickPlane(double thicknessPlane) { m_thicknessPlane = thicknessPlane; }
        void SetWidthPlane(double widthPlane) { m_widthPlane = widthPlane; }
        void SetVSpacePlane(double vspacePlane) { m_vspacePlane = vspacePlane; }
        void SetWallX(double wall_x) { m_wall_x = wall_x; }
        void SetWallY(double wall_y) { m_wall_y = wall_y; }
        void SetWallZ(double wall_z) { m_wall_z = wall_z; }
        void SetZLowerWall(double zlowerwall) { m_zlowerwall = zlowerwall; }
        void SetZShift(double zshift) { m_zshift = zshift; }
        void SetZSpace(double zspace) { m_zspace = zspace; }
        void SetZSpaceChimney(double zspaceChimney) { m_zspaceChimney = zspaceChimney; }
        void SetXSpace(double xspace) { m_xspace = xspace; }
        void SetXSpace_1(double xspace1) { m_xspace1 = xspace1; }
        void SetYSpace(double yspace) { m_yspace = yspace; }
        void SetXHole(double xhole) { m_xhole = xhole; }
        void SetZLowerWallChimney(double zlowerwallChimney) { m_zlowerwallChimney = zlowerwallChimney; }

};

#endif
