/*=============================================================================
#
# Author: Philipp Kampmann - p.kampmann@fz-juelich.de
# Last modified: 2018-10-05 14:30
# Filename: ClusterInfoSvc.h
# Description: While the informations about the clusters are stored in the 
ClusterInfoEvt and the physical information is stored in the CalibEvent. This
service should act as an interface to have an easier access to the clusterized
hits. Therefore this service combines the information from ClusterInfoEvt and
CalibEvent. To run properly one needs to have both: The CalibEvent and the 
ClusterInfoEvt.
=============================================================================*/



#ifndef CLUSTERINFOSVC_H
#define CLUSTERINFOSVC_H

#include "SniperKernel/SvcBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/CalibHeader.h"
#include "Event/ClusterInfoHeader.h"
#include "Event/ClusterInfoEvt.h"
#include "Event/ClusterHitIdx.h"

//class TVector3;
class ClusterInfoSvc : public SvcBase {

    public: // SvcBase functions
    ClusterInfoSvc(const std::string &name);
    ~ClusterInfoSvc();

    bool initialize();
    bool finalize();

    public: // my functions

    /// Before using this service one needs to initialize it by providing the evt navigator
    bool   initEvent(JM::EvtNavigator* nav);

    /// Get the number of clusters
    UInt_t getNCluster() const;

    /// Get the number of hits in the cluster
    UInt_t getClusterSize(UInt_t clusterIdx) const;

    /// Get the charge of the hit
    double getCharge(UInt_t clusterIdx, UInt_t hitIdx);

    /// Get the time of the hit
    double getTime(UInt_t clusterIdx, UInt_t hitIdx);

    /// Get the PMTID of the hit
    UInt_t getPMTID(UInt_t clusterIdx, UInt_t hitIdx);

    /// Get the cluster start time (ns)
    double getClusterStart(UInt_t clusterIdx);

    /// Get the cluster end time (ns)
    double getClusterEnd(UInt_t clusterIdx);

    /// Get the cluster duration (ns)
    double getClusterLength(UInt_t clusterIdx);


    private:
    bool readCalib();
    bool readCluster();


    JM::EvtNavigator* m_nav;
    JM::CalibHeader* m_calib;
    JM::ClusterInfoEvt* m_cluster;
};

#endif
