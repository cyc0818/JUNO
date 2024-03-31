#ifndef JUNO_CD_GEOM_PAR_H
#define JUNO_CD_GEOM_PAR_H

#include <iostream>

class CdGeomPar
{
    public:

	CdGeomPar() { std::cout << "CdGeomPar constructor" << std::endl; }
	~CdGeomPar() { std::cout << "CdGeomPar destructor" << std::endl; }

	void InitCdVariables();

        // Simulation/DetSimV2/CentralDetector/src/DetSim1Construction.cc
        // --------------------------------------------------------------
        double GetRadLS() { return m_radLS; }
        double GetThickAcrylic() { return m_thicknessAcrylic; }
        double GetRadLSChim() { return m_radLSChimney; }
        double GetRadInnerWater() { return m_radInnerWater; }
        double GetThickReflector() { return m_thicknessReflector; }
        double GetChimWaterHeight() { return m_heightWaterChimney; }

        void InitCDDetSim1Construction();

        // Simulation/DetSimV2/CentralDetector/src/FastenerAcrylicConstruction.cc
        // ---------------------------------------------------------------------
        double GetFastenersUpOutR() { return fasteners_up_out_R; }
        double GetFastenersLengthUp() { return fasteners_length_up; }
        double GetFastenersUp1InR() { return fasteners_up1_in_R; }
        double GetFastenersUp1OutR() { return fasteners_up1_out_R; }
        double GetFastenersLengthUp1() { return fasteners_length_up1; }
        double GetFastenersDownInR() { return fasteners_down_in_R; }
        double GetastenersDownOutR() { return fasteners_down_out_R; }
        double GetFastenersLengthDown() { return fasteners_length_down; }
        double GetFastenersBoltsR() { return fasteners_bolts_R; }
        double GetFastenersBoltsLength() { return fasteners_bolts_length; }

        void InitCDFastenerAcrylicConstruction();

        //Simulation/DetSimV2/CentralDetector/src/PrototypeConstruction.cc
        //------------------------------------------------------------------
        double GetSteelTubeHeight() { return m_steel_tube_height; }
        double GetSteelTubeRadius() { return m_steel_tube_radius; }
        double GetSteelTubeThickness() { return m_steel_tube_thickness; }
        double GetTargetRadius() { return m_target_radius; }
        double GetTargetChimneyRadius() { return m_target_chimney_radius; }
        double GetAcrylicBallThickness() { return m_acrylic_ball_thickness; }
        double GetAcrylicChimneyRadius() { return m_acrylic_chimney_radius; }
        double GetAcrylicCircleRadius() { return m_acrylic_circle_radius; }
        double GetAcrylicCircleHeight() { return m_acrylic_circle_height; }

        void InitCDPrototypeConstruction();

        // Simulation/DetSimV2/CentralDetector/src/StrutAcrylicConstruction.cc
        // -------------------------------------------------------------------
        double GetRadStrutIn() { return m_radStrut_in; }
        double GetRadStrutOut() { return m_radStrut_out; }
        double GetLengthStrut() { return m_lengthStrut; }
        double GetGap() { return gap; }

        void InitCDStrutAcrylicConstruction();

    private:

        // Simulation/DetSimV2/CentralDetector/src/DetSim1Construction.cc
        // ---------------------------------------------------------------
        double m_radLS;
        double m_thicknessAcrylic;
        double m_radLSChimney;
        double m_radInnerWater;
        double m_thicknessReflector;
        double m_heightWaterChimney;

        void SetRadLS(double radLS) { m_radLS = radLS; }
        void SetThickAcrylic(double thicknessAcrylic) { m_thicknessAcrylic = thicknessAcrylic; }
        void SetRadLSChimney(double radLSChimney) { m_radLSChimney = radLSChimney; }
        void SetRadInnerWater(double radInnerWater) { m_radInnerWater = radInnerWater; }
        void SetThickReflector(double thicknessReflector) { m_thicknessReflector = thicknessReflector; }
        void SetChimWaterHeight(double heightWaterChimney) { heightWaterChimney = m_heightWaterChimney; }

        // Simulation/DetSimV2/CentralDetector/src/FastenerAcrylicConstruction.cc
        // ----------------------------------------------------------------------
        double fasteners_up_out_R;
        double fasteners_length_up;
        double fasteners_up1_in_R;
        double fasteners_up1_out_R;
        double fasteners_length_up1;
        double fasteners_down_in_R;
        double fasteners_down_out_R;
        double fasteners_length_down;
        double fasteners_bolts_R;
        double fasteners_bolts_length;

        void SetFastenersUpOutR(double up_out_R) { fasteners_up_out_R = up_out_R; }
        void SetFastenersLengthUp(double length_up) { fasteners_length_up = length_up; }
        void SetFastenersUp1InR(double up1_in_R) { fasteners_up1_in_R = up1_in_R; }
        void SetFastenersUp1OutR(double up1_out_R) { fasteners_up1_out_R = up1_out_R; }
        void SetFastenersLengthUp1(double length_up1) { fasteners_length_up1 = length_up1; }
        void SetFastenersDownInR(double down_in_R) { fasteners_down_in_R = down_in_R; }
        void SetFastenersDownOutR(double down_out_R) {fasteners_down_out_R = down_out_R; }
        void SetFastenersLengthDown(double length_down) { fasteners_length_down = length_down; }
        void SetFastenersBoltsR(double bolts_R) { fasteners_bolts_R = bolts_R; }
        void SetFastenersBoltsLength(double bolts_length) { fasteners_bolts_length = bolts_length; }

        //Simulation/DetSimV2/CentralDetector/src/PrototypeConstruction.cc
        //-----------------------------------------------------------------
        double m_steel_tube_height;
        double m_steel_tube_radius;
        double m_steel_tube_thickness;
        double m_target_radius;
        double m_target_chimney_radius;
        double m_acrylic_ball_thickness;
        double m_acrylic_chimney_radius;
        double m_acrylic_circle_radius;
        double m_acrylic_circle_height;

        void SetSteelTubeHeight(double steel_tube_height) { m_steel_tube_height = steel_tube_height; }
        void SetSteelTubeRadius(double steel_tube_radius) { m_steel_tube_radius = steel_tube_radius; }
        void SetSteelTubeThickness(double steel_tube_thickness) { m_steel_tube_thickness = steel_tube_thickness; }
        void SetTargetRadius(double target_radius) { m_target_radius = target_radius; }
        void SetTargetChimneyRadius(double target_chimney_radius) { m_target_chimney_radius = target_chimney_radius; }
        void SetAcrylicBallThickness(double acrylic_ball_thickness) { m_acrylic_ball_thickness = acrylic_ball_thickness; }
        void SetAcrylicChimneyRadius(double acrylic_chimney_radius) { m_acrylic_chimney_radius = acrylic_chimney_radius; }
        void SetAcrylicCircleRadius(double acrylic_circle_radius) { m_acrylic_circle_radius = acrylic_circle_radius; }
        void SetAcrylicCircleHeight(double acrylic_circle_height) { m_acrylic_circle_height = acrylic_circle_height; }

        // Simulation/DetSimV2/CentralDetector/src/StrutAcrylicConstruction.cc
        // -------------------------------------------------------------------
        double m_radStrut_in;
        double m_radStrut_out;
        double m_lengthStrut;
        double gap;

        void SetRadStrutIn(double radStrut_in) { m_radStrut_in = radStrut_in; }
        void SetRadStrutOut(double radStrut_out) { m_radStrut_out = radStrut_out; }
        void SetLengthStrut(double lengthStrut) { m_lengthStrut = lengthStrut; }
        void SetGap(double value) { gap = value; }

};

#endif
