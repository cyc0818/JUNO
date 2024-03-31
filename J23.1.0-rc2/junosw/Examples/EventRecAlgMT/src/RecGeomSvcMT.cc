
#include <boost/python.hpp>
#include "RecGeomSvcMT.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperPtr.h"

#include "TROOT.h"

DECLARE_SERVICE(RecGeomSvcMT);

RecGeomSvc* RecGeomSvcMT::m_geom = NULL;

RecGeomSvcMT::RecGeomSvcMT(const std::string& name)
    : SvcBase(name) {

}

RecGeomSvcMT::~RecGeomSvcMT() {

}

bool RecGeomSvcMT::initialize() {
    // if the geometry is not initialized, get it from RecGeomSvc.
    // assume this is the master thread
    if (!m_geom) {
        ROOT::EnableThreadSafety();

	SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
	if ( rgSvc.invalid()) {
		LogError << "Failed to get RecGeomSvc instance!" << std::endl;
		return false;
	}
        m_geom = rgSvc.data();
    }

    return true;
}

bool RecGeomSvcMT::finalize() {
    return true;
}

RecGeomSvc* RecGeomSvcMT::geom() {
    return m_geom;
}
