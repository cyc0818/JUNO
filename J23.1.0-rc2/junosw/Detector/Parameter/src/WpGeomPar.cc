#include "Parameter/WpGeomPar.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "TString.h"
#include <sstream>

void WpGeomPar::InitWpVariables()
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
	    if (key == "WP.Height"){
                SetWpHeight(value);
		continue;
            }
            if (key == "WP.Radius"){
                SetWpRadius(value);
		continue;
            }

	}

    }

}

