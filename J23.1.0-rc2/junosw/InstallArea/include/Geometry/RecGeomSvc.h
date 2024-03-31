#ifndef JUNO_REC_GEOM_SVC_H
#define JUNO_REC_GEOM_SVC_H

//
//  Geometry Service to be used in reconstruction and others
//
//  Author: Zhengyun You  2013-11-20
//

#include "SniperKernel/SvcBase.h"
#include "Identifier/Identifier.h"
#include "Geometry/CdGeom.h"
#include "Geometry/WpGeom.h"
#include "Geometry/TtGeom.h"
#include <map>

/**************************************************************************
 * Examples
 *     RecGeomSvc* svc = SvcMgr::get<RecGeomSvc>("RecGeomSvc");
 *     svc->initialize();
 *     CdGeom *cdGeom = svc->getCdGeom();
 *
 **************************************************************************
 */

class RecGeomSvc : public SvcBase
{
    public :

	RecGeomSvc(const std::string& name);
	virtual ~RecGeomSvc();

	bool initialize();
	bool finalize();

        // Initialization
        //-----------------------------------------------
        bool initRootGeom();
        bool initCdGeom();
        bool initWpGeom();
	bool initTtGeom();

	// Control initialization
	// ----------------------------------------------
	void initCdGeomControl(bool value);
        void initWpGeomControl(bool value);
	void initTtGeomControl(bool value);

        // Get ptr to each sub detector Geom
        //-----------------------------------------------
        CdGeom* getCdGeom();
        WpGeom* getWpGeom();
	TtGeom* getTtGeom();

    private :

        std::string m_geomFileName;
        std::string m_geomPathName;
        bool m_fastInit;
        bool m_initCd;
        bool m_initWp;
        bool m_initTt;
        CdGeom* m_CdGeom;
        WpGeom* m_WpGeom;
	TtGeom* m_TtGeom;
        int m_verbose;
};

#endif // JUNO_REC_GEOM_SVC_H
