////////////////////////////////
/*
Package for PMT time offset extraction, it reads trigger type to only use LED calibration data. A simple waveform integral algorithm is used as input of time-amplitude corrections while timing and also as input of event-level reconstruction for event selection(charge-center).
For each run, a set of corretion parameters will be extracted and repeat the whole process until the parameters fixed. Then a set of parameters will be saved into a user-root-file.
Final comparing and combination of all runs will be implemented on a independent C++ script in share folder.
Author: miaoyu@lxslc6.ihep.ac.cn
Sep, 2019
 */
////////////////////////////////

#ifndef TimeCorrAlg_cc
#define TimeCorrAlg_cc
#include <boost/python.hpp>
#include <boost/filesystem.hpp>
#include "TimeCorrAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/Task.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "Event/CdWaveformHeader.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include "RootWriter/RootWriter.h"
#include "TROOT.h"
#include <TFile.h>
#include <TTree.h>
#include <vector>
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TF1.h"
#include "TRandom.h"
#include "Context/TimeStamp.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "Event/CdLpmtCalibHeader.h"
//#include "Event/CalibEvent.h"
//#include "Event/CalibPMTChannel.h"
std::ostream& operator<<(std::ostream& os, const std::vector<double>& v) {
    os << "[";
    for (std::vector<double>::const_iterator it = v.begin();
            it != v.end(); ++it) {
        os << *it << ",";
    }
    os << "]";
    return os;
}

using namespace std;

DECLARE_ALGORITHM(TimeCorrAlg);

TimeCorrAlg::TimeCorrAlg(const string& name) : AlgBase(name)
            , m_totalPMT(17612)
            , m_length(1250)
            , Ball_R(19.316)
            , PMT_R(19.5)
            , LS_R(17.7)
            , ChaCenRec_ratio(1.2)
            , hitsOption("all")
            , forceTriggerSigma(0)
            //, option("cfd")                      
{
    declProp("TotalPMT", m_totalPMT);
    declProp("Length", m_length);
    declProp("Ball_R", Ball_R);
    declProp("PMT_R", PMT_R);
    declProp("option", option);
    declProp("trigger", trigger);
    declProp("Positions", m_particlePositions);
    declProp("hitsOption", hitsOption);
    declProp("forceTriggerSigma", forceTriggerSigma);
    peak_time = -1;
    m_evt = 0;
}




TimeCorrAlg::~TimeCorrAlg(){}



bool TimeCorrAlg::initialize(){
    
    spescale = 770; // based on charge spectrum fitting ...

    // event navigator
    SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
    if( navBuf.invalid()) {
        LogError << "cannot get the NavBuffer @ /Event " << std::endl;
        return false;
    }
    m_buf = navBuf.data();


    // user data definitions
    SniperPtr<RootWriter> m_rw(*getParent(), "RootWriter");
    if( ! m_rw.valid() ) {
        LogError << "Failed to get RootWriter instance!" 
                 << "If you want to use it, pelase enable it in your job option file. "
                 << std::endl;
        return false;   
    }



    if(m_totalPMT > 20000){
        LogError << "Only CD 20inch PMT could be processed. " << endl;
        return false;
    }

    // user defined histograms

    for(int i=0;i<20000;i++){
        TString chName1=Form("ch%d_deltaT",i);
        deltaT[i] = new TH1D(chName1,chName1,BIN_NUM, MIN_BIN, MAX_BIN);
        m_rw->attach("CALIBEVT",deltaT[i]);
    }

/*
    // histograms for QT corrections
    for(int i=0; i<30;i++){
        TString chName1 = Form("charge%d_hama",i);
        time1hist[i]=new TH1D(chName1, chName1, 500,0,500);
        m_rw->attach("CALIBEVT",time1hist[i]);
        TString chName2 = Form("charge%d_nnvt", i);
        time2hist[i] = new TH1D(chName2, chName2, 500,0,500);
        m_rw->attach("CALIBEVT",time2hist[i]);
    }
*/

    // record several basic quantities: 
    LogInfo << "Register ROOt File" << std::endl; 

    m_corr = m_rw->bookTree(*m_par, "USER_OUTPUT/TIMECORR", "time offset calibration");
    m_corr->Branch("evtID",&m_evt);
    m_corr -> Branch("nPE",m_nPE, "nPE[20000]/D");
    //m_corr -> Branch("nPEforFH",m_nPEforFH, "nPEforFH[20000]/D");
    m_corr->Branch("baseline", m_baseline,"baseline[20000]/D");
    m_corr->Branch("threshold",m_threshold,"threshold[20000]/D");
    m_corr->Branch("first_charge", &m_charge);
    m_corr->Branch("refTime", &m_reftime);  // forceTriggerTime-globalTriggerTime
    m_corr->Branch("recTime", &m_time);


    // reconstruction geometry service
    SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        return false;
    }
    m_cdGeom = rgSvc->getCdGeom(); 


    //// Get the geometry service for all PMT;
    // num_PMT = m_cdGeom->findPmtNum();
    num_PMT = m_cdGeom->findPmt20inchNum(); 
    if(num_PMT == m_totalPMT) LogInfo << "Total PMT:" << num_PMT << std::endl;
    else
        LogError << "pmt number initialization is wrong! " << std::endl;
    for(int i=0;i<num_PMT; i++){
        unsigned int all_pmt_id = (unsigned int) i;
        Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
        PmtGeom *all_pmt = m_cdGeom->getPmt(all_id);
        if( !all_pmt ){
            LogError << "Wrong Pmt Id " << i <<std::endl;
        }
        TVector3 all_pmtCenter = all_pmt->getCenter();
        ALL_PMT_pos.push_back(Ball_R/PMT_R*all_pmtCenter);

    }

    SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
    if(calSvc.invalid()) {
        LogError <<  "Failed to get PMTCalibSvc instance!" << std::endl;
        return false;
    }
    
    relaDE = calSvc->getRelativeDE();
    gain = calSvc->getGain();
    toffset = calSvc->getTimeOffset();
    darkrate = calSvc->getDarkRate();


    // tof calculation
    std::vector<double>& calib_pos = m_particlePositions[0];
    double ref_tof = CalculateTOF(0, calib_pos[0], calib_pos[1], calib_pos[2]);
    //cout << "ref_tof: " << ref_tof << endl;
    for(int k=0; k<num_PMT; k++){
        relativeTOF[k] = CalculateTOF(k, calib_pos[0], calib_pos[1], calib_pos[2]) - ref_tof;
        //cout << "PMT channel " << k << " TOF is " << relativeTOF[k] << endl;
    }


    LogInfo << "initialized successfully ! " << std::endl;

    return true;
}

bool TimeCorrAlg::execute(){

    LogDebug << "start PMT time offset extraction" << endl;
    tOffset.clear();
    
    auto nav = m_buf->curEvt();
    if(!nav){
        LogWarn << "can't load event navigator. " << std::endl;
        return dynamic_cast<Task*>(getRoot())->stop();
    
    }
    m_charge.clear();
    m_time.clear();


    double t1 = -1; double t2 =-1;
    double t3 = -1; double t4 =-1;

    auto eh = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
    if( !eh->hasEvent() ) return true;
    auto ee = dynamic_cast<JM::CdWaveformEvt*>(eh->event()); // get the CdWaveformEvt
    const auto& waveforms = ee->channelData(); // get the map of ElecWaveform

    // TODO: should find a place to put EvtTimeStamp and TriggerTime
    // Tao Lin, 2022.03.08
    TimeStamp evtTime     ; //= m_crate->EvtTimeStamp();
    TimeStamp triggerTime ; //= m_crate->TriggerTime();

    double refT = 100 - (triggerTime.GetNanoSec() - evtTime.GetNanoSec());
    //m_reftime = (triggerTime.GetNanoSec() - evtTime.GetNanoSec())-220;
    m_reftime = 100 - (triggerTime.GetNanoSec() - evtTime.GetNanoSec());
    m_reftime = gRandom->Gaus(m_reftime, forceTriggerSigma);
    

    for(auto it=waveforms.begin();it!=waveforms.end();++it){
        const auto& channel = *(it->second);
        if(channel.adc().size() == 0){
            continue;
        }
        vec_time.clear();
        vec_pass_time.clear();
        //vec_fall_time.clear();
        vec_start_time.clear();
        vec_end_time.clear();

        int pmtID = it->first;
        unsigned int detID = CdID::id(static_cast<unsigned int>(pmtID),0);
        
        double baseline =0; double baseline_sigma = 0;
        for(int i=0;i<50;i++) baseline+=(channel.adc())[i]; 
        baseline /= 50.;  
        for(int k=0;k<50;k++) baseline_sigma += (((channel.adc())[k] -baseline) * ((channel.adc())[k]-baseline)); 
        baseline_sigma /= 49.;    
        baseline_sigma = TMath::Sqrt(baseline_sigma);

        threshold = 0;
        threshold = 5*baseline_sigma;   


        // check baseline and threshold
        //if( baseline>cut_baseline && threshold>cut_threshold){
        //    baseline =  nominal_baseline;
        //    threshold = nominal_threshold;
        //}


        //search pulses
        getPulse(channel, baseline, threshold);
        //no pulses found in this channel, continue to read next channel
        if(vec_pass_time.size() == 0) {  continue;}

        npe =-1; first_npe =-1; firstHitTime=-1;
        npe = getNPE(channel, baseline, threshold, pmtID);  
        first_npe = getNPE_FH(channel, baseline, threshold, pmtID);
        if(option == "fixed" ) firstHitTime = getFHT_fixed(channel, baseline, threshold, firstHit);
        if(option == "linear") firstHitTime = getFHT_linear(channel, baseline, threshold, firstHit); 
        if(option == "fit1" )  firstHitTime = getFHT_linearFit(channel, baseline, threshold, firstHit, 0); 
        if(option == "cfd"  )  firstHitTime = getFHT_CFD(channel, baseline, threshold, firstHit);
        if(option == "peak")   firstHitTime = getFHT_peakFitting(channel, threshold, baseline, firstHit); 

        // dark noise exclude time window: 180~300ns
        //**************************/
        //.......
        //**************************/
        

        if(firstHitTime >= 0 && firstHitTime < m_length && npe>=0 && first_npe >=0){
            m_nPE[pmtID] = npe;
            m_baseline[pmtID] = baseline;
            m_threshold[pmtID] = threshold;
        
            // use force trigger mode 
            if(trigger == "force") { firstHitTime -= m_reftime; }
            // correct time of flight
            firstHitTime -= relativeTOF[pmtID];
               
            if( hitsOption=="all" ) {   // Use all hits to obtain offset info
                deltaT[pmtID] -> Fill(firstHitTime);
            } else if ( hitsOption=="1PE") {  // Only use approx 1p.e. hits to obtain offset info
                if( npe<1.5) deltaT[pmtID]->Fill(firstHitTime);
            }
        
            m_time.push_back(firstHitTime);
            m_charge.push_back(first_npe);

        }
        else {continue;}
    }

    /******************************/
    // vertex selection
    TimeCorrAlg::ChargeCenterRec();
    ChaCenRec_x *= ChaCenRec_ratio;
    ChaCenRec_y *= ChaCenRec_ratio;
    ChaCenRec_z *= ChaCenRec_ratio;

    std::cout<<"ChaCenRec:("<<ChaCenRec_x<<","<<ChaCenRec_y<<","<<ChaCenRec_z<<")"<<std::endl;
    if( TMath::Sqrt(ChaCenRec_x*ChaCenRec_x + ChaCenRec_y*ChaCenRec_y + ChaCenRec_z*ChaCenRec_z) > 2000.)  
    {LogInfo << "Out of range, discard! " << endl; return true; }  // set 2.m cut for event selection

    LogInfo << m_evt << "th" << " Good event! ===>  Done to read PMT waveforms " << std::endl;
    m_evt++;

    m_corr->Fill();

    return true;
}

bool TimeCorrAlg::finalize(){

    return true;
}



/*************************************************/
// get NPE by simply integral
double TimeCorrAlg::getNPE(const JM::ElecWaveform& channel, double baseline, double threshold, int pmtid){
    const auto& adc_origin = channel.adc();
    const auto& tdc = channel.tdc();
    if(adc_origin.size() == 0 ) return -1;

    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    double tmp_npe = 0.0;
    for(int i=0; i<vec_start_time.size();i++) {  //cout << "start: " << start[i] << " end: " << end[i] << endl;
        for(int j=vec_start_time[i]; j<vec_end_time[i]; j++) { if(adc[j]>=0) tmp_npe += adc[j];}
         }
    return tmp_npe/spescale;

}
         
/***********************************************************/
// get firstHit NPE by simply integral
double TimeCorrAlg::getNPE_FH(const JM::ElecWaveform& channel, double baseline, double threshold, int pmtid){
    const auto& adc_origin = channel.adc();
    const auto& tdc = channel.tdc();
    if(adc_origin.size() == 0 ) return -1;

    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    
    double spe = gain[pmtid];
    //double spe = 1459;
    double tmp_npe = 0.0;
    for(int i=0;i<vec_pass_time.size();i++){
        for(int m=vec_start_time[i]; m<vec_end_time[i]; m++){
        //npe +=  (adc[m]+adc[m+1])/2  ;
            if(adc[m]>=0)  tmp_npe+= adc[m];
        }
        if(tmp_npe/spescale < 0.1) { tmp_npe = 0; }
        else {firstHit = i;  break;}
        //firstHit = i;  break;
    }
    if(tmp_npe == 0) { tmp_npe = -1; firstHit = -1; } 
//    if(npe/spe < 2) cout << start << " --- " << end << endl;
    return tmp_npe/spescale;

}

/*************************************************************/
// single fixed threshold timing w/o corrections
double TimeCorrAlg::getFHT_fixed(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum){
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
    else return vec_start_time[hitNum];
}



/*************************************************************/
// 2-points linear fitting leading egde

double TimeCorrAlg::getFHT_linear(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum){

    auto& adc_origin = channel.adc();
    auto& tdc = channel.tdc();
    if(adc_origin.size() == 0) return 0;

    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    if(hitNum<0) return -1;
    int pass_time = vec_pass_time[hitNum];
/*    for(int i=0; i<m_length;i++){
        if( adc[i-1]<=threshold && adc[i]>=threshold ){
            pass_time = tdc[i]; break;
        }
    }
    if(pass_time < 0) return pass_time;  // no-passing threshold, return
*/
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
    }  gHeight = max;
    if(max <= 0) return pass_time;  // negative peak value, return
    else{

        double threshold1 = 0.2*max;
        double threshold2 = 0.8*max;
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

        //cout <<"20% & 90% ==> t1: " << t1 << " y1: " << adc[t1]  << " t2: " << t2 << " y2: " << adc[t2] << endl;
        xx1 = t1; xx2 = t2; yy1=adc[t1]; yy2=adc[t2];
        if(threshold1-threshold2 != 0)return ( (threshold1*t2 - threshold2*t1) / (threshold1 - threshold2) );
        else return t1;
    }

    
}

/******************************************************************/


double TimeCorrAlg::getFHT_linearFit(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum, int fitOption){

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

        xx1 = t1; xx2 = t2; yy1=adc[t1]; yy2=adc[t2];
        if(t2-t1 != 0){   // fitting all points between these 2 cuts
            TGraphErrors *g1 = new TGraphErrors();
            for(int kk=t1;kk<t2;kk++){
                g1->SetPoint(kk-t1,kk,adc[kk]);
                g1->SetPointError(kk-t1,0.5,threshold/20);
            }
            TF1* f1 = new TF1("linear_fit","[0]+x*[1]",t1,t2);
            g1->Fit(f1,"RQE");
            chi2ndf = 0;
            double tmp_y;
            for(int kk=t1;kk<t2;kk++){
            //    chi2ndf += TMath::Abs( (adc[kk]-f1->Eval(kk)) /adc[kk] );    
                tmp_y += (adc[kk]-f1->Eval(kk) );
            }
            //chi2ndf = TMath::Sqrt(chi2ndf);
            //chi2ndf /= (t2-t1+1);
            tmp_y /= (t2-t1+1);
            //return ( (threshold1*t2 - threshold2*t1) / (threshold1 - threshold2) );
            //cout << "fitting timing: " << -f1->GetParameter(0)/f1->GetParameter(1) << " with chi2/ndf" << f1->GetChisquare()/f1->GetNDF() << endl;
            p0 = f1->GetParameter(0); p1 = f1->GetParameter(1);
            chi2ndf = f1->GetChisquare() / f1->GetNDF();  //cout << "chi2ndf: " << chi2ndf << endl;
            t_bias = tmp_y / p1;
            delete g1; delete f1;
            return ( -p0/p1 );
        }
        else return t1;
    
    }

}


double TimeCorrAlg::getFHT_CFD(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum){

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


double TimeCorrAlg::getFHT_peakFitting(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum){


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




/*******************************************************************/


// Charge Center Vertex Reconstruction
bool TimeCorrAlg::ChargeCenterRec()
{
   double x_sum = 0;
   double y_sum = 0;
   double z_sum = 0; 
   double PE_sum = 0;
    
   for(int i = 0; i< num_PMT; i++){  
     x_sum = x_sum + ALL_PMT_pos.at(i).X()*m_nPE[i];
     y_sum = y_sum + ALL_PMT_pos.at(i).Y()*m_nPE[i];
     z_sum = z_sum + ALL_PMT_pos.at(i).Z()*m_nPE[i];
     PE_sum += m_nPE[i];  
   }

   if(PE_sum == 0) {
    ChaCenRec_x = 0; ChaCenRec_y = 0; ChaCenRec_z = 0;
   }
    else{
    ChaCenRec_x = x_sum/PE_sum;  
    ChaCenRec_y = y_sum/PE_sum;
    ChaCenRec_z = z_sum/PE_sum;
   }
   return true;
}

/**********************************************************/

// search pulse
void TimeCorrAlg::getPulse(const JM::ElecWaveform& channel, double baseline, double threshold){
    const auto& adc_origin = channel.adc();
    const auto& tdc = channel.tdc();
    if(adc_origin.size() == 0 ) return ;
    
    const int m_length = adc_origin.size();
    double adc[m_length];
    for(int i=0;i<m_length;i++){
        adc[i] = adc_origin[i];  
        adc[i] -= baseline; 
    }
    
   // search for passing-threshold pulse 
    for(int i=1; i<m_length; i++){ 
        if(adc[i-1]<=threshold && adc[i]>=threshold) {
            vec_pass_time.push_back(i); 
        } 
//        if(adc[i-1]>=threshold && adc[i] <=threshold ){
//            vec_fall_time.push_back(i-1);
//        }
    }

    //to avoid signals in first or end of waveforms
//    if( vec_fall_time.size() > vec_pass_time.size() ) {vec_fall_time.erase(vec_fall_time.begin()); }
//    if( vec_pass_time.size() > vec_fall_time.size() ) {vec_pass_time.erase( vec_pass_time.end());}


    //find integral range
    if (vec_pass_time.size() ==0)  return ;  
    for(int i=0; i<vec_pass_time.size();i++){  // pulse iteration
        for(int k=0; k<vec_pass_time[i]; k++){
            if( adc[vec_pass_time[i]-k] <= 0 ) { //cout << "no. pass "<< i <<" scan k :" << k << "  start time: " << vec_pass_time[i]-k << endl;
                if(i==0){vec_start_time.push_back(vec_pass_time[i]-k); break;}
                else if(i>0) {
                    if(vec_pass_time[i]-k == vec_start_time[vec_start_time.size()-1]) break;
                     else { vec_start_time.push_back(vec_pass_time[i]-k); break;} 
                }
            }
            //vec_start_time.push_back(0);
        }
    }

    for(int i=0; i<vec_pass_time.size();i++){
        for(int k=0; k<m_length; k++){
            if( adc[vec_pass_time[i]+k] <= 0 ) { //cout << "no. pass "<< i <<" scan k :" << k << "  end time: " << vec_pass_time[i]+k << endl;
                if(i==0){vec_end_time.push_back(vec_pass_time[i]+k); break;}
                else if(i>0){ 
                    if( vec_pass_time[i]+k == vec_end_time[vec_end_time.size()-1] ) break; 
                    else { vec_end_time.push_back(vec_pass_time[i]+k); break; } 
                } 
            }
            //vec_end_time.push_back(m_length);
        }
    }
    return;
}

double TimeCorrAlg::CalculateTOF(int pmtid, double source_x, double source_y, double source_z){
    // input source position in unit mm
    
  double pmt_pos_x = ALL_PMT_pos.at(pmtid).X();
  double pmt_pos_y = ALL_PMT_pos.at(pmtid).Y();
  double pmt_pos_z = ALL_PMT_pos.at(pmtid).Z();

  double dx = (source_x - pmt_pos_x)/1000;
  double dy = (source_y - pmt_pos_y)/1000;
  double dz = (source_z - pmt_pos_z)/1000;

  double r0 = (TMath::Sqrt(source_x*source_x+source_y*source_y+source_z*source_z))/1000;
  double dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);

  double cos_theta = (Ball_R*Ball_R+dist*dist-r0*r0)/(2*Ball_R*dist);
   
  double theta = TMath::ACos(cos_theta);

  double dist_oil = Ball_R*cos_theta-TMath::Sqrt(LS_R*LS_R-Ball_R*Ball_R*TMath::Sin(theta)*TMath::Sin(theta));

  double deltaR = TMath::Sqrt(pmt_pos_x*pmt_pos_x + pmt_pos_y*pmt_pos_y + pmt_pos_z*pmt_pos_z) - LS_R;

  
    //return deltaR * 1e9 * 1.33/299792458;
    return (dist-dist_oil)*1e9*1.54/299792458+dist_oil*1e9*1.33/299792458;//currently
}


void TimeCorrAlg::draw_waveform(const JM::ElecWaveform& channel, int pmtID, double time, int fht, double baseline, double threshold, double p0, double p1){
    
    auto& adc_origin = channel.adc();
    auto& tdc = channel.tdc();
    if(adc_origin.size() == 0 ) return ;

    TGraph* waveform = new TGraph(adc_origin.size());

    int startpoint;
    if(floor(fht) -100>=0)  startpoint = fht-99;
    else startpoint = 0;
    for(int i=0; i<adc_origin.size();i++){
    //for(int i=startpoint; i<fht+200;i++){
        //waveform->SetPoint(i- startpoint,tdc[i],adc_origin[i]);
        waveform->SetPoint(i,tdc[i],adc_origin[i]);
    }

    //std::string str = "deltaT =";
    //auto graphName = std::to_string(time);
    //str = str+graphName;

    std::string str = "pmtID = ";
    auto graphName = std::to_string(pmtID);
    str = str+graphName;

    TGraph* line1 = new TGraph();
    //line1->SetPoint(0,time,100);
    //line1->SetPoint(1,time,200);
    line1->SetPoint(0,xx1,yy1+baseline);
    line1->SetPoint(1,xx2,yy2+baseline);

    TGraph *line2 = new TGraph();
    line2->SetPoint(0,x1,y1+baseline);
    line2->SetPoint(1,x2,y2+baseline);

    TGraph* gThre = new TGraph();
    gThre->SetPoint(0,0,threshold+baseline);
    gThre->SetPoint(1,1000,threshold+baseline);

    TF1 *ff = new TF1("ff","[0]+[1]*x",0,500);
    ff->FixParameter(0,p0);
    ff->FixParameter(1,p1);

//    int num_pulse = vec_start_time.size();  cout << "pulse num: " << num_pulse << endl;
//    TGraph* pulse_edge[num_pulse];
//    for(int ii=0;ii<num_pulse;ii++){
//        pulse_edge[ii]->SetPoint(0,180/*vec_start_time[ii]*/,50);
//        pulse_edge[ii]->SetPoint(1,180/*vec_start_time[ii]*/, 500);
        //pulse_edge[ii+1]->SetPoint(0,vec_start_time[ii+1],50);
        //pulse_edge[ii+1]->SetPoint(1,vec_end_time[ii+1],500);
//    }

//    cout << " all graphs ready! " << endl;

    cc->cd();
    waveform->SetTitle(str.c_str());
    waveform->Draw("APL");
    //waveform->Draw("AP");
    line1->SetMarkerColor(kRed);
    line1->SetLineColor(kRed);
    line1->SetLineWidth(2);
    line1->Draw("SAME PL");
    gThre->SetLineColor(kGreen+2);
    gThre->SetLineWidth(2);
    gThre->Draw("SAME PL");
    line2->SetLineColor(kBlue+1);
    line2->SetLineWidth(2);
    line2->Draw("SAME PL");
    ff->SetLineWidth(2);
    ff->SetLineColor(kPink);
    ff->Draw("SAME");
/*    for(int ii=0;ii<num_pulse*2;ii++){
        pulse_edge[ii]->SetLineWidth(2);
        pulse_edge[ii]->SetLineColor(kGreen+1);
        pulse_edge[ii]->Draw("SAME PL");
    }*/
    cc->Print("pmt_wf.pdf(");
    delete waveform;

}


#endif
