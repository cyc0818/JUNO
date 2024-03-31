/*=============================================================================
#
# Author: Philipp Kampmann - p.kampmann@fz-juelich.de
# Last modified: 2018-10-08 17:30
# Filename: Clusterization.h
# Description: This algorithm needs to be called after the waveform
reconstruction to sort the hits into clusters. As the size of the DAQ-window is 
independent of the length of the event, clusterization helps to get a better
handle on events with multiple clusters and other hits which do not belong to
the primary cluster (e.g. dark noise hits). The cluster affiliation for each hit
is stored parallel to the CalibEvent in a ClusterInfoEvent.
=============================================================================*/



#ifndef ClusterizationAlg_h
#define ClusterizationAlg_h

#include "SniperKernel/AlgBase.h"
#include <TH1.h>
#include <vector>

namespace JM {
    class EvtNavigator;
    class CdLpmtCalibHeader;
    class CdLpmtCalibEvt;
    class ClusterInfoEvt;
    class ClusterInfoHeader;
    class ClusterHitIdx;
}

class ClusterizationAlg : public AlgBase {
    public: // usual public AlgBase methods
        ///Constructor: creates the AlgBase object and declares user-definable properties
        ClusterizationAlg(const std::string& name);
        /// Destructor
        ~ClusterizationAlg();
         
        bool initialize();
        bool execute();
        bool finalize();

    private: //Private methods
        /// Loads the input calib event from the EvtNavigator
        bool load_calib_event();
        ///constructs the output ClusterInfoEvent from the Clusterization output
        bool writeClusterInfo();
        /// Performs the clusterization this one event and fills the output member variables
        int clusterizeEvent();
        /// Fills hits from the calib event in an histogram 
        bool prepareHits();

        //Functions to convert input threshold to applied number of hits threshold
	///Calculates the hit thresholds for start and end of the cluster
        bool calculateHitThresholds(); 
        /// Calculate the poisson overshoot from the provided umber of sigmas
        unsigned int calculatePoissonLimit(const double sigma); 

    private:
        JM::CdLpmtCalibHeader* m_calibheader;
        JM::CdLpmtCalibEvt* m_calibevent;
        JM::ClusterInfoHeader* m_clusterheader;
        JM::ClusterInfoEvt* m_clusterevent;
       
        unsigned int m_numberPMTs; 
        // variables used by clusterization algorithm
        unsigned int m_iEvt;
        double m_startthreshold_sigma;
        double m_endthreshold_sigma;
        double m_sizeDAQwin;
        unsigned int m_startthreshold;
        unsigned int m_endthreshold;
        double m_window;
        double m_darkRate;
        unsigned int m_nBins;
        TH1D*  m_hittimes;
	    double*  m_integralHitTimes;
	    std::vector<double> *m_clusterStarts;
	    std::vector<double> *m_clusterEnds;

        //std::vector< std::vector<ClusterHitIdx> > clusterVec;

};

#endif
