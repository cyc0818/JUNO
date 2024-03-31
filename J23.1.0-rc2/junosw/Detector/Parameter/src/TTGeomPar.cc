#include "Parameter/TTGeomPar.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "TString.h"
#include <sstream>

void TTGeomPar::InitTTVariables()
{
    std::string parameterPath = getenv("JUNO_PARAMETER_PATH");
    TString parameterFile = parameterPath + "/data/Dimensions.txt";
    std::ifstream fin(parameterFile);

    const int BufSize = 512;
    char lineBuf[BufSize];
    std::string key;
    double value;

    while (fin.getline(lineBuf,BufSize))
    {
        std::istringstream strBuf(lineBuf);
	    if (strBuf >> key >> value){
			if (key == "TT.Box.X"){
				SetBoxX(value);
				continue;
			}
			if (key == "TT.Box.Y"){
				SetBoxY(value);
				continue;
			}
			if (key == "TT.Box.Z"){
				SetBoxZ(value);
				continue;
			}
			if (key == "TT.Chimney.Radius"){
				SetChimneyR(value);
				continue;
			}
			if (key == "TT.Chimney.Z"){
				SetChimneyZ(value);
				continue;
			}
			if (key == "TT.Bar.Length"){
				SetLengthBar(value);
				continue;
			}
			if (key == "TT.Bar.Thickness"){
				SetThicknessBar(value);
				continue;
			}
			if (key == "TT.Bar.Width"){
				SetWidthBar(value);
				continue;
			}
			if (key == "TT.Bar.Space"){
				SetSpaceBar(value);
				continue;
			}
			if (key == "TT.Coating.Length"){
				SetLengthCoating(value);
				continue;
			}
			if (key == "TT.Coating.Thickness"){
				SetThickCoating(value);
				continue;
			}
			if (key == "TT.Coating.Width"){
				SetWidthCoating(value);
				continue;
			}
			if (key == "TT.ModuleTape.Length"){
				SetLengthModuleTape(value);
				continue;
			}
			if (key == "TT.ModuleTape.Thickness"){
				SetThickModuleTape(value);
				continue;
			}
			if (key == "TT.ModuleTape.Width"){
				SetWidthModuleTape(value);
				continue;
			}
			if (key == "TT.Module.Length"){
				SetLengthModule(value);
				continue;
			}
			if (key == "TT.Module.Thickness"){
				SetThickModule(value);
				continue;
			}
			if (key == "TT.Module.Width"){
				SetWidthModule(value);
				continue;
			}
			if (key == "TT.Module.Space"){
				SetSpaceModule(value);
				continue;
			}
			if (key == "TT.Plane.Length"){
				SetLengthPlane(value);
				continue;
			}
			if (key == "TT.Plane.Thickness"){
				SetThickPlane(value);
				continue;
			}
			if (key == "TT.Plane.Width"){
				SetWidthPlane(value);
				continue;
			}
			if (key == "TT.Plane.VSpace"){
				SetVSpacePlane(value);
				continue;
			}
			if (key == "TT.Wall.X"){
				SetWallX(value);
				continue;
			}
			if (key == "TT.Wall.Y"){
				SetWallY(value);
				continue;
			}
			if (key == "TT.Wall.Z"){
				SetWallZ(value);
				continue;
			}
			if (key == "TT.LowerWall.Z"){
				SetZLowerWall(value);
				continue;
			}
			if (key == "TT.Shift.Z"){
				SetZShift(value);
				continue;
			}
			if (key == "TT.Space.Z"){
				SetZSpace(value);
				continue;
			}
			if (key == "TT.SpaceChimney.Z"){
				SetZSpaceChimney(value);
				continue;
			}
			if (key == "TT.Space.X"){
				SetXSpace(value);
				continue;
			}
			if (key == "TT.Space1.X"){
				SetXSpace_1(value);
				continue;
			}
			if (key == "TT.Space.Y"){
				SetYSpace(value);
				continue;
			}
			if (key == "TT.Hole.X"){
				SetXHole(value);
				continue;
			}
			if (key == "TT.LowerWallChimney.Z"){
				SetZLowerWallChimney(value);
				continue;
			}
		}
    }

}

