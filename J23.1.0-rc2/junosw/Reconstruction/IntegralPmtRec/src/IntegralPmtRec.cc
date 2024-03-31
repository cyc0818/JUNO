#ifndef IntegralPmtRec_cc
#define IntegralPmtRec_cc

#include "IntegralPmtRec.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"

#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Identifier/CdID.h"
#include <TFile.h>
#include <TTree.h>

using namespace std;

DECLARE_ALGORITHM(IntegralPmtRec);

IntegralPmtRec::IntegralPmtRec(const string& name): AlgBase(name)
{
    declProp("TotalPMT", m_totalPMT);
    declProp("Threshold", m_threshold);
    m_bin = 1.0;
}


IntegralPmtRec::~IntegralPmtRec(){}


bool IntegralPmtRec::initialize()
{
    // read gain file

    SniperPtr<IPMTSimParamSvc> pmtsimsvc(getParent(), "PMTSimParamSvc");
    if (pmtsimsvc.invalid()) {
        LogError << "Failed to get service PMTSimParamSvc" << std::endl;
		return true;
	}
    IPMTSimParamSvc* m_pmtsimsvc;
    m_pmtsimsvc = pmtsimsvc.data();

    for(int i=0;i<m_totalPMT;i++){
        m_gainMap.insert( pair<int,double>(i,m_pmtsimsvc->get_gain(i)) );
    }


    m_memMgr = SniperPtr<IDataMemMgr>(getParent(),"BufferMemMgr").data();

    return true;
}


bool IntegralPmtRec::execute()
{
    LogDebug << "start Integral Pmt Rec" << endl;
    SniperDataPtr<JM::NavBuffer> navBuf(getParent(),"/Event");
    auto nav = navBuf->curEvt();
    auto eh = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
    if (!eh) {
        LogWarn << "No waveform header is found. skip. " << std::endl;
        return true;
    }

    auto ee = (eh->event());

    const auto& feeChannels = ee->channelData();

    list<JM::CalibPmtChannel*> cpcl;//CalibPMTChannel list


    for(auto it=feeChannels.begin();it!=feeChannels.end();++it){

        const auto& channel = *(it->second);

        if(channel.adc().size()==0){
            continue;
        }

        int pmtID = it->first;
        double nPE = getNPE(channel,m_gainMap[pmtID]);
        double firstHitTime = getFHT(channel);

        auto cpc = new JM::CalibPmtChannel;
        cpc->setNPE(nPE);
        unsigned int detID = CdID::id(static_cast<unsigned int>(pmtID),0);
        cpc->setPmtId(detID);
        cpc->setFirstHitTime(firstHitTime);
        cpcl.push_back(cpc);
    }

    auto ce = new JM::CdLpmtCalibEvt;
    //JM::CalibHeader* ch = new JM::CalibHeader;
    ce->setCalibPMTCol(cpcl);
    auto ch = new JM::CdLpmtCalibHeader;
    ch->setEvent(ce);

    // TODO: why a new event navigator is created?
    // Tao Lin, 2022.03.08

    JM::addHeaderObject(nav, ch);

    LogDebug << "end Integral Pmt Rec" << endl;
    return true;
}


bool IntegralPmtRec::finalize()
{
    return true;
}


double IntegralPmtRec::getNPE(const JM::ElecWaveform& channel, double gain)
{
    const auto& adc = channel.adc();
    if(adc.size()==0){
        return 0;
    }
    double temp = 0.0;
    for(auto it=adc.begin();it!=adc.end()-1;it++){   
        if(*it>=0) temp += (*it+*(it+1))*m_bin/2;
    }   
    return temp/gain;
}


double IntegralPmtRec::getFHT(const JM::ElecWaveform& channel)
{
    const auto& adc = channel.adc();
    const auto& tdc = channel.tdc();

    if (adc.size()==0){
        return 0.0;
    }

    for(int i=0; i < adc.size(); ++i){
        if( adc[i] > m_threshold ){
            return tdc[i];
        }
    }
    return 0.0;
}


#endif


