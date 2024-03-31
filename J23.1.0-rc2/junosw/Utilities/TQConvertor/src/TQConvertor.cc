#include "TQConvertor/TQConvertor.h"
#include "SniperKernel/SniperLog.h"

#include <functional>

TQConvertor::TQConvertor(){
}

TQConvertor::~TQConvertor(){
}

uint32_t TQConvertor::t_flt2uint(float t_flt){
    return (uint32_t)(10000*(t_flt+300));
}

float TQConvertor::t_uint2flt(uint32_t t_uint){
    return t_uint/10000.-300;
}

uint32_t TQConvertor::q_flt2uint(float q_flt){
    return (uint32_t)(10000*q_flt);
}

float TQConvertor::q_uint2flt(uint32_t q_uint){
    return q_uint/10000.;
}

JM::CdLpmtElecHeader* TQConvertor::convertTQ(JM::CdLpmtCalibHeader* header_calib_cdltq, TQSRC tqsrc){
    // = Create the CdLpmtElecHeader, and fill T/Q into it =
    if (header_calib_cdltq==NULL){
        LogWarn<<"CalibHeader for CD large PMT is not found! T/Q is not known!"<<std::endl;
        return NULL;
    }
    if (tqsrc!=WFREC && tqsrc!=FPGA){
        LogWarn<<"Unknow source for tq! Use WFREC as default!"<<std::endl;
        tqsrc = WFREC;
    }
    JM::CdLpmtElecEvt* evt_elec_cdltq = new JM::CdLpmtElecEvt();
    JM::CdLpmtCalibEvt* evt_calib_cdltq = header_calib_cdltq->event();
    const auto& chhlist = evt_calib_cdltq->calibPMTCol();
    std::function<uint32_t(float)> f_t;
    std::function<uint32_t(float)> f_q;
    if (tqsrc==WFREC){
        f_t = [this](float t_flt){return this->t_flt2uint(t_flt);};
        f_q = [this](float q_flt){return this->q_flt2uint(q_flt);};
    }
    else if (tqsrc==FPGA){
        // = TODO: Dummy, to be changed in the future =
        f_t = [this](float t_flt){return this->t_flt2uint(t_flt);};
        f_q = [this](float q_flt){return this->q_flt2uint(q_flt);};
    }
    for (auto it=chhlist.begin();it!=chhlist.end();it++){//create channel
        int chid = (*it)->pmtId();
        JM::ElecChannel* thischannel = evt_elec_cdltq->addChannel(chid);
        // = set charge and time =
        std::vector<unsigned int> thecharge;
        for (auto ait : (*it)->charge()){//charge
            thecharge.push_back(f_q(ait));
        }
        thischannel->setCharge(thecharge);
        std::vector<unsigned int> thetime;
        for (auto ait : (*it)->time()){//time
            thetime.push_back(f_t(ait));
        }
        thischannel->setTime(thetime);
    }

    JM::CdLpmtElecHeader* header_elec_cdltq = new JM::CdLpmtElecHeader();
    header_elec_cdltq->setEvent(evt_elec_cdltq);
    return header_elec_cdltq;
}

JM::CdSpmtElecHeader* TQConvertor::convertTQ(JM::CdSpmtCalibHeader* header_calib_cdstq, TQSRC tqsrc){
    // = Create the CdSpmtElecHeader, and fill T/Q into it
    if (header_calib_cdstq==NULL){
        LogWarn<<"CalibHeader for CD small PMT is not found! T/Q is not known!"<<std::endl;
        return NULL;
    }
    if (tqsrc!=WFREC && tqsrc!=FPGA){
        LogWarn<<"Unknow source for tq! Use WFREC as default!"<<std::endl;
        tqsrc = WFREC;
    }
    JM::CdSpmtElecEvt* evt_elec_cdstq = new JM::CdSpmtElecEvt();
    JM::CdSpmtCalibEvt* evt_calib_cdstq = header_calib_cdstq->event();
    const auto& chhlist = evt_calib_cdstq->calibPMTCol();
    std::function<uint32_t(float)> f_t;
    std::function<uint32_t(float)> f_q;
    if (tqsrc==WFREC){
        f_t = [this](float t_flt){return this->t_flt2uint(t_flt);};
        f_q = [this](float q_flt){return this->q_flt2uint(q_flt);};
    }
    else if (tqsrc==FPGA){
        // = TODO: Dummy, to be changed in the future =
        f_t = [this](float t_flt){return this->t_flt2uint(t_flt);};
        f_q = [this](float q_flt){return this->q_flt2uint(q_flt);};
    }
    for (auto it=chhlist.begin();it!=chhlist.end();it++){//create channel
        int chid = (*it)->pmtId();
        JM::ElecChannel* thischannel = evt_elec_cdstq->addChannel(chid);
        //set charge and time
        std::vector<unsigned int> thecharge;
        for (auto ait : (*it)->charge()){//charge
            thecharge.push_back(f_q(ait));
        }
        thischannel->setCharge(thecharge);
        std::vector<unsigned int> thetime;
        for (auto ait : (*it)->time()){//time
            thetime.push_back(f_t(ait));
        }
        thischannel->setTime(thetime);
    }

    JM::CdSpmtElecHeader* header_elec_cdstq = new JM::CdSpmtElecHeader();
    header_elec_cdstq->setEvent(evt_elec_cdstq);
    return header_elec_cdstq;
}

JM::WpElecHeader* TQConvertor::convertTQ(JM::WpCalibHeader* header_calib_wptq, TQSRC tqsrc){
    if (header_calib_wptq==NULL){
        LogWarn<<"CalibHeader for WP is not found! T/Q is not known!"<<std::endl;
        return NULL;
    }
    if (tqsrc!=WFREC && tqsrc!=FPGA){
        LogWarn<<"Unknow source for tq! Use WFREC as default!"<<std::endl;
        tqsrc = WFREC;
    }
    JM::WpElecEvt* evt_elec_wptq = new JM::WpElecEvt();
    JM::WpCalibEvt* evt_calib_wptq = header_calib_wptq->event();
    const auto& chhlist = evt_calib_wptq->calibPMTCol();
    std::function<uint32_t(float)> f_t;
    std::function<uint32_t(float)> f_q;
    if (tqsrc==WFREC){
        f_t = [this](float t_flt){return this->t_flt2uint(t_flt);};
        f_q = [this](float q_flt){return this->q_flt2uint(q_flt);};
    }
    else if (tqsrc==FPGA){
        // = TODO: Dummy, to be changed in the future =
        f_t = [this](float t_flt){return this->t_flt2uint(t_flt);};
        f_q = [this](float q_flt){return this->q_flt2uint(q_flt);};
    }
    for (auto it=chhlist.begin();it!=chhlist.end();it++){//create channel
        int chid = (*it)->pmtId();
        JM::ElecChannel* thischannel =evt_elec_wptq->addChannel(chid);
        //set charge and time
        std::vector<unsigned int> thecharge;
        for (auto ait : (*it)->charge()){//charge
            thecharge.push_back(f_q(ait));
        }
        thischannel->setCharge(thecharge);
        std::vector<unsigned int> thetime;
        for (auto ait : (*it)->time()){//time
            thetime.push_back(f_t(ait));
        }
        thischannel->setTime(thetime);
    }
    
    JM::WpElecHeader* header_elec_wptq = new JM::WpElecHeader();
    header_elec_wptq->setEvent(evt_elec_wptq);
    return header_elec_wptq;
}

JM::TtElecHeader* TQConvertor::convertTQ(JM::TtCalibHeader* header_calib_tttq, TQSRC tqsrc){
    if (header_calib_tttq==NULL){
        LogWarn<<"CalibHeader for TT is not found! T/Q is not known!"<<std::endl;
        return NULL;
    }
    if (tqsrc!=WFREC && tqsrc!=FPGA){
        LogWarn<<"Unknow source for tq! Use WFREC as default!"<<std::endl;
        tqsrc = WFREC;
    }
    JM::TtElecEvt* evt_elec_tttq = new JM::TtElecEvt();
    JM::TtCalibEvt* evt_calib_tttq = header_calib_tttq->event();
    const auto& chhlist = evt_calib_tttq->calibTtCol();
    std::function<uint32_t(float)> f_t;
    std::function<uint32_t(float)> f_q;
    if (tqsrc==WFREC){
        f_t = [this](float t_flt){return this->t_flt2uint(t_flt);};
        f_q = [this](float q_flt){return this->q_flt2uint(q_flt);};
    }
    else if (tqsrc==FPGA){
        // = TODO: Dummy, to be changed in the future =
        f_t = [this](float t_flt){return this->t_flt2uint(t_flt);};
        f_q = [this](float q_flt){return this->q_flt2uint(q_flt);};
    }
    for (auto it=chhlist.begin();it!=chhlist.end();it++){//create channel
        int chid = (*it)->pmtId();
        JM::ElecChannel* thischannel = evt_elec_tttq->addChannel(chid);
        //set charge and time
        std::vector<unsigned int> thecharge;
        thecharge.push_back(f_q((*it)->nPE()));
        thischannel->setCharge(thecharge);
        std::vector<unsigned int> thetime;
        thetime.push_back(f_t((*it)->time()));
        thischannel->setTime(thetime);
    }

    JM::TtElecHeader* header_elec_tttq = new JM::TtElecHeader();
    header_elec_tttq->setEvent(evt_elec_tttq);
    return header_elec_tttq;
}

JM::CdLpmtCalibHeader* TQConvertor::convertTQ(JM::CdLpmtElecHeader* header_elec_cdltq, TQSRC tqsrc){
    if (header_elec_cdltq==NULL){
        LogWarn<<"ElecHeader for CD large PMT is not found! T/Q is not known!"<<std::endl;
        return NULL;
    }
    if (tqsrc!=WFREC && tqsrc!=FPGA){
        LogWarn<<"Unknow source for tq! Use WFREC as default!"<<std::endl;
        tqsrc = WFREC;
    }
    JM::CdLpmtCalibEvt* evt_calib_cdltq = new JM::CdLpmtCalibEvt();
    JM::CdLpmtElecEvt* evt_elec_cdltq = header_elec_cdltq->event();    
    const auto& map_TQchannel = evt_elec_cdltq->channelData(); // Map between PmtId and ElecChannel 
    std::function<float(uint32_t)> f_t;
    std::function<float(uint32_t)> f_q;
    if (tqsrc==WFREC){
        f_t = [this](uint32_t t_uint){return this->t_uint2flt(t_uint);};
        f_q = [this](uint32_t q_uint){return this->q_uint2flt(q_uint);};
    }
    else if (tqsrc==FPGA){
        // = TODO: Dummy, to be changed in the future =
        f_t = [this](uint32_t t_uint){return this->t_uint2flt(t_uint);};
        f_q = [this](uint32_t q_uint){return this->q_uint2flt(q_uint);};
    }
    for (auto it = map_TQchannel.begin(); it != map_TQchannel.end(); ++it) {
        auto& channel = *(it->second);
        //if (channel.charge().size() == 0) {
        //    continue;
        //}
        unsigned int chid = it->first;
        JM::CalibPmtChannel* thischannel = evt_calib_cdltq->addCalibPmtChannel(chid);
        // = set charge =
        std::vector<float> thecharge;
        float nPE = 0;
        for (unsigned int ait : channel.charge()){
            float q = f_q(ait);
            thecharge.push_back(q);  
            nPE += q;
        }
        thischannel->setCharge(thecharge);        
        thischannel->setNPE(nPE);

        // = set time =
        std::vector<float> thetime;    
        float firstHitTime = 1e9;                   
        for (unsigned int ait : channel.time()){
            float t = f_t(ait);
            thetime.push_back(t);
            if(t<firstHitTime) firstHitTime = t;
        }
        if(thetime.size()<1) firstHitTime = 0;
        thischannel->setTime(thetime);
        thischannel->setFirstHitTime(firstHitTime);
    }

    JM::CdLpmtCalibHeader* header_calib_cdltq = new JM::CdLpmtCalibHeader();
    header_calib_cdltq->setEvent(evt_calib_cdltq);
    return header_calib_cdltq;
}

JM::CdSpmtCalibHeader* TQConvertor::convertTQ(JM::CdSpmtElecHeader* header_elec_cdstq, TQSRC tqsrc){
    if (header_elec_cdstq==NULL){
        LogWarn<<"ElecHeader for CD small PMT is not found! T/Q is not known!"<<std::endl;
        return NULL;
    }
    if (tqsrc!=WFREC && tqsrc!=FPGA){
        LogWarn<<"Unknow source for tq! Use WFREC as default!"<<std::endl;
        tqsrc = WFREC;
    }
    JM::CdSpmtCalibEvt* evt_calib_cdstq = new JM::CdSpmtCalibEvt();
    JM::CdSpmtElecEvt* evt_elec_cdstq = header_elec_cdstq->event();    
    const auto& map_TQchannel = evt_elec_cdstq->channelData(); // Map between PmtId and ElecChannel 
    std::function<float(uint32_t)> f_t;
    std::function<float(uint32_t)> f_q;
    if (tqsrc==WFREC){
        f_t = [this](uint32_t t_uint){return this->t_uint2flt(t_uint);};
        f_q = [this](uint32_t q_uint){return this->q_uint2flt(q_uint);};
    }
    else if (tqsrc==FPGA){
        // = TODO: Dummy, to be changed in the future =
        f_t = [this](uint32_t t_uint){return this->t_uint2flt(t_uint);};
        f_q = [this](uint32_t q_uint){return this->q_uint2flt(q_uint);};
    }
    for (auto it = map_TQchannel.begin(); it != map_TQchannel.end(); ++it){
        auto& channel = *(it->second);
        //if (channel.charge().size() == 0) {
        //    continue;
        //}
        unsigned int chid = it->first;
        JM::CalibPmtChannel* thischannel = evt_calib_cdstq->addCalibPmtChannel(chid);
        // = set charge =
        std::vector<float> thecharge;
        float nPE = 0;
        for (unsigned int ait : channel.charge()){
            float q = f_q(ait);
            thecharge.push_back(q);  
            nPE += q;
        }
        thischannel->setCharge(thecharge);        
        thischannel->setNPE(nPE);

        // = set time =
        std::vector<float> thetime;    
        float firstHitTime = 1e9;                   
        for (unsigned int ait : channel.time()){
            float t = f_t(ait);
            thetime.push_back(t);
            if(t<firstHitTime) firstHitTime = t;
        }
        if(thetime.size()<1) firstHitTime = 0;
        thischannel->setTime(thetime);
        thischannel->setFirstHitTime(firstHitTime);
    }

    JM::CdSpmtCalibHeader* header_calib_cdstq = new JM::CdSpmtCalibHeader();
    header_calib_cdstq->setEvent(evt_calib_cdstq);
    return header_calib_cdstq;
}

JM::WpCalibHeader* TQConvertor::convertTQ(JM::WpElecHeader* header_elec_wptq, TQSRC tqsrc){
    if (header_elec_wptq==NULL){
        LogWarn<<"ElecHeader for WP PMT is not found! T/Q is not known!"<<std::endl;
        return NULL;
    }
    if (tqsrc!=WFREC && tqsrc!=FPGA){
        LogWarn<<"Unknow source for tq! Use WFREC as default!"<<std::endl;
        tqsrc = WFREC;
    }
    JM::WpCalibEvt* evt_calib_wptq = new JM::WpCalibEvt();
    JM::WpElecEvt* evt_elec_wptq = header_elec_wptq->event();    
    const auto& map_TQchannel = evt_elec_wptq->channelData(); // Map between PmtId and ElecChannel 
    std::function<float(uint32_t)> f_t;
    std::function<float(uint32_t)> f_q;
    if (tqsrc==WFREC){
        f_t = [this](uint32_t t_uint){return this->t_uint2flt(t_uint);};
        f_q = [this](uint32_t q_uint){return this->q_uint2flt(q_uint);};
    }
    else if (tqsrc==FPGA){
        // = TODO: Dummy, to be changed in the future =
        f_t = [this](uint32_t t_uint){return this->t_uint2flt(t_uint);};
        f_q = [this](uint32_t q_uint){return this->q_uint2flt(q_uint);};
    }
    for (auto it = map_TQchannel.begin(); it != map_TQchannel.end(); ++it){
        auto& channel = *(it->second);
        //if (channel.charge().size() == 0) {
        //    continue;
        //}
        unsigned int chid = it->first;
        JM::CalibPmtChannel* thischannel = evt_calib_wptq->addCalibPmtChannel(chid);
        // = set charge =
        std::vector<float> thecharge;
        float nPE = 0;
        for (unsigned int ait : channel.charge()){
            float q = f_q(ait);
            thecharge.push_back(q);  
            nPE += q;
        }
        thischannel->setCharge(thecharge);        
        thischannel->setNPE(nPE);

        // = set time =
        std::vector<float> thetime;    
        float firstHitTime = 1e9;                   
        for (unsigned int ait : channel.time()){
            float t = f_t(ait);
            thetime.push_back(t);
            if(t<firstHitTime) firstHitTime = t;
        }
        if(thetime.size()<1) firstHitTime = 0;
        thischannel->setTime(thetime);
        thischannel->setFirstHitTime(firstHitTime);
    }

    JM::WpCalibHeader* header_calib_wptq = new JM::WpCalibHeader();
    header_calib_wptq->setEvent(evt_calib_wptq);
    return header_calib_wptq;
}

JM::TtCalibHeader* TQConvertor::convertTQ(JM::TtElecHeader* header_elec_tttq, TQSRC tqsrc){
    if (header_elec_tttq==NULL){
        LogWarn<<"ElecHeader for TT PMT is not found! T/Q is not known!"<<std::endl;
        return NULL;
    }
    if (tqsrc!=WFREC && tqsrc!=FPGA){
        LogWarn<<"Unknow source for tq! Use WFREC as default!"<<std::endl;
        tqsrc = WFREC;
    }
    JM::TtCalibEvt* evt_calib_tttq = new JM::TtCalibEvt();
    JM::TtElecEvt* evt_elec_tttq = header_elec_tttq->event();    
    const auto& map_TQchannel = evt_elec_tttq->channelData(); // Map between PmtId and ElecChannel 
    std::function<float(uint32_t)> f_t;
    std::function<float(uint32_t)> f_q;
    if (tqsrc==WFREC){
        f_t = [this](uint32_t t_uint){return this->t_uint2flt(t_uint);};
        f_q = [this](uint32_t q_uint){return this->q_uint2flt(q_uint);};
    }
    else if (tqsrc==FPGA){
        // = TODO: Dummy, to be changed in the future =
        f_t = [this](uint32_t t_uint){return this->t_uint2flt(t_uint);};
        f_q = [this](uint32_t q_uint){return this->q_uint2flt(q_uint);};
    }
    for (auto it = map_TQchannel.begin(); it != map_TQchannel.end(); ++it){
        auto& channel = *(it->second);
        unsigned int chid = it->first;
        JM::CalibTtChannel* thischannel = evt_calib_tttq->addCalibTtChannel(chid);
        if (channel.charge().size() == 0) {
            continue;
        }
        // = set charge and time=
        float thecharge = channel.charge().at(0);
        thischannel->setNPE(f_q(thecharge));
        float thetime = channel.time().at(0);
        thischannel->setTime(f_t(thetime));
    }

    JM::TtCalibHeader* header_calib_tttq = new JM::TtCalibHeader();
    header_calib_tttq->setEvent(evt_calib_tttq);
    return header_calib_tttq;
}
