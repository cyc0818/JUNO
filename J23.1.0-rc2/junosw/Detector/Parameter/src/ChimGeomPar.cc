#include "Parameter/ChimGeomPar.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "TString.h"
#include <sstream>

void ChimGeomPar::InitChimneyVariables()
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
            if (key == "Chim.Tyvek.Thickness"){
                SetTyvekThick(value);
                continue;
            }
            if (key == "Chim.Tube.InnerRadius"){
                SetTubeInnerR(value);
                continue;
            }
            if (key == "Chim.UpperTube.Height"){
                SetUpperTubeH(value);
                continue;
            }
            if (key == "Chim.Steel.Thickness"){
                SetSteelThick(value);
                continue;
            }
            if (key == "Chim.Upper.SteelTube.Height"){
                SetUpperSteelTubeH(value);
                continue;
            }
            if (key == "Chim.Lower.SteelTube.Height"){
                SetLowerSteelTubeH(value);
                continue;
            }
            if (key == "Chim.Inner.Box.Length"){
                SetInnerBoxLength(value);
                continue;
            }
            if (key == "Chim.Inner.Box.Wideth"){
                SetInnerBoxWideth(value);
                continue;
            }
            if (key == "Chim.Inner.Box.Height"){
                SetInnerBoxHeight(value);
                continue;
            }
            if (key == "Chim.Blocker.Thickness"){
                SetBlockerThick(value);
                continue;
            }
            if (key == "Chim.Blocker.Length"){
                SetBlockerLength(value);
                continue;
            }
            if (key == "Chim.Blocker.Width"){
                SetBlockerWidth(value);
                continue;
            }
		}
    }

}

