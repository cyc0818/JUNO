#include <boost/python.hpp>
#include "G4SystemOfUnits.hh"
#include "GlobalGeomInfo.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/ToolFactory.h"

#include "globals.hh"

DECLARE_TOOL(GlobalGeomInfo);

GlobalGeomInfo::GlobalGeomInfo(const std::string& name)
    : ToolBase(name)
{
    init_variables();

    declProp("LS.AbsLen", m_LS_abslen_at430);
    declProp("LS.RayleighLen", m_LS_raylen_at430);
    declProp("LS.LightYield", m_LS_light_yield=-1); // if LY<0, use default one.

    declProp("UseParamSvc", m_use_param_svc=false); // Using IMCParamsSvc
}

GlobalGeomInfo::~GlobalGeomInfo()
{

}

double
GlobalGeomInfo::geom_info(const std::string& param)
{
    // TODO use a map to replace these if else.
    if (param == "") {
        
    } else if (param == "ExpHall.XLen") {
        return m_expHallXLen;
    } else if (param == "ExpHall.XLenGap") {
        return m_expHallXLen_gap;
    } else if (param == "ExpHall.XLenTot") {
        return m_expHallXLen_tot;
    } else if (param == "ExpHall.YLen") {
        return m_expHallYLen;
    } else if (param == "ExpHall.ZLen") {
        return m_expHallZLen;
    } else if (param == "ExpHall.+X") {
        return m_expHallXmax;
    } else if (param == "ExpHall.-X") {
        return m_expHallXmin;
    } else if (param == "ExpHall.+Y") {
        return m_expHallYmax;
    } else if (param == "ExpHall.-Y") {
        return m_expHallYmin;
    } else if (param == "ExpHall.+Z") {
        return m_expHallZmax;
    } else if (param == "ExpHall.-Z") {
        return m_expHallZmin;
    } else if (param == "ExpHall.ZLenTot") {
        return m_expHallZLen_tot;

    } else if (param == "ExpHall.domeH") {
        return m_expHall_domeHeight;
    } else if (param == "ExpHall.domeR") {
        return m_expHall_domeRadius;

    } else if (param == "WaterPool.R") {
        return m_waterPoolRadius;
    } else if (param == "WaterPool.H") {
        return m_waterPoolHeight;
    } else if (param == "WaterPool.ConcreteH") {
        return m_waterPoolConcreteHeight;
    } else if (param == "WaterPool.LiningThick") {
        return m_poolLiningThickness;

    } else if (param == "Rock.Thickness") {
        return m_rockThickness;
    } else if (param == "TopRock.XLen") {
        return m_topRockXLen;
    } else if (param == "TopRock.YLen") {
        return m_topRockYLen;
    } else if (param == "TopRock.ZLen") {
        return m_topRockZLen;
    } else if (param == "TopRock.ZLenTot") {
        return m_topRockZLen_tot;
    } else if (param == "TopRock.domeH") {
        return m_topRock_domeHeight;
    } else if (param == "TopRock.domeR") {
        return m_topRock_domeRadius;

    } else if (param == "BtmRock.H") {
        return m_btmRockHeight;
    } else if (param == "BtmRock.R") {
        return m_btmRockRadius;


    } else if (param == "LS.AbsLen") {
        return m_LS_abslen_at430;
    } else if (param == "LS.RayleighLen") {
        return m_LS_raylen_at430;
    } else if (param == "LS.LightYield") {
        return m_LS_light_yield;
    } else if (param == "UseParamSvc") {
        return m_use_param_svc;
    } else {

    }
    throw SniperException(std::string("Can't find param: ") + param);
    return 0;
}

double
GlobalGeomInfo::geom_info(const std::string& param, int /* idx */)
{
    throw SniperException(std::string("Can't find param: ") + param);
    return 0;
}

void
GlobalGeomInfo::init_variables()
{
    // == Experiment Hall ==
    m_expHallXLen = 50*m; //to keep ExpHall center above CD
    m_expHallXLen_gap = 6.25*m;
    m_expHallXLen_tot = m_expHallXLen + m_expHallXLen_gap; //JUNOEng-doc-5-v3, 
    m_expHallYLen = 49*m; // 
    m_expHallZLen = 11*m;
    m_expHall_domeHeight = 16*m;
    m_expHallZLen_tot = m_expHallZLen + m_expHall_domeHeight; //ground to dome, = bot rectangle h 11*m + upper spherical cap h 16*m
    m_expHall_domeRadius = 26.76*m;

    // == Water Pool ==
    m_waterPoolRadius = 43.5*m/2;
    m_waterPoolHeight = 43.5*m;
    m_waterPoolConcreteHeight = 44*m;
    m_poolLiningThickness = 5*mm;

    //Rock
    m_rockThickness = 3*m;
    m_topRockXLen = m_expHallXLen_tot + 2* m_rockThickness;
    m_topRockYLen = m_expHallYLen + 2* m_rockThickness;
    m_topRockZLen = m_expHallZLen;
    m_topRock_domeHeight = m_expHall_domeHeight + m_rockThickness;
    m_topRock_domeRadius = m_expHall_domeRadius + m_rockThickness;
    m_topRockZLen_tot = m_topRockZLen +  m_topRock_domeHeight;

    m_btmRockHeight = m_waterPoolConcreteHeight + m_rockThickness;
    m_btmRockRadius = m_waterPoolRadius + m_rockThickness;

    m_expHallXmax = m_expHallXLen/2.;
    m_expHallYmax = m_expHallYLen/2.;
    m_expHallZmax = m_expHallZLen/2.;

    m_expHallXmin = -m_expHallXLen/2.;
    m_expHallYmin = -m_expHallYLen/2.;
    m_expHallZmin = -m_expHallZLen/2.;
    // == Central Detector ==
    //
    // == material ==
    m_LS_abslen_at430 = 77.;
    m_LS_raylen_at430 = 27.;
}
