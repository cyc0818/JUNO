#ifndef JUNO_EXPHALL_GEOM_PAR_H
#define JUNO_EXPHALL_GEOM_PAR_H

#include <iostream>


// New Dome geometry:
//
//  * side walls are a box up to m_expHallZlenSide with
//    horizontal surface m_expHallXlen by m_expHallYlen
//
//  * Above m_expHallZlenSide the ceiling is an arc in the YZ plane
//    * arc radius by m_expHallRlenDome
//    * maximum dome height given by m_expHallZlenSide + m_expHallZlenDome
//      (note there is a z-offset got the radius center!)
//
//  * The Experimental hall center is offset from the simulation center
//    by (m_expHallOffsetX, m_expHallOffsetY) in (X,Y)
//  * Th bottom of the Experimetal hall is given by m_expHallOffsetZ = WaterPool Height/2
//
// Geometry and values from Simulation/DetSimV2/DetSimOptions/src/RockConstruction.cc

class ExpHallGeomPar
{
    public:

        ExpHallGeomPar() { std::cout << "ExpHallGeomPar constructor" << std::endl; }
        ~ExpHallGeomPar() { std::cout << "ExpHallGeomPar destructor" << std::endl; }

	double GetExpHallXLength() { return m_expHallXlen; }
	double GetExpHallYLength() { return m_expHallYlen; }
	double GetExpHallZLengthSide() { return m_expHallZlenSide; }
	double GetExpHallZLengthDome() { return m_expHallZlenDome; }
	double GetExpHallZLength() { return m_expHallZlenSide+m_expHallZlenDome; }
	double GetExpHallRLengthDome() { return m_expHallRlenDome; }
	double GetExpHallDomeZOffset() { return m_expHallZlenSide+m_expHallZlenDome-m_expHallRlenDome; }
	double GetExpHallXOffset() { return m_expHallOffsetX; }
	double GetExpHallYOffset() { return m_expHallOffsetY; }
	double GetExpHallZLowest() { return m_expHallLowestZ; }


	void InitExpHallVariables();

    private:

	double m_expHallXlen;
	double m_expHallYlen;
	double m_expHallZlenSide;
	double m_expHallZlenDome;
	double m_expHallRlenDome;
	double m_expHallOffsetX;
	double m_expHallOffsetY;
	double m_expHallLowestZ;

	void SetExpHallXLength(double expHallXlen) { m_expHallXlen = expHallXlen; }
	void SetExpHallYLength(double expHallYlen) { m_expHallYlen = expHallYlen; }
	void SetExpHallZLengthSide(double expHallZlenSide) { m_expHallZlenSide = expHallZlenSide; }
	void SetExpHallZLengthDome(double expHallZlenDome) { m_expHallZlenDome = expHallZlenDome; }
	void SetExpHallRLengthDome(double expHallRlenDome) { m_expHallRlenDome = expHallRlenDome; }
	void SetExpHallXOffset(double expHallOffsetX) { m_expHallOffsetX = expHallOffsetX; }
	void SetExpHallYOffset(double expHallOffsetY) { m_expHallOffsetY = expHallOffsetY; }
	void SetExpHallZLowest(double expHallLowestZ) { m_expHallLowestZ = expHallLowestZ; }

};

#endif
