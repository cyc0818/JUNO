#include "Parameter/CdGeomPar.h"
#include <iostream>
#include <string>
#include <fstream>
#include "TString.h"
#include "stdlib.h"
#include <sstream>

void CdGeomPar::InitCdVariables()
{
    InitCDDetSim1Construction();
    InitCDFastenerAcrylicConstruction();
    InitCDPrototypeConstruction();
    InitCDStrutAcrylicConstruction(); 
}

void CdGeomPar::InitCDDetSim1Construction()
{
    std::cout << "CdGeomPar " << __func__ << " begin" << std::endl;

    std::string parameterPath = getenv("JUNO_PARAMETER_PATH");
    std::cout << "parameterPath " << parameterPath << std::endl;

    TString parameterFile = parameterPath + "/data/Dimensions.txt";
    std::cout << "parameterFile " << parameterFile << std::endl;

    std::ifstream fin(parameterFile);

    const int BufSize = 512;
    char lineBuf[BufSize];
    std::string key;
    double value; 

    while (fin.getline(lineBuf,BufSize))
    {
        std::istringstream strBuf(lineBuf);
        if (strBuf >> key >> value){
            if (key == "CD.LS.Radius"){
                SetRadLS(value);
                std::cout << "parameter radius is " << GetRadLS() << std::endl;
                continue;
            }   
            if (key == "CD.Acrylic.Thickness"){
                SetThickAcrylic(value);
                std::cout << "parameter thickness is " << GetThickAcrylic() << std::endl;
                continue;
            }
            if (key == "CD.LS.Chimeny.Radius"){
                SetRadLSChimney(value);
                continue;
            }
            if (key == "CD.InnerWater.Radius"){
                SetRadInnerWater(value);
                continue;
            }
            if (key == "CD.Reflector.Thickness"){
                SetThickReflector(value);
                continue;
            }
            if (key == "CD.ChimWater.Thickness"){
                SetThickReflector(value);
                continue;
            }

        }

    }
    std::cout << "CdGeomPar " << __func__ << " end" << std::endl;
}

void CdGeomPar::InitCDFastenerAcrylicConstruction()
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
            if (key == "CD.Fasteners.Up_Out.Radius"){
                SetFastenersUpOutR(value);
                continue;
            }
            if (key == "CD.Fasteners.Up.Length"){
                SetFastenersLengthUp(value);
                continue;
            }
            if (key == "CD.Fasteners.Up1_In.Radius"){
                SetFastenersUp1InR(value);
                continue;
            }
            if (key == "CD.Fasteners.Up1_Out.Radius"){
                SetFastenersUp1OutR(value);
                continue;
            }
            if (key == "CD.Fasteners.Up1.Length"){
                SetFastenersLengthUp1(value);
                continue;
            }
            if (key == "CD.Fasteners.Down_In.Radius"){
                SetFastenersDownInR(value);
                continue;
            }
            if (key == "CD.Fasteners.Down_Out.Radius"){
                SetFastenersDownOutR(value);
                continue;
            }
            if (key == "CD.Fasteners.Down.Length"){
                SetFastenersLengthDown(value);
                continue;
            }
            if (key == "CD.Fasteners.Bolts.Radius"){
                SetFastenersBoltsR(value);
                continue;
            }
            if (key == "CD.Fasteners.Bolts.Length"){
                SetFastenersBoltsLength(value);
                continue;
            }
        }
    }
}

void CdGeomPar::InitCDPrototypeConstruction()
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
            if (key == "CD.SteelTube.Height"){
                SetSteelTubeHeight(value);
                continue;
            }
            if (key == "CD.SteelTube.Radius"){
                SetSteelTubeRadius(value);
                continue;
            }
            if (key == "CD.SteelTube.Thickness"){
                SetSteelTubeThickness(value);
                continue;
            }
            if (key == "CD.Target.Radius"){
                SetTargetRadius(value);
                continue;
            }
            if (key == "CD.Target.Chimney.Radius"){
                SetTargetChimneyRadius(value);
                continue;
            }
            if (key == "CD.AcrylicBall.Thickness"){
                SetAcrylicBallThickness(value);
                continue;
            }
            if (key == "CD.Acrylic.Chimney.Radius"){
                SetAcrylicChimneyRadius(value);
                continue;
            }
            if (key == "CD.Acrylic.Circle.Radius"){
                SetAcrylicCircleRadius(value);
                continue;
            }
            if (key == "CD.Acrylic.Circle.Height"){
                SetAcrylicCircleHeight(value);
                continue;
            }
	}
    }

}

void CdGeomPar::InitCDStrutAcrylicConstruction()
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
            if (key == "CD.Strut.In.Radius"){
                SetRadStrutIn(value);
                continue;
            }
            if (key == "CD.Strut.Out.Radius"){
                SetRadStrutOut(value);
                continue;
            }
            if (key == "CD.Strut.Length"){
                SetLengthStrut(value);
                continue;
            }
            if (key == "CD.Strut.Gap"){
                SetGap(value);
                continue;
            }
        }
    }
}

