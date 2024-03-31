#include "PullSimHeaderAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"

#include "Event/SimHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "Event/WpCalibHeader.h"

#include "Geometry/PMT.h"

#include <RandomSvc/IRandomSvc.h>
#include <TFile.h>
#include <TTree.h>

#include <list>
#include <cassert>
#include <vector>
#include <algorithm>

DECLARE_ALGORITHM(PullSimHeaderAlg);

PullSimHeaderAlg::PullSimHeaderAlg(const std::string& name)
    : AlgBase(name)
    , m_simheader(NULL)
    , m_simevent(NULL)
    , m_calibhitcol(NULL)
    , m_randomsvc(NULL)
{
    declProp("QEScale", m_qe_scale = 1.0);
    // if SmearQE is set, the QEScale will be omitted.
    declProp("SmearQE", m_flag_smear_qe=false); 
    declProp("SmearQEFile", m_smear_qe_file);
}

PullSimHeaderAlg::~PullSimHeaderAlg()
{

}

bool
PullSimHeaderAlg::initialize()
{

    //  random servcie
    if ((m_qe_scale<1.0) or m_flag_smear_qe) {
        SniperPtr<IRandomSvc> randomsvc(getParent(), "RandomSvc");
        if (randomsvc.invalid()) {
            LogError << "Failed to load RandomSvc" << std::endl;
            return false;
        }
        m_randomsvc = randomsvc.data();
        assert(m_randomsvc);
    }

    // load the smear data if necessary
    if (m_flag_smear_qe) {
        // load the data
        TFile* tmpf = TFile::Open(m_smear_qe_file.c_str());
        if (not tmpf) {
            LogError << "File: [" << m_smear_qe_file << "] does not exists"
                     << std::endl;
            return false;
        }
        TTree* tmppmtdata = (TTree*) tmpf->Get("PmtData");
        tmppmtdata ->SetBranchAddress("pmtId",&m_pmtId);
        tmppmtdata ->SetBranchAddress("efficiency",&m_efficiency);

        for (int i = 0; i < tmppmtdata->GetEntries(); ++i) {
            tmppmtdata->GetEntry(i);
            m_pmt2efficiency[m_pmtId] = m_efficiency;
        }
    }

    return true;
}

bool
PullSimHeaderAlg::execute()
{
    m_cache_first_hits.clear();
    assert( m_cache_first_hits.size() == 0 );
    LogDebug << "Reset Header and Event in Converter" << std::endl;
    // reset the pointer
    m_simheader = 0;
    m_simevent = 0;
    m_calibhitcol = 0;

    LogDebug << "Load Sim Header and Event in Converter" << std::endl;
    load_sim_event();
    LogDebug << "Convert to Calib Header and Event in Converter" << std::endl;
    return convert_to_calib();
}

bool
PullSimHeaderAlg::finalize()
{
    return true;
}

bool
PullSimHeaderAlg::load_sim_event()
{
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if (navBuf.invalid()) {
        return false;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;
    auto evt_nav = navBuf->curEvt();
    LogDebug << "evt_nav: " << evt_nav << std::endl;
    if (not evt_nav) {
        return false;
    }
    m_simheader = JM::getHeaderObject<JM::SimHeader>(evt_nav);
    LogDebug << "simhdr: " << m_simheader << std::endl;
    if (not m_simheader) {
        return false;
    }
    m_simevent = dynamic_cast<JM::SimEvt*>(m_simheader->event());
    LogDebug << "simevt: " << m_simevent << std::endl;
    if (not m_simevent) {
        return false;
    }

    return true;
}

bool
PullSimHeaderAlg::convert_to_calib()
{
    if (not m_simevent) {
        LogError << "Can't Load SimEvent" << std::endl;
        return false;
    }
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if (navBuf.invalid()) {
        return false;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;
    auto evt_nav = navBuf->curEvt();
    if (not evt_nav) {
        evt_nav = new JM::EvtNavigator;
        TTimeStamp ts;
        evt_nav->setTimeStamp(ts);
        SniperPtr<IDataMemMgr> mMgr(getParent(), "BufferMemMgr");
        if (mMgr.invalid()) {
            LogError << "Can't find BufferMemMgr" << std::endl;
            return false;
        }
        mMgr->adopt(evt_nav, "/Event");
    }

    // TODO: use CD LPMT as an example. The other sub-detectors should implement the similar code.
    // Tao Lin, 2022.03.08

    // Build the Calib.
    auto ch = new JM::CdLpmtCalibHeader; 
    auto ce = new JM::CdLpmtCalibEvt; 

    // select the first hit time in every PMT
    select_first_hit();
    std::list<JM::CalibPmtChannel*>vcph; 

    for (auto it = m_cache_first_hits.begin();
            it != m_cache_first_hits.end();
            ++it) {
        vcph.push_back(it->second);
    }

    ce->setCalibPMTCol(vcph); 
    ch->setEvent(ce); 
    JM::addHeaderObject(evt_nav, ch); 

    return true;
}

// FIXME: Maybe the pmtid using Identifier 
//        should be created in DetSim
// Used for convert the pmtid.
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include "Identifier/WpID.h"

bool
PullSimHeaderAlg::select_first_hit()
{
    if (not m_simevent) {
        LogError << "Can't Load SimEvent" << std::endl;
        return false;
    }
    int cnt_lost_pe = 0;

    // TODO: in previous impl, all the info are merged. 
    // Tao Lin. 

    // const std::vector<JM::SimPMTHit*>* vhits[2]; 
    // vhits[0] = &(m_simevent->getCDHitsVec()); 
    // vhits[1] = &(m_simevent->getWPHitsVec()); 

    const auto& hits = m_simevent->getCDHitsVec();

    // for(int iarray = 0; iarray<2; iarray++){
    // std::vector<JM::SimPMTHit*>::const_iterator it = vhits[iarray]->begin(); 
    for(auto it = hits.begin(); it!=hits.end(); it++){ 

        auto hit = *it; 
        JM::CalibPmtChannel* pmt_hdr = 0;
        // get pmt id from sim
        auto pmtid = hit->getPMTID();
        auto hittime = hit->getHitTime();
        auto npe = hit->getNPE();

        int newnpe = hit_or_not(pmtid, npe);
        cnt_lost_pe += (npe - newnpe);

        if (newnpe == 0) {
            continue;
        }
        npe = newnpe;

        if (m_cache_first_hits.find(pmtid) == m_cache_first_hits.end()) {
            bool isCD = false;
            bool isWP = false;
            // filter the pmtid
            if (PMT::IsCD(pmtid)) {
                // 20inch PMT in CD
                // 3inch PMT in CD
                isCD = true;
            } else if (PMT::IsWP(pmtid)) {
                // 20inch PMT in WP
                isWP = true;
            } else {
                continue;
            }
            // don't find it, create new one
            //
	    std::vector<float> times_cache; 		// vectors to hold hit times and corresponding nPE
	    std::vector<float> charge_cache;
	    times_cache.push_back(hittime);		// push current values into vectors
	    charge_cache.push_back((float)npe);
            pmt_hdr = new JM::CalibPmtChannel;
            if (isCD) {
                pmt_hdr->setPmtId(CdID::id(pmtid, 0)); 
            } else if (isWP) {
                pmt_hdr->setPmtId(WpID::id(pmtid, 0)); 
            }
            pmt_hdr->setFirstHitTime(hittime);
            pmt_hdr->setNPE(npe);
	    pmt_hdr->setTime(times_cache);		// add vectors to CalibPMTChannel entry for pmtId
	    pmt_hdr->setCharge(charge_cache);
	    //

            m_cache_first_hits[pmtid] = pmt_hdr;
            continue;
        } 
        pmt_hdr = m_cache_first_hits[pmtid];

        // update the nPE in that pmt
        pmt_hdr->setNPE( pmt_hdr->nPE() + npe );
        // find the first hit;
        if ( hittime < pmt_hdr->firstHitTime() ) {
            pmt_hdr->setFirstHitTime(hittime);
        }
	//
	auto times_cache = pmt_hdr->time();	// for each hit retrieve vectors according to pmtId
	times_cache.push_back(hittime);				// and push new values in
	pmt_hdr->setTime(times_cache);				// put filled (+1) vector back into CalibPmtChannel
	auto charge_cache = pmt_hdr->charge();	// same for charge
	charge_cache.push_back(npe);
	pmt_hdr->setCharge(charge_cache);
	// TODO: check if sorting is needed later. At the moment no sorting needed, because hit collection is sorted by hit time.
	//
    }
    // }

    LogDebug << "Total PMT Headers: " << m_cache_first_hits.size() 
             << std::endl;
    LogDebug << "Total Lost PEs: " << cnt_lost_pe
             << std::endl;
    return true;
}

int 
PullSimHeaderAlg::hit_or_not(int pmtid, int npe)
{
    if (m_qe_scale == 1.0 and (not m_flag_smear_qe)) {
        // don't do any sample.
        return npe;
    }
    // sample the hit again
    // - use set qe scale
    // - or, load qe scale from file
    double qe_scale = m_qe_scale;
    if (m_flag_smear_qe) {
        // look for the PMT efficiency
        if (m_pmt2efficiency.find(pmtid)!=m_pmt2efficiency.end()) {
            qe_scale = m_pmt2efficiency[pmtid];
        } else {
            LogWarn << "Can't find the efficiency for PMT [" 
                << pmtid << "]" << std::endl;
        }
    }

    int newnpe = 0;
    int cnt_lost_pe = 0;
    for (int i = 0; i < npe; ++i) {
        if ( m_randomsvc->random() < qe_scale) {
            // hit
            newnpe += 1;
        } else {
            // lost 
            cnt_lost_pe += 1;
        }
    }
    if (cnt_lost_pe) {
        LogDebug << "PMTID [" << pmtid << "] lost hits: " << cnt_lost_pe << std::endl;
    }
    // end sample the hit

    assert((newnpe+cnt_lost_pe) == npe);
    return newnpe;
}
