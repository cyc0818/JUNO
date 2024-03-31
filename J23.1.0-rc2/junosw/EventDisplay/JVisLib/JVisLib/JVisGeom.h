#ifndef JUNO_VIS_GEOM_H
#define JUNO_VIS_GEOM_H

//
//  Juno Visualization Geometry
//
//  Author: Zhengyun You  2013-12-25
//

#include "TString.h"

class CdGeom;
class WpGeom;

class JVisGeom 
{
    public :

        JVisGeom();
        virtual ~JVisGeom();

        bool initialize(TString geomFile);

        CdGeom* getCdGeom();
        WpGeom* getWpGeom();

        double get20inchPmtRadius() { return 25.4*20/2; }

    private :

        CdGeom* m_CdGeom;
        WpGeom* m_WpGeom;

};

#endif // JUNO_VIS_GEOM_H


