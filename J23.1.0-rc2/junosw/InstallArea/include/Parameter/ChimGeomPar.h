#ifndef JUNO_CHIM_GEO_PAR_H
#define JUNO_CHIM_GEO_PAR_H

#include <iostream>

class ChimGeomPar
{
    public:

        ChimGeomPar() { std::cout << "ChimGeomPar constructor" << std::endl; }
        ~ChimGeomPar() { std::cout << "ChimGeomPar destructor" << std::endl; }

        // Simulation/DetSimV2/Chimney/inlcude/Dimensions.hh
        // ---------------------------------------------------
        double GetTyvekThick() { return Tyvek_thickness; }
        double GetTubeInnerR() { return TubeInnerR; }
        double GetUpperTubeH() { return UpperTubeH; }
        double GetSteelThick() { return Steel_thickness; }
        double GetUpperSteelTubeH() { return upperSteelTubeH; }
        double GetLowerSteelTUbeH() { return lowerSteelTubeH; }
        double GetInnerBoxLength() { return Inner_Box_Length; }
        double GetInnerBoxWideth() { return Inner_Box_Wideth; }
        double GetInnerBoxHeight() { return Inner_Box_Height; }
        double GetBlockerThick() { return Blocker_thickness; }
        double GetBlockerLength() { return Blocker_length; }
        double GetBlockerWidth() { return Blocker_width; }

        void InitChimneyVariables();

    private:

        // Simulation/DetSimV2/Chimney/
        //------------------------------------------
        // for both upper and lower chimney
        double Tyvek_thickness;     // thickness of the reflector/absorbsion tyvek.
        double TubeInnerR;          // inner R of acrylic tube.
        // for upper chimney
        double UpperTubeH;          // upper chimney height;
        // for lower chimney
        double Steel_thickness;     // steel box/tube thickness **** please update ! ******
        double upperSteelTubeH;     // steel tube above the steel box od lower chimney
        double lowerSteelTubeH;     // steel tube under the steel box od lower chimney
        double Inner_Box_Length;    // dimensions of the steel box: inner length
        double Inner_Box_Wideth;    // dimensions of the steel box: inner width
        double Inner_Box_Height;    // dimensions of the steel box: inner height
        double Blocker_thickness;   // thickness of  blocker in the center of the steel box.
        double Blocker_length;      // thickness of  blocker in the center of the steel box.
        double Blocker_width;       // thickness of  blocker in the center of the steel box.

        void SetTyvekThick(double tyvekthickness) { Tyvek_thickness = tyvekthickness; }
        void SetTubeInnerR(double innr) { TubeInnerR = innr; }
        void SetUpperTubeH(double tubeh) { UpperTubeH = tubeh; }
        void SetSteelThick(double steelthick) { Steel_thickness = steelthick; }
        void SetUpperSteelTubeH(double value) { upperSteelTubeH = value; }
        void SetLowerSteelTubeH(double value) { lowerSteelTubeH = value; }
        void SetInnerBoxLength(double boxlength) { Inner_Box_Length = boxlength; }
        void SetInnerBoxWideth(double boxwideth) { Inner_Box_Wideth = boxwideth; }
        void SetInnerBoxHeight(double boxheight) { Inner_Box_Height = boxheight; }
        void SetBlockerThick(double blockthick) { Blocker_thickness = blockthick; }
        void SetBlockerLength(double blocklength) { Blocker_length = blocklength; }
        void SetBlockerWidth(double blockwidth) { Blocker_width = blockwidth; }


};

#endif
