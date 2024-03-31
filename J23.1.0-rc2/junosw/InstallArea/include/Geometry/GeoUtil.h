#ifndef JUNO_GEO_UTIL_H
#define JUNO_GEO_UTIL_H

//
//  This class provides some utilities to use in geometry 
//
//  Author: Zhengyun You  2013-11-20
//

class TVector3;

class GeoUtil
{
    public :

        static int projectAitoff2xy(double l, double b, double &Al, double &Ab);

        // From a vertex + direction vector, get its intersection with a sphere with radius r
        static TVector3 getSphereIntersection(TVector3 vtx, TVector3 dir, double r);

        static double cm2mm() { return 10.0; }  // use CLHEP when available
        static double mm2cm() { return 0.1; }
        
};

#endif // JUNO_GEO_UTIL_H

