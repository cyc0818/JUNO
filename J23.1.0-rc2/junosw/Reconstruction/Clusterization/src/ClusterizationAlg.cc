#include "ClusterizationAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Incident.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"

#include "Event/CdLpmtCalibHeader.h"

#include "Event/ClusterInfoHeader.h"

#include "Geometry/RecGeomSvc.h"
#include<vector>
#include<list>
#include<Math/DistFunc.h>

DECLARE_ALGORITHM(ClusterizationAlg);

ClusterizationAlg::ClusterizationAlg(const std::string& name)
    :AlgBase(name)
    , m_calibheader(NULL)
    , m_calibevent(NULL)
    , m_clusterheader(NULL)
    , m_clusterevent(NULL) 
{
    //declProp("someStuff", someVariable);
    declProp("StartThreshold", m_startthreshold_sigma = 6.50131); // in n_sigma over DarkRate, numbers presented in docDB 3132
    declProp("EndThreshold", m_endthreshold_sigma = -0.821532); //in n_sigma over DarkRate
    declProp("windowSize", m_window = 50.); // in ns
    declProp("darkRate", m_darkRate = 30.e3); // in Hz
    declProp("sizeDAQwin", m_sizeDAQwin = 1250.); // in ns
}

ClusterizationAlg::~ClusterizationAlg()
{

}

bool ClusterizationAlg::initialize()
{

    //SniperPtr<RecGeomSvc> rgSvc("RecGeomSvc");
    //if ( rgSvc.invalid()) {
    //    LogError << "Failed to get RecGeomSvc instance!" << std::endl;
    //    return false;
    //}
    //m_numberPMTs = rgSvc->getCdGeom()->findPmt20inchNum();
    m_numberPMTs = 17746; // hard-coded, better use geometry later
    LogDebug << "Using " << m_numberPMTs << " PMTs\n";
    m_nBins = static_cast<unsigned int>(m_sizeDAQwin); 
    m_hittimes = new TH1D("InternalClusterHist","InternalClusterHist",m_nBins,0.,m_sizeDAQwin);
    m_integralHitTimes = new double[m_nBins+1]; 
    m_clusterStarts = new std::vector<double>(4); // 4 clusters should be enough
    m_clusterEnds = new std::vector<double>(4); // 4 clusters should be enough

    m_iEvt = -1;
    this->calculateHitThresholds();

    return true;
}

bool ClusterizationAlg::finalize()
{
    delete m_hittimes;
    delete[] m_integralHitTimes;
    delete m_clusterStarts;
    delete m_clusterEnds;
    return true;
}

bool ClusterizationAlg::execute()
{
    ++m_iEvt; 
    LogDebug << "Executing clusterization algorithm for event "<<m_iEvt<<'\n';
    m_calibheader = 0;
    m_calibevent = 0;

    bool retval = false; //due to coding conventions returns should not be nested

    if(load_calib_event()){
        int ncluster = clusterizeEvent();
	if(!writeClusterInfo()){
            LogError << "Error while creating ClusterInfoEvent!\n";
            retval = false;
            }
        LogInfo << "Found " << ncluster << " clusters!\n";
        retval = true;
    }
    else{
        LogError << "Calib event could not be loaded!\n";
        retval = false;
    }

    return retval;    
}


int ClusterizationAlg::clusterizeEvent()
{
    LogInfo << "Finding clusters... \n";
    // sort hits in an histogram:
    prepareHits();
    // now find cluster informations (starts and ends)
    m_clusterStarts->clear(); 
    m_clusterEnds->clear();
    bool checkforstart = true;
    int nCluster = 0;
    for(unsigned int bin = 1;bin<=m_nBins;bin++){
        double windowhits = m_integralHitTimes[bin];
        // Are we outside a cluster (look for a start) or inside (look for an end)?
        if(checkforstart){
            if(windowhits>m_startthreshold){
                LogDebug << "Found a cluster start!\n";
                nCluster++;
                int startbin = bin;
                // go back in bins to check for real cluster start (starts at end threshold, not start for conservativeness)
                for(int subbin = bin;subbin>1;subbin--){
                    windowhits = m_integralHitTimes[subbin];
                    if(windowhits<m_endthreshold) break; //we reached DN level
                    else startbin = subbin;
                }
                double cl_start_fill = m_hittimes->GetXaxis()->GetBinLowEdge(startbin);
                LogDebug<<"Found the start time to be " << cl_start_fill <<'\n';
                m_clusterStarts->push_back(cl_start_fill);
                checkforstart = false;
            }
            else continue;
        }
        else{ //look for end
            if(windowhits<m_endthreshold){
                double cl_end_fill = m_hittimes->GetXaxis()->GetBinUpEdge(bin);
                LogDebug<<"Found the cluster end at " << cl_end_fill <<'\n';
                m_clusterEnds->push_back(cl_end_fill);
                checkforstart = true;
            }
            else continue;
        }
    }
    // last sanity check:
    if(m_clusterStarts->size() == m_clusterEnds->size() && static_cast<int>(m_clusterStarts->size()) == nCluster && checkforstart){ //static cast to make the compiler happy
        LogDebug << "Clusterization finished, found " <<nCluster <<" clusters"<<'\n';
    	return nCluster;
    }
    else{
        LogError<< "Number of clusters do not match! There is a bug in the clusterization code\n";
        return -1;
    }
}

bool ClusterizationAlg::writeClusterInfo()
{

    LogDebug << "Creating the ClusterInfoEvent\n";

    //write it out
    //Load the navigator again
    bool retval = true;
    SniperDataPtr<JM::NavBuffer>  navBuf(*getParent(), "/Event");
    if (navBuf.invalid()) {
        retval = false;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;
    auto evt_nav = navBuf->curEvt();
    LogDebug << "evt_nav: " << evt_nav << std::endl;
    if (not evt_nav) {
        retval = false;
    }

    auto cih = new JM::ClusterInfoHeader;
    cih->setEvent(new JM::ClusterInfoEvt);
    auto cie = (JM::ClusterInfoEvt*)cih->event();

    // Find out the cluster for each hit
    const auto& chhlist = m_calibevent->calibPMTCol();

    unsigned int channelidx = 0.;
    for(auto chit=chhlist.begin();chit!=chhlist.end();chit++,channelidx++){
        auto calib = *chit;
        const auto &c_time = calib->time();
        for(unsigned int hitidx = 0;hitidx<c_time.size();++hitidx){
            //decide to which cluster the time belongs
            const double& tmptime = c_time.at(hitidx);
            unsigned int clusteridx=0;
            //loop over all clusters and sort in the hit
            for(;clusteridx<m_clusterStarts->size();clusteridx++){
                if(tmptime>m_clusterStarts->at(clusteridx) && tmptime<m_clusterEnds->at(clusteridx)){
                    //right cluster found, stop here
                    break;
                }
                else{
                    //try next cluster
                    continue;
                }   
            }
            // first all physics clusters, then the DarkNoise cluster (clusteridx==m_clusterStarts->size()
            cie->AddHitToCluster(channelidx,hitidx,clusteridx,tmptime);
        }
    }

    //cih->setEvent(cie);
    JM::addHeaderObject(evt_nav, cih);

    return retval;
}

bool ClusterizationAlg::load_calib_event()
{
    bool retval = true;
    SniperDataPtr<JM::NavBuffer>  navBuf(*getParent(), "/Event");
    if (navBuf.invalid()) {
        retval = false;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;
    auto evt_nav = navBuf->curEvt();
    LogDebug << "evt_nav: " << evt_nav << std::endl;
    if (not evt_nav) {
        retval = false;
    }
    m_calibheader = JM::getHeaderObject<JM::CdLpmtCalibHeader>(evt_nav);
    LogDebug << "calibhdr: " << m_calibheader << std::endl;
    if (not m_calibheader) {
        retval = false;
    }
    m_calibevent = dynamic_cast<JM::CdLpmtCalibEvt*>(m_calibheader->event());
    LogDebug << "calibevent: " << m_calibevent << std::endl;
    if (not m_calibevent) {
        retval = false;
    }

    return retval;
}

bool ClusterizationAlg::prepareHits()
{
    
    m_hittimes->Reset();
    const auto& chhlist = m_calibevent->calibPMTCol();

    for(auto chit = chhlist.begin(); chit!=chhlist.end(); chit++){
        auto calib = *chit;
        const auto &c_time = calib->time();
        for(size_t i = 0;i<c_time.size();++i){
            m_hittimes->Fill(c_time.at(i));
        }
    }
    for(unsigned int bin=1;bin<m_nBins;++bin){
        m_integralHitTimes[bin] = m_hittimes->Integral(bin-m_window/2.,bin+m_window/2.); 
    }
    m_integralHitTimes[0] = 0;
    m_integralHitTimes[m_nBins] = 0;
    LogDebug << "Filled hit time distribution\n";
    LogDebug << "Number of entries:\t"<<m_hittimes->GetEntries()<<'\n';


    return true; //TODO change return type or give it a meaning
}

bool ClusterizationAlg::calculateHitThresholds()
{
    m_startthreshold = this->calculatePoissonLimit(m_startthreshold_sigma);
    m_endthreshold   = this->calculatePoissonLimit(m_endthreshold_sigma);
    if(m_startthreshold < 30000. && m_endthreshold < 30000.){
        LogDebug << "Clusterization thresholds calculated: \n";
        LogDebug << " start threshold = "<<m_startthreshold<<" hits \n";
        LogDebug << " end   threshold = "<<m_endthreshold<<" hits \n";
        LogDebug << " expected dark hits in window = "<<m_darkRate*m_numberPMTs*m_window*1.e-9<<" hits \n";
        return true;
    }
    else{
        LogWarn << "Clusterization thresholds are too high, it is possibly a bug:\tstart="<<m_startthreshold<<" hits\tend="<<m_endthreshold<<" hits\n";
        return false;
    }
}


unsigned int ClusterizationAlg::calculatePoissonLimit(const double sigma)
{

    double mu = m_darkRate*m_numberPMTs*m_window*1.e-9; //expected DN in time window, 1e-9 conversion Hz to 1/ns
    double CL = ROOT::Math::normal_cdf(sigma);

    unsigned int poissonLimit = -1;    
    if(mu<100) {
        poissonLimit = 0;
        while(ROOT::Math::poisson_cdf(poissonLimit,mu)<=CL)                                         
            ++poissonLimit;                                                                         
    } else
        poissonLimit = ROOT::Math::gaussian_quantile(CL,sqrt(mu))+mu;                               

    return poissonLimit;   
}
