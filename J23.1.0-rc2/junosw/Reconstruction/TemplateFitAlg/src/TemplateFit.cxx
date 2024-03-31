/*
 *debug: wether output some control plots
 *m_xT: sample rate related 1=1G,2=500M,0.1=10G
 *weight:sample rate related. 1=1G,2=500M
 * change samplefrequency in python script
 *
 */
bool debug=false;
#include "TemplateFit.h"
    void
TemplateFit::genth1()
{

    TFile* wavef2=TFile::Open("/workfs/dyw/chengyp/newjuno/offline/Reconstruction/TemplateFitAlg/share/mcp_waveform.root","READ");
    m_template=(TH1F* )wavef2->Get("spe");
    m_template->SetDirectory(0);
    wavef2->Close();
}

    float
TemplateFit::singletemplate(Double_t *x, Double_t *par)
{
    const double xx = x[0]; 
    const double w1 = par[0];//shift tdc
    const double w2 = par[1];//scale adc
  //  if((xx-w1)>9.9995e+02||(xx-w1)<(-5.0e-02)) return 0;
    const double y1 = (w2*m_template->Interpolate((xx-w1)));//both xx and w1 in unit of ns
    return y1;
}

    double 
TemplateFit::gentemplate(Double_t *x, Double_t *par)
{
    double value= 0; 
    for(int pulseIdx=0; pulseIdx<m_nfitpulse; pulseIdx++){
        int startIdx = pulseIdx*2;
        value +=singletemplate(x,&par[startIdx]);
    } 
    return value;
}

TemplateFit::TemplateFit()
{
    m_xT = 1.;//sample period unit :ns
    m_initializeoffset = 7/m_xT;// need find it by yourself
    m_SlidingSumWidth = int(m_absSlidingSumWidth/m_xT);


    genth1();// get template
    calib();// calib
}
// calib spe area--this is important
    void 
TemplateFit::calib()
{
    TFile* wavef=TFile::Open("/workfs/dyw/chengyp/newjuno/offline/Reconstruction/TemplateFitAlg/share/mcp_calib.root","READ");
    m_singlepecalib=(TH1F* )wavef->Get("spe");
    m_singlepecalib->SetDirectory(0);
    wavef->Close();

    int calib_totalpoints = m_singlepecalib->GetNbinsX();
    std::vector<float> adc_arr_spe;
    adc_arr_spe.clear();
    int calib_SWSumN = calib_totalpoints-m_SlidingSumWidth+1;
    std::vector<float> SlidingSum_spe;
    SlidingSum_spe.clear();

    double noise_rms = m_singlepecalib->GetBinError(1);
    double startthres_spe=5*sqrt(m_SlidingSumWidth)*noise_rms;
    double endthres_spe=1.0/sqrt(m_SlidingSumWidth)*noise_rms;

    float sampletruetime;
    for(int ia=0;ia<calib_totalpoints;ia++)
    {
        sampletruetime = m_singlepecalib->GetBinCenter(ia+1);
        adc_arr_spe.push_back(m_singlepecalib->Interpolate(sampletruetime));   // ia is only bin change it to time :ns
    }

    SlidingSum_spe.push_back(0);
    for(int ip=0;ip<m_SlidingSumWidth;ip++)
    {
        SlidingSum_spe[0]+=adc_arr_spe[ip];
    }

    for(int ib=1;ib<calib_SWSumN;ib++)
    {
        SlidingSum_spe.push_back(SlidingSum_spe[ib-1]-adc_arr_spe[ib-1]+adc_arr_spe[ib+m_SlidingSumWidth-1]);
    }

    int startb=-10;int endb=-10;
    bool pulse_exist=false;

    for(int ib=0;ib<calib_SWSumN;ib++){
        if(!pulse_exist&&SlidingSum_spe[ib]>startthres_spe)
        {
            pulse_exist=true;
            startb=ib;
        }
        if(pulse_exist&&SlidingSum_spe[ib]<endthres_spe){
            pulse_exist=false;
            endb=ib;
            // here assume the first region found is true single pe pmt pulse
            break;
        }
    }

    float totalQ=0;
    for(int roiQindex=startb;roiQindex<endb;roiQindex++)
    {
        totalQ+=adc_arr_spe[roiQindex];
    }
    m_QcalibSpe=totalQ;
}

    Pair
TemplateFit::SearchRoi(TH1F* thf)
{
    // initalize for a new channel
    m_fittrycount=0;
    m_nfitpulse=0;

    m_totalpoints = thf->GetNbinsX();
    m_SWSumN = m_totalpoints-m_SlidingSumWidth+1;

    m_tstart = thf->GetBinLowEdge(1);
    m_tend = thf->GetBinLowEdge(m_totalpoints)+thf->GetBinWidth(1);
    m_peaks.clear();
    m_adc_arr.clear();
    m_SlidingSum.clear();

    double noise_rms=thf->GetBinError(1);
    //here use bin error rather than 0.3mv
    //positive pulse
    m_startthres=5*noise_rms*sqrt(m_SlidingSumWidth);
    m_endthres=noise_rms/sqrt(m_SlidingSumWidth);

    float sampletruetime;
    for(int ia=0;ia<m_totalpoints;ia++)
    {
        sampletruetime = thf->GetBinCenter(ia+1);
        m_adc_arr.push_back(thf->Interpolate(sampletruetime));
    }

    m_SlidingSum.push_back(0);
    for(int ip=0;ip<m_SlidingSumWidth;ip++)
    {
        m_SlidingSum[0]+=m_adc_arr[ip];
    }
    for(int ib=1;ib<m_SWSumN;ib++)
    {
        m_SlidingSum.push_back(m_SlidingSum[ib-1]-m_adc_arr[ib-1]+m_adc_arr[ib+m_SlidingSumWidth-1]);
    }
    //   find pulse region by sliding window sum
    //   pulse begin at <m_startthres  end at  >m_endthres
    //   store roi use tmp_roi
    Pair tmp_roi;
    tmp_roi.clear();
    float startb=-10;
    float endb=-10;
    bool pulse_exist=false;
    for(int ib=0;ib<m_SWSumN;ib++){
        if(!pulse_exist&&m_SlidingSum[ib]>m_startthres)
        {
            pulse_exist=true;
            startb=ib;
            if(ib==0)startb=1;// see line 134
        }
        if(pulse_exist&&(m_SlidingSum[ib]<m_endthres||ib==m_SWSumN-1)){
            pulse_exist=false;
            endb=ib;
            if(ib==m_SWSumN-1) endb=m_totalpoints-1;
            tmp_roi[startb]=endb;
            // before insert, can add some judge to remove random baseline
            // fluc than pass the selection criteria
        }
    }

    // generate test histograms to check whether sliding window works as expected

    if(debug)
    {
        TH1F* th1=new TH1F("th1","",m_totalpoints,m_tstart,m_tend);//sliding window sum histogram
        TH1F* th2=new TH1F("th2","",m_totalpoints,m_tstart,m_tend);//cumulative sum of waveform amplitude
        th1->SetDirectory(0);
        th2->SetDirectory(0);
        for(int ib=0;ib<m_SWSumN;ib++)
        {th1->SetBinContent(ib+1,m_SlidingSum[ib]);}

        std::vector<float> cumulative_sum;
        cumulative_sum.clear();

        for(int ia=0;ia<m_totalpoints;ia++)
        {
            cumulative_sum.push_back(0);
        }
        Pair::iterator peakIt = tmp_roi.begin();
        for(;peakIt != tmp_roi.end();peakIt++)
        {
            for(int roiQindex=(int)(peakIt->first);roiQindex<(int)(peakIt->second);roiQindex++)
            {
                cumulative_sum[roiQindex]=m_adc_arr[roiQindex]+cumulative_sum[roiQindex-1];
            }
        }
        for(int ia=0;ia<m_totalpoints;ia++)
        {
            th2->SetBinContent(ia+1,cumulative_sum[ia]);
        }
        draw(thf,th1,th2);
    } 
    return tmp_roi;
}
    std::vector<float> 
TemplateFit::NpeQ(float a, float b)
{
    std::vector<float> npeQ;
    npeQ.clear();
    float totalQ=0;
    for(int roiQindex=(int)(a);roiQindex<(int)(b);roiQindex++)
    {
        totalQ+=m_adc_arr[roiQindex];
    }
    npeQ.push_back(totalQ/m_QcalibSpe+0.5);
    npeQ.push_back(totalQ);
    return npeQ;
}
//estimate charge and hittime  in pulse region
// if region npe is 0, return empty vector
    vector<float>
TemplateFit::RoughQT(float a, float b,vector<float> npeQ)
{
    int npe=(int)npeQ[0];
    float totalQ=npeQ[1];

    vector<float> hittime_candidate;
    hittime_candidate.clear();
    if(npe==-1||npe==0){/*  cout<<"eee.."<<endl;hittime_candidate.push_back(a-m_initializeoffset);*/return hittime_candidate;}
    float totalQ_n_equidistant = totalQ/(npe+1);
    int n_equi = 1;
    totalQ=0;
    float totalQ_old=0;
    for(int roiQindex=(int)(a);roiQindex<(int)(b);roiQindex++)
    {
        totalQ+=m_adc_arr[roiQindex];
        if( totalQ>n_equi*totalQ_n_equidistant
                && totalQ_old<n_equi*totalQ_n_equidistant)
        {
            hittime_candidate.push_back(roiQindex-m_initializeoffset);
            if(n_equi==npe)break;
            n_equi++;
        }
        totalQ_old=totalQ;
    }
    // /*  
    for(unsigned int rindex=0;rindex<hittime_candidate.size();rindex++)
    {
        if(rindex<hittime_candidate.size()-1)
        {
            if(abs(hittime_candidate[rindex+1]-hittime_candidate[rindex])*m_xT<1.5)
            {
                hittime_candidate.erase(hittime_candidate.begin()+rindex);
                //rindex--;
                break;
            }
        }
    }
    //  */
    return hittime_candidate;
}
// construct fit function  for region fit
// if vector size is 0, construct pol0
    void  
TemplateFit::SetParameters(vector<float> peaks,float lower, float higher)// abs time
{
    m_fittrycount++;
    m_nfitpulse = peaks.size();
    if(m_nfitpulse==0)
    {
        m_presentFit = new TF1(Form("m_presentFit%d",m_fittrycount), "pol0(0)",lower,higher);
    }
    else
    {
        m_presentFit = new TF1(Form("m_presentFit%d",m_fittrycount), this, &TemplateFit::gentemplate,lower,higher,2*m_nfitpulse);
        for(int pulseIdx=0;pulseIdx<m_nfitpulse;pulseIdx++)
        {
            int paridx=pulseIdx*2;
            m_presentFit->SetParameter(paridx,peaks[pulseIdx]);
            m_presentFit->SetParLimits(paridx,peaks[pulseIdx]-20,peaks[pulseIdx]+20);
            m_presentFit->SetParameter(paridx+1,1.0);
            m_presentFit->SetParLimits(paridx+1,0.01,1.99);
        }
    }
}

TFitResultPtr
TemplateFit::Fit(TH1F* thorg){
    //    TVirtualFitter::SetPrecision(10);
    TH1F* th= (TH1F* )thorg->Clone("th");
    // /*  
    if(m_peaks.size()!=0)
    {
        m_nfitpulse = m_peaks.size();
        TF1* alreadyFit = new TF1(Form("alreadyFit_%d",m_fittrycount),this, &TemplateFit::gentemplate,m_tstart,m_tend,2*m_nfitpulse);
        std::map<float,float>::iterator pulseIt = m_peaks.begin();
        int index=0;
        for(;pulseIt != m_peaks.end();pulseIt++) {
            int paridx = index*2;
            alreadyFit->FixParameter(paridx, pulseIt->first);
            alreadyFit->FixParameter(paridx+1, pulseIt->second);
            index++;
        }
        float error = thorg->GetBinError(1);
        for(int ip=0;ip<m_totalpoints;ip++)
        {

            float  sampletruetime = thorg->GetBinCenter(ip+1);
            th->SetBinContent(ip+1,thorg->Interpolate(sampletruetime)-alreadyFit->Eval(sampletruetime));
            th->SetBinError(ip,error);
        }
        delete alreadyFit;
    } 
    // */ 
    TFitResultPtr re=th->Fit(m_presentFit,"RQS","");//"QSEX0M",""); 
    if(debug)
    {
        TCanvas* c= new TCanvas("c","c",800,600); 
        c->cd();
        m_presentFit->SetLineColor(kRed);
        th->Draw();
        m_presentFit->Draw("same");
        c->SaveSource(Form("fittry_%d_%d.C",m_channelcount,m_fittrycount));
        delete c;
    }
    delete  m_presentFit;
    delete th;
    return re; 
}
    bool 
TemplateFit::InsertRegionResult(TFitResultPtr re)
{
    int npulse_region=int(re->NPar()/2);
    if(npulse_region==0){return false;}//no pulse here
    for(int pulseIdx=0; pulseIdx<npulse_region; pulseIdx++) {
        float pulseTime   =  re->Parameter(2*pulseIdx);
        float amp =  re->Parameter(2*pulseIdx+1);
        m_peaks[pulseTime] = amp;
    }
    return true;
}
    double
TemplateFit::Get1st()
{
    /*  some bug in TF1 GetMaximumX() not reliable
        m_nfitpulse =1;
        TF1* f1st = new TF1(Form("f1st_%d",m_channelcount), this, &TemplateFit::gentemplate,m_tstart,m_tend,2*m_nfitpulse);
        std::map<float,float>::iterator pulseIt = m_peaks.begin();
        f1st->FixParameter(0,pulseIt->first);
        f1st->FixParameter(1,pulseIt->second);
        m_firsthittime = f1st->GetMaximumX();
        delete f1st;
        return m_firsthittime;

*/
    return 0.0;
}
    TFitResultPtr
TemplateFit::totalFit(TH1F* th)
{
    m_nfitpulse =  m_peaks.size();
    m_fit = new TF1(Form("m_fit%d",m_channelcount), this, &TemplateFit::gentemplate,m_tstart,m_tend,2*m_nfitpulse);

    std::map<float,float>::iterator pulseIt = m_peaks.begin();

    int index=0;
    for(;pulseIt != m_peaks.end();pulseIt++) {
        int paridx = index*2;
        m_fit->FixParameter(paridx, pulseIt->first);
        m_fit->FixParameter(paridx+1, pulseIt->second);
        index++;
    }
    TFitResultPtr finalfit = th->Fit(m_fit,"SQR0","");
    if(debug)
    {
        TCanvas* c= new TCanvas("c","c",800,600); 
        c->cd();
        m_fit->SetLineColor(kRed);
        m_fit->SetNpx(10000);
        th->Draw();
        m_fit->Draw("same");
        c->SaveSource(Form("/publicfs/dyb/data/userdata/chengyp/junotemplate/finalfit_%d.C",m_channelcount));
        delete c;
    }
    delete m_fit;
    return finalfit;
}

    void 
TemplateFit::draw(TH1F* thf,TH1F* th1,TH1F* th2)
{
    float scalefactor = 1;

    TCanvas* c0 = new TCanvas("c0","c0",800,600);
    // insert scaled TLine here to illustrate threshold
    TLine* a=new TLine(m_tstart,scalefactor*m_startthres,m_tend,scalefactor*m_startthres);
    TLine* b=new TLine(m_tstart,scalefactor*m_endthres,m_tend,scalefactor*m_endthres);
    a->SetLineStyle(9);a->SetLineColor(16);a->SetLineWidth(3);
    b->SetLineStyle(7);b->SetLineColor(16);b->SetLineWidth(3);
    // here clone to not effect original waveform thf 
    TH1F* th = (TH1F* )thf->Clone("th");
    // here the waveform's peak is -1 Volts
    scalefactor = -1/thf->GetMinimum();
    //th->Scale(scalefactor);
    //  th1->Scale(scalefactor);
    //  th2->Scale(scalefactor);

    th->SetLineColor(8);
    th1->SetLineColor(4);
    th2->SetLineColor(2);


    TLegend* leg = new TLegend(0.1,0.7,0.48,0.9,"pulse region finding","brNDC");
    leg->SetFillColor(kWhite);
    leg->AddEntry(th,"Sim MCP waveform","l");
    leg->AddEntry(th1,Form("%d points( %d ns) sliding window sum",m_SlidingSumWidth,m_absSlidingSumWidth),"l");
    leg->AddEntry(th2,"cumulative sum of waveform in pulse region","l");
    leg->AddEntry(a,"pulse region start threshold","l");
    leg->AddEntry(b,"pulse region end threshold","l");

    th2->Draw();
    th1->Draw("same");
    th->SetLineWidth(2);th->SetLineStyle(1);th->SetMarkerStyle(1);
    th->Draw("hist same c");
    a->Draw("same");
    b->Draw("same");
    th->SetLineWidth(2);th->SetLineStyle(1);th->SetMarkerStyle(1);

    leg->SetNColumns(2);
    leg->Draw();

    c0->Update();
    // here SaveSource to avoid No TLegend problem in saved macro
    c0->SaveSource(Form("pulse_region_find_%d.C",m_channelcount));
    delete th;
    delete a;
    delete b;
    delete c0;
    delete leg;
}
