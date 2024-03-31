///////////////////////////////////////
/*
 * Package for time reconstruction for waveforms
 * Author: miaoyu@ihep.ac.cn
 * Oct, 2019
 */
///////////////////////////////////////
#ifndef TimeRec_cc
#define TimeRec_cc
#include "TimeRec.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/Task.h"
#include "RootWriter/RootWriter.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Context/TimeStamp.h"
#include "TTimeStamp.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtElecTruthHeader.h"
#include "Event/CdLpmtCalibHeader.h"

#include "PMTCalibSvc/PMTCalibSvc.h"
#include "TMath.h"
#include "TCanvas.h"

using namespace std;

DECLARE_ALGORITHM(TimeRec);

TimeRec::TimeRec(const string& name): AlgBase(name)
        , m_windowLength(1250)
        , Ball_R(19.250)  //m
        , PMT_R(19.434)   //m                     
        , m_doCorrection(true)
        , m_nlpmt(17621)
        //, m_save(false)
{
    declProp("WindowLength", m_windowLength) ; 
    declProp("AlgOption", algOption);
    declProp("Ball_R", Ball_R);
    declProp("PMT_R", PMT_R);
    declProp("Mode", mode);
    declProp("DoCorrection", m_doCorrection);
    declProp("enableElecTruth", m_elecTruth=false);
}

TimeRec::~TimeRec(){}

bool TimeRec::initialize(){

    //event navigator
    SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
    if(navBuf.invalid()){
        LogError << "cannot get the NavBuffer @ /Event " << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    // user data definition
    SniperPtr<RootWriter> svc(*getRoot(), "RootWriter");
    if (svc.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
            << "enable it in your job option file."
            << std::endl;
        return false;
    }


    LogInfo << "Correct Time Offset : " << m_doCorrection << endl;

    //user defined histograms
    
    // each channel info
    
    //for(int i=0;i<20000;i++){
    //    TString chName1=Form("ch%d_deltaT",i);
    //    deltaT[i] = new TH1D(chName1,chName1, 1000,-200,800);
    //    if(mode == "save") {
    //    svc->attach("CALIBEVT",deltaT[i]); }
    //}
    
     
    recCharge = new TH1D("rec_charge","rec_charge",1000,0,20);
    svc->attach("CALIBEVT",recCharge);
    recFirstCharge = new TH1D("recFirstCharge", "recFirstCharge",1000,0,20);
    svc->attach("CALIBEVT",recFirstCharge);
    time = new TH1D("rectime","rectime",1250,0,1250);
    svc->attach("CALIBEVT",time);
    hist_allhitsTime = new TH1D("allhitsTime","allhitsTime", 1250,0,1250);
    svc->attach("CALIBEVT", hist_allhitsTime);
    time_force = new TH1D("time_force","time_force", 1500, -200, 1300);
    svc->attach("CALIBEVT", time_force);
    thre = new TH1D("threshold", "5 baseline sigma", 250,20,70);
    svc->attach("CALIBEVT",thre);
    basel = new TH1D("baseline", "baseline", 100,90,110);
    svc->attach("CALIBEVT",basel);
    diff_fht = new TH1D("diff_fht","diff_fht",110,-10,100);
    svc->attach("CALIBEVT",diff_fht);



    // reconstruct pmtcalib svc
    SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
    if(calSvc.invalid()) {
        LogError <<  "Failed to get PMTCalibSvc instance!" << std::endl;
        return false;
    }
    // initialize channel correction data
    tOffset = calSvc->getTimeOffset();

    m_calib = svc->bookTree(*m_par, "USER_OUTPUT/CALIBEVT","simple output of calibration data");
    m_calib -> Branch("decon", &m_decon);
    m_calib->Branch("linear", &m_linear);
    m_calib->Branch("adc", &m_adc);
    m_calib->Branch("tdc", &m_tdc);
    
    m_evt = 0;
    count = 0;
    LogInfo << "Initialize successfully! " << endl;
    return true;
}


bool TimeRec::execute(){


    LogDebug << "-----------------------------------" << std::endl;
    m_decon.clear();
    m_linear.clear();

    auto nav = m_buf->curEvt(); 
    if(!nav){
        LogWarn << "can't load event navigator. " << std::endl;
        return dynamic_cast<Task*>(getRoot())->stop();
    
    }
    //TTimeStamp evtTime = nav->TimeStamp();
    //cout << "evt time stamp: " << evtTime.GetSec()*1e9 + evtTime.GetNanoSec() << endl;

    /*****************************/
    // load CalibEvent           //
    /****************************/
    
//    std::list<JM::CalibPMTChannel*> cpcl;//CalibPMTChannel list
//    if(cpcl.size())  cout << "PMTCol Channel read"<< endl;

    // load elecEvent
    LogInfo << " elecEvent has been read !" << endl;
    auto eh = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
    if (!eh) return true;
    if( !eh->hasEvent() ) return true;
    auto ee = eh->event();

    auto chcol = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav); 
    if(!chcol->hasEvent()) return true; //cout << "Read Calib Event successfully!" << endl;
    auto ce = chcol->event();

    // TODO: missing trigger time in crate
    // Tao Lin, 2022.03.08
    TimeStamp triggerTime; // = m_crate->TriggerTime();
    TimeStamp evtTime    ; // = m_crate->EvtTimeStamp();
    double delta_ref = ( triggerTime.GetNanoSec() - evtTime.GetNanoSec() );
    double refT;
    refT = 100 - ( triggerTime.GetNanoSec() - evtTime.GetNanoSec() );

    const auto& feeChannels = ee->channelData();
    
    for(auto it=feeChannels.begin();it!=feeChannels.end();++it){
        const auto& channel = *(it->second);
        if(channel.adc().size() == 0){
            continue;
        }


        vec_time.clear();
        vec_pass_time.clear();
        vec_start_time.clear();
        vec_end_time.clear();
        vec_decon_time.clear();
        
        unsigned int pmtID = it->first;  //cout << pmtID << endl; 
        unsigned int detID = CdID::id(static_cast<unsigned int>(pmtID),0);
        auto cpc = const_cast<JM::CalibPmtChannel*>(ce->getCalibPmtChannel(detID));
        if(!cpc) continue;
        // jump two poles
        //if(pmtID >=0 && pmtID< 21)  continue;
        //if(pmtID >=17592 && pmtID <pmtID<17613) continue;

        // read all hits time from deconvolution
        vec_decon_time = cpc->time();
        for(int ll=0; ll<vec_decon_time.size(); ll++){
            hist_allhitsTime->Fill(vec_decon_time[ll]);
        }

        // calculate baseline and threshold
        double baseline = 0;
        for( int i=0; i<50; i++) {  baseline+=(channel.adc())[i];  }
        baseline /= 50;
        double baseline_sigma = 0;
        for(int k=0;k<50;k++) baseline_sigma += (((channel.adc())[k] -baseline) * ((channel.adc())[k]-baseline)); 
        baseline_sigma /= 49.;    
        baseline_sigma = TMath::Sqrt(baseline_sigma);
        double threshold = 0;
        threshold =  5*baseline_sigma;   

        basel->Fill(baseline);
        thre->Fill(threshold);

        //search pulses
        getPulse(channel, baseline, threshold);
        

        if(vec_pass_time.size() == 0) {
            if(cpc) cpc->setFirstHitTime(0);  // no-passing channel FHT = 0
            
            continue;}
    
        npe =-1; first_npe =-1; firstHitTime=-1;
        npe = getNPE(channel, baseline, threshold);  
        first_npe = getNPE_FH(channel, baseline, threshold);
        if(algOption == "fixed" ) firstHitTime = getFHT_fixed(/*channel,baseline, threshold,*/ firstHit);
        if(algOption == "fit1"  ) firstHitTime = getFHT_fitting1(channel, baseline, threshold, firstHit); 
        if(algOption == "cfd" )   firstHitTime = getFHT_CFD(channel, baseline, threshold, firstHit);
        if(algOption == "peak" )  firstHitTime = getFHT_peakFitting(channel, baseline, threshold, firstHit);


        if(firstHitTime >= 0 && firstHitTime < m_windowLength && npe>=0 && first_npe >=0){
        
        if(m_doCorrection and (pmtID>=0 and pmtID<m_nlpmt))firstHitTime -= tOffset[pmtID];  // time offset corrections

        //time->Fill(vec_pass_time[0]-refT);
        time->Fill(firstHitTime);
        if(vec_pass_time.size() > 1) {
            for(int hh=1; hh<vec_pass_time.size(); hh++){    // record all hits time
                time_force->Fill(vec_pass_time[hh] - refT);
            }
        }
        recCharge->Fill(npe); 
        recFirstCharge->Fill(first_npe);

        //deltaT[pmtID]->Fill(firstHitTime);


        //m_pmtId.push_back(pmtID);
        //m_time.push_back(firstHitTime);

        double firstHitTime_Decon = cpc->firstHitTime();
        cpc->setFirstHitTime(firstHitTime);   // only fill firstHitTime in this alg
        diff_fht->Fill(firstHitTime - firstHitTime_Decon);
        if( firstHitTime > 200 &&  firstHitTime < 250) {   // set firstHitTime window
            m_decon.push_back(firstHitTime_Decon);
            m_linear.push_back(firstHitTime);
            if(count < 100){
            for(int i=0; i<1000; i++){
                m_adc.push_back(channel.adc()[i]);
                m_tdc.push_back(channel.tdc()[i]);
            }  count++;
            }
        }

        }
        else{ 

            cpc->setFirstHitTime(0);
        }
    }

    if(m_elecTruth) {
        auto elec_truth_header = JM::getHeaderObject<JM::CdLpmtElecTruthHeader>(nav);
        nav->addHeader(elec_truth_header);
    }
    m_calib->Fill();

    LogDebug << "End of the PMT channel correction" << std::endl;


    LogInfo << "Done to read CalibPMT " << m_evt <<"th Event! "  << std::endl;
    m_evt++; 
    
    return true;
}

bool TimeRec::finalize(){
    //cc->Print("abnormal.pdf)")
    //pdf->Close();
    

    LogInfo << "Finalize successfully! " << endl;
    return true;
}


/**********************************************************/

void TimeRec::getPulse(const JM::ElecWaveform& channel, double baseline, double threshold){
    const auto& adc_origin = channel.adc();
    const auto& tdc = channel.tdc();
    if(adc_origin.size() == 0 ) return ;
    
    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    
    for(int i=1; i<m_length; i++){ if(adc[i-1]<=threshold && adc[i]>=threshold) vec_pass_time.push_back(i); vec_time.push_back(i);}
    if (vec_pass_time.size() ==0)  return ;
    for(int i=0; i<vec_pass_time.size();i++){
        for(int k=0; k<vec_pass_time[i]; k++){
            if( adc[vec_pass_time[i]-k] <= 0 ) {vec_start_time.push_back(vec_pass_time[i]-k); break;}
            //vec_start_time.push_back(0);
        }
    }

    for(int i=0; i<vec_pass_time.size();i++){
        for(int k=0; k<m_length; k++){
            if( adc[vec_pass_time[i]+k] <= 0 ) {vec_end_time.push_back(vec_pass_time[i]+k); break;}
            //vec_end_time.push_back(m_length);
        }
    }
    return;
}



/*********************************************************/

// get charge for one channel by simple integral
double TimeRec::getNPE(const JM::ElecWaveform& channel, double baseline, double threshold){
    const auto& adc_origin = channel.adc();
    const auto& tdc = channel.tdc();
    if(adc_origin.size() == 0 ) return -1;

    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    
    double spe = 1500.; // nominal value
    double tmp_npe = 0.0;
    for(int i=0; i<vec_start_time.size();i++) {  //cout << "start: " << start[i] << " end: " << end[i] << endl;
        for(int j=vec_start_time[i]; j<vec_end_time[i]; j++) { if(adc[j]>=0) tmp_npe += adc[j];}
         }

    return tmp_npe/spe;

}

/**********************************************************/

// get charge for first hit
double TimeRec::getNPE_FH(const JM::ElecWaveform& channel, double baseline, double threshold){
    const auto& adc_origin = channel.adc();
    const auto& tdc = channel.tdc();
    if(adc_origin.size() == 0 ) return -1;

    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    
    double spe = 1500;  // nominal value

    double tmp_npe = 0.0;
    for(int i=0;i<vec_pass_time.size();i++){
        for(int m=vec_start_time[i]; m<vec_end_time[i]; m++){
        //npe +=  (adc[m]+adc[m+1])/2  ;
            if(adc[m]>=0)  tmp_npe+= adc[m];
        }
        if(tmp_npe/spe < 0.1) { tmp_npe = 0; }  // skip charge<0.1 pulses
        else {firstHit = i;  break;}
    }
    if(tmp_npe == 0) { tmp_npe = -1; firstHit = -1; }  // not found pulse
    return tmp_npe/spe;

}


/**********************************************************/

// fixed threshold for first hit time reconstruction
double TimeRec::getFHT_fixed(/*const JM::ElecWaveform& channel, double baseline, double threshold, */int hitNum){

    if(hitNum>=0) { return vec_pass_time[hitNum]; }
    else {return -1;}
}

/**********************************************************/

// 2-points linear fitting leading egde

double TimeRec::getFHT_fitting1(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum){

    const auto& adc_origin = channel.adc();
    const auto& tdc = channel.tdc();
    if(adc_origin.size() == 0) return 0;

    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    if(hitNum<0) return -1;
    int pass_time = vec_pass_time[hitNum];

    double max = -1;
    int peak_time = 0;
    for(int i=pass_time; i<m_length; i++){   
        bool firstpeak = true;
        if(pass_time > 10){
            for(int k=0;k<21;k++){
                if(adc[i] < adc[i-10+k]) firstpeak=false;
            }
            if(firstpeak) {max = adc[i]; peak_time = tdc[i]; break;}  // only searching first hit peak
        
            }

        else{
            for(int k=0;k<pass_time*2+1;k++){
                if(adc[i]<adc[i-pass_time+k]) firstpeak=false;
            }
            if(firstpeak) {max = adc[i];peak_time = tdc[i];  break;}  // only searching first hit peak
    
        }
    }
    if(max <= 0) return pass_time;  // negative peak value, return
    else{
        double threshold1 = 0.2*max;
        double threshold2 = 0.9*max;
        int t1=0; int t2=0;
    
        for(int j=1; j<peak_time; j++){
            if(adc[peak_time-j] <= threshold2 && adc[peak_time-j+1] > threshold2){
                t2 = tdc[peak_time-j]; break;
            }
        }               

        for(int m=1; m<peak_time;m++){   
            if(adc[peak_time-m] <= threshold1 && adc[peak_time-m+1]>threshold1){
                t1 = tdc[peak_time-m]; break;
            }
        }

        if(threshold1-threshold2 != 0)return ( (threshold1*t2 - threshold2*t1) / (threshold1 - threshold2) );
        else return t1;
    }

}


/*******************************************************************/
// CFD timing methods
double TimeRec::getFHT_CFD(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum){

    const auto& adc_origin = channel.adc();
    const auto& tdc = channel.tdc();
    if(adc_origin.size() == 0) return 0;

    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    if(hitNum<0) return -1;
    int pass_time = vec_pass_time[hitNum];

    double max = -1;
    int peak_time = 0;
    for(int i=pass_time; i<m_length; i++){   
        bool firstpeak = true;
        if(pass_time > 1/*10*/){
            for(int k=0;k<3/*21*/;k++){
                if(adc[i] < adc[i-1/*10*/+k]) firstpeak=false;
            }
            if(firstpeak) {max = adc[i]; peak_time = tdc[i]; break;}  // only searching first hit peak
        
            }

        else{
            for(int k=0;k<pass_time*2+1;k++){
                if(adc[i]<adc[i-pass_time+k]) firstpeak=false;
            }
            if(firstpeak) {max = adc[i];peak_time = tdc[i];  break;}  // only searching first hit peak
    
        }
    }
    if(max <= 0) return pass_time;  // negative peak value, return
    else{
        
        float constfrac = 0.8*max; 
        float tt = -1;
        for(int j=1; j<peak_time; j++){
            if(adc[peak_time-j] <= constfrac && adc[peak_time-j+1] > constfrac){
                tt = tdc[peak_time-j]; break;
            }
        }               
        return tt;
    }
     
}


/*******************************************************************/
// peak fitting methods

double TimeRec::getFHT_peakFitting(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum){


    const auto& adc_origin = channel.adc();
    const auto& tdc = channel.tdc();
    if(adc_origin.size() == 0) return 0;

    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    if(hitNum<0) return -1;
    int pass_time = vec_pass_time[hitNum];

    double max = -1;
    int peak_time = 0;
    for(int i=pass_time; i<m_length; i++){   
        bool firstpeak = true;
        if(pass_time > 1/*10*/){
            for(int k=0;k<3/*21*/;k++){
                if(adc[i] < adc[i-1/*10*/+k]) firstpeak=false;
            }
            if(firstpeak) {max = adc[i]; peak_time = tdc[i]; break;}  // only searching first hit peak
        
            }

        else{
            for(int k=0;k<pass_time*2+1;k++){
                if(adc[i]<adc[i-pass_time+k]) firstpeak=false;
            }
            if(firstpeak) {max = adc[i];peak_time = tdc[i];  break;}  // only searching first hit peak
    
        }
    }  
    if(max <= 0) return pass_time;  // negative peak value, return
    else{
        //cout<< " peakFit time: " <<  ( adc[peak_time-1]*(peak_time-1) + max*peak_time + adc[peak_time+1]*(peak_time+1) ) / (adc[peak_time-1]+ max + adc[peak_time+1]) << endl ;
        return ( adc[peak_time-1]*(peak_time-1) + max*peak_time + adc[peak_time+1]*(peak_time+1) ) / (adc[peak_time-1]+ max + adc[peak_time+1]) ;
    }

}



#endif
