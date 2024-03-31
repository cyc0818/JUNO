#ifndef RecGeomSvcMT_h
#define RecGeomSvcMT_h

/*
 * Description:
 *   This service will cache the pointer to the real RecGeomSvc.
 *
 *   At main thread:
 *     - cache the real RecGeomSvc
 *   At worker threads:
 *     - access the single instance
 *
 * Author:
 *   Tao Lin <lintao@ihep.ac.cn>
 */

#include "Geometry/RecGeomSvc.h"

class RecGeomSvcMT: public SvcBase {
public:
    RecGeomSvcMT(const std::string& name);
    ~RecGeomSvcMT();

    bool initialize();
    bool finalize();

    static RecGeomSvc* geom();

private:
    static RecGeomSvc* m_geom;

};

#endif
