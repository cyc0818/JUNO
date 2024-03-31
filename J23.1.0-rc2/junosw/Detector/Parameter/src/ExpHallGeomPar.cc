#include "Parameter/ExpHallGeomPar.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "TString.h"
#include <sstream>

void ExpHallGeomPar::InitExpHallVariables()
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
            if (key == "ExpHall.Length.X"){
                SetExpHallXLength(value);
                continue;
            }
            else if (key == "ExpHall.Length.Y"){
                SetExpHallYLength(value);
                continue;
            }
            else if (key == "ExpHall.Length.ZSide"){
                SetExpHallZLengthSide(value);
                continue;
            }
            else if (key == "ExpHall.Length.ZDome"){
                SetExpHallZLengthDome(value);
                continue;
            }
            else if (key == "ExpHall.Length.RDome"){
                SetExpHallRLengthDome(value);
                continue;
            }
            else if (key == "ExpHall.Offset.X"){
                SetExpHallXOffset(value);
                continue;
            }
            else if (key == "ExpHall.Offset.Y"){
                SetExpHallYOffset(value);
                continue;
            }
            else if (key == "WP.Height"){
                SetExpHallZLowest(value/2);
                continue;
            }
        }
    }

}

