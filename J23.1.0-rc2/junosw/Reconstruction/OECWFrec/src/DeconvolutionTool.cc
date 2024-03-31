/*
    @auther: Wei Jiang
    Alg2Tool by jiangw@ihep.ac.cn 2023.02.03
*/
#include "DeconvolutionTool.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

DECLARE_TOOL(DeconvolutionTool);
std::mutex DeconvolutionTool::m_mutex;

DeconvolutionTool::DeconvolutionTool(const std::string &name): ToolBase(name){
    m_maxhits = 32;
    m_maxProfileN = 100;
    m_totalLPMT = 17612;
    enableCalibCorr = true;

    m_length = 1000;
    m_calib_length = 1000;
    m_calib_start_time = 300;
    m_amp_threshold   = {0.030, 0.025};
    m_width_threshold = {   12,    12};
    m_itg_width       = {    7,     8};

    declProp("TotalPMT", m_totalLPMT);
    declProp("enableCalibCorr", enableCalibCorr);
    declProp("Calib_Length", m_calib_length);
    declProp("Calib_Start_Time", m_calib_start_time);
    declProp("Deconvolution_amp_threshold", m_amp_threshold);
    declProp("Deconvolution_width_threshold", m_width_threshold);
    declProp("Deconvolution_itg_width", m_itg_width);
}

DeconvolutionTool::~DeconvolutionTool(){
    fft_forward.reset();
    fft_back.reset();
}

bool DeconvolutionTool::initialize(){
    SniperPtr<PMTParamSvc> pmtParamSvc(getParent(), "PMTParamSvc");
    if (pmtParamSvc.invalid()) {
        LogError << "Failed to get PMTParamSvc instance!" << std::endl;
        return false;
    }
    auto pmtsvc = pmtParamSvc.data();
    SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
    if (calSvc.invalid()) {
        LogError << "Failed to get PMTCalibSvc instance!" << std::endl;
        return false;
    }
    auto calibsvc = calSvc.data();

    // get hmmt pmt
    m_hmmtpmt = std::vector<int>(m_totalLPMT);
    for(int i=0;i<m_totalLPMT;i++) m_hmmtpmt[i] = int(pmtsvc->isHamamatsu(i));

    m_toffset  = calibsvc->getTimeOffset();
    m_SPEratio = calibsvc->getGain();
    m_filter   = calibsvc->getLPMTfilter();
    m_SPERE    = calibsvc->getLPMTre();
    m_SPEIM    = calibsvc->getLPMTim();
    
    N_filter = int(m_filter.size()/2);
    for(int i=0;i<4;i++) LogInfo << "Read filter bin " << i+1 << " nnvt/hmmt value: " << m_filter[i] << "/" << m_filter[N_filter+i] << std::endl;
    
    N_freq  = int(m_SPERE.size())/m_totalLPMT;
    LogInfo << "SPE freq NbinX: " << N_freq << std::endl;
    for(int i=1;i<4;i++) LogInfo << "Read SPE freq of PMT3, bin " << i+1 << " re/im value: " << m_SPERE[3*N_freq+i] << "/" << m_SPEIM[3*N_freq+i] << std::endl;

    if(enableCalibCorr){
        for(int i=0;i<3;i++) LogInfo << "Read toffset:  pmt "  << i << " " << m_toffset[i]  << std::endl;

        for(int i=0;i<3;i++) LogInfo << "Read SPEratio:  pmt " << i << " " << m_SPEratio[i] << std::endl;
    }

    // size check
    if(m_filter.size()%2!=0 || m_SPERE.size()%m_totalLPMT!=0 || m_SPEIM.size()%m_totalLPMT!=0 || m_hmmtpmt.size()%m_totalLPMT!=0 || m_toffset.size()%m_totalLPMT!=0 || m_SPEratio.size()%m_totalLPMT!=0){
        LogError << "Size check fail!" << std::endl;
        return false;
    }
 
    return true;
}
void DeconvolutionTool::rec(QTInfo &qtinfo){
    if(qtinfo.b_getFHT){
        LogError << "Deconvolution can not be FHT method alone!" << std::endl;
        return;
    }

    int pmtid = qtinfo.pmtid;
    auto &wf = qtinfo.wf;
    auto &charge = qtinfo.charge;
    auto &time = qtinfo.time;
    // initial fft at the first wf
    if(!fft_forward.get()){
        if(m_length != int(wf.size())){
            LogInfo << "Raw waveform length: " << wf.size() << std::endl;
            m_length = int(wf.size());
        }
        if(int(wf.size())!=m_calib_length){
            LogError << "Tmp not fit for different length (!="<<m_calib_length<<") waveform!" << std::endl;
            return;
        }
        isLongerWave = m_length > m_calib_length;
        m_dft_length = isLongerWave ? m_length : m_calib_length;
        m_f_scale = m_calib_length*1./(m_length*1.);
        // wf  .resize(m_dft_length); // extra space for shorter wf's zero-padding, but... leave it to the future if there is shorter wf
        wfre.resize(m_dft_length);
        wfim.resize(m_dft_length);
        wfback.resize(m_dft_length);
        HitBTime.resize(m_maxhits);
        HitETime.resize(m_maxhits);
        adc_profile.resize(m_maxProfileN); // 100 is enough
        b_bsl.resize(m_dft_length);

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            fft_forward.reset(TVirtualFFT::FFT(1, &m_dft_length, "R2C EX K"));
            fft_back   .reset(TVirtualFFT::FFT(1, &m_dft_length, "C2R EX K"));
        }
    }
    // notice that if isShorterWave, the vector size is not enough
    if(m_length<m_dft_length) std::fill(wf.begin()+m_length, wf.end(), 0);

    fft_forward->SetPoints(wf.data());
    fft_forward->Transform();
    fft_forward->GetPointsComplex(wfre.data(), wfim.data());

    const int isHmmt = m_hmmtpmt[pmtid];
    auto pmt_filter = &(m_filter[isHmmt*N_filter]);
    for (int i = 0; i < N_filter; i++) {
        if(!isLongerWave){
            wfre[i] *= pmt_filter[i];
            wfim[i] *= pmt_filter[i];
        }else{
            double tmp_x = i*m_f_scale;
            int x0 = int(tmp_x);
            wfre[i] *= getInterpolateValue(tmp_x-1.*x0,pmt_filter[x0],pmt_filter[x0+1]);
            wfim[i] *= getInterpolateValue(tmp_x-1.*x0,pmt_filter[x0],pmt_filter[x0+1]);
        }
    }
    std::fill(wfre.begin()+N_filter, wfre.end(),0);
    std::fill(wfim.begin()+N_filter, wfim.end(),0);

    auto spere = &(m_SPERE[pmtid*N_freq]);
    auto speim = &(m_SPEIM[pmtid*N_freq]);
    wfre[0] = 0;
    wfim[0] = 0;
    for(int i=1;i<N_freq;i++){
        double c11, d11;
        if(!isLongerWave){
            c11 = spere[i];
            d11 = speim[i];
        }else{
            double tmp_x = i*m_f_scale;
            int x0 = int(tmp_x);
            c11 = getInterpolateValue(tmp_x-1.*x0, spere[x0], spere[x0+1]);
            d11 = getInterpolateValue(tmp_x-1.*x0, speim[x0], speim[x0+1]);
        }
        if(c11!=0 || d11!=0){
            double a11 = wfre[i], b11 = wfim[i];
            double f11 = c11*c11 + d11*d11;
            wfre[i] = (a11 * c11 + b11 * d11) / f11;
            wfim[i] = (b11 * c11 - a11 * d11) / f11;
        }else{
            wfre[i] = 0;
            wfim[i] = 0;
        }
    }
    std::fill(wfre.begin()+N_freq, wfre.end(),0);
    std::fill(wfim.begin()+N_freq, wfim.end(),0);

    fft_back->SetPointsComplex(wfre.data(), wfim.data());
    fft_back->Transform();
    fft_back->GetPoints(wfback.data());

    for(int i=m_calib_start_time;i<m_length;i++) wf[i] = wfback[i             -m_calib_start_time]/m_dft_length;
    for(int i=0;       i<m_calib_start_time;i++) wf[i] = wfback[i+m_dft_length-m_calib_start_time]/m_dft_length;


    // start reco.
    const int N = m_length;

    double wf_min = *min_element(wf.begin(), wf.end());
    double wf_max = *max_element(wf.begin(), wf.end());

    // find raw baseline
    const double bslUnder0_cut = 1.;
    double tmp_scale = double(m_maxProfileN);
    if(wf_max-wf_min>bslUnder0_cut){
        // diff > 1 is enough to identify big pulse
        wf_max = 0.5;
        tmp_scale = bslUnder0_cut*m_maxProfileN/(wf_max-wf_min);
    }
    const int imin = int(wf_min*tmp_scale);
    // const int imax = int(wf_max*tmp_scale);
    std::fill(adc_profile.begin(), adc_profile.end(), 0);
    for(int i=0;i<N;i++){
        int tmpidx = int(wf[i]*tmp_scale)-imin;
        if(tmpidx<0 || tmpidx>=m_maxProfileN) continue;
        ++adc_profile[tmpidx];
    }
    double baseline = (max_element(adc_profile.begin(), adc_profile.end())-adc_profile.begin()+imin) / tmp_scale;
    for(int i=0;i<N;i++) wf[i] -= baseline;

    double amp_threshold = m_amp_threshold[isHmmt];
    double width_threshold = m_width_threshold[isHmmt];
    // rought hits
    int HitCount = 0;
    int Pass = 0;
    for (int i = 0; i < N; i++) {
        if (Pass == 0 && wf[i] > amp_threshold) {
            for (int j = i; j >= 0; j--) {
                if (wf[j] <= 0 || j == 0){
                    HitBTime[HitCount] = j;
                    break;
                }
            }
            Pass = 1;
            continue;
        }
        if (Pass == 1) {
            if (wf[i] <= 0 || i == N - 1) {
                HitETime[HitCount] = i;
                if (HitETime[HitCount]-HitBTime[HitCount]+1 > width_threshold) ++HitCount;  // 12 ns width for J19
                Pass = 0;
            }
        }
        if (HitCount >= m_maxhits) {
            LogWarn << "Too much hits recognized! Move to next waveform." << std::endl;
            break;  // if it really happend, it should print out warning and move to next waveform.
        }
    }

    // accurate baseline
    int counter = 0;
    baseline = 0;
    std::fill(b_bsl.begin(), b_bsl.end(), 1);
    for(int j=0;j<HitCount;j++){
        int bL = HitBTime[j] - 9;
        int bR = HitETime[j] + 9;
        if(bL<0)   bL = 0;
        if(bR>N-1) bR = N-1;
        counter += bR-bL+1;
        for(int i=bL;i<=bR;i++) b_bsl[i] = 0;
    }
    counter = N-counter;
    for (int i = 0; i < N; i++) baseline += wf[i]*b_bsl[i];
    if (counter < 40) {
        LogWarn << "No enough points for baseline calculation, baseline will set to be the default one." << std::endl;  // make sure to have enough points to calculate baseline.
    } else if (counter != 0) {
        baseline /= (counter + 0.);
        for (int i = 0; i < N; i++) wf[i] -= baseline;
    }
    
    // accurate hits
    HitCount = 0;
    Pass = 0;
    for (int i = 0; i < N; i++) {
        if (Pass == 0 && wf[i] > amp_threshold) {
            for (int j = i; j >= 0; j--) {
                if (wf[j] <= 0 || j == 0){
                    HitBTime[HitCount] = j;
                    break;
                }
            }
            Pass = 1;
            continue;
        }
        if (Pass == 1) {
            if (wf[i] <= 0 || i == N - 1) {
                HitETime[HitCount] = i;
                if (HitETime[HitCount]-HitBTime[HitCount]+1 > width_threshold) ++HitCount;  // 12 ns width for J19
                Pass = 0;
            }
        }
        if (HitCount >= m_maxhits) {
            LogWarn << "Too much hits recognized! Move to next waveform." << std::endl;
            break;  // if it really happend, it should print out warning and move to next waveform.
        }
    }

    if (HitCount == 0) return;

    int inteW = m_itg_width[isHmmt];
    for (int i = 0; i < HitCount; i++) {
        time.push_back(HitBTime[i]);
        int inte_begin = 0, inte_end = 0;
        if (HitCount == 1) {
            inte_begin = HitBTime[i] - inteW + 1;
            if (inte_begin < 0) inte_begin = 0;
            inte_end = HitETime[i] + inteW;
            if (inte_end > N) inte_end = N;
        } else {
            if (i == 0) {
                inte_begin = HitBTime[i] - inteW + 1;
                if (inte_begin < 0) inte_begin = 0;
                int interval = HitBTime[1] - HitETime[0];
                if (interval > 2 * inteW) {
                    inte_end = HitETime[0] + inteW;
                } else
                    inte_end = int(HitETime[0] + interval / 2.);
            } else if (i == HitCount - 1) {
                inte_end = HitETime[i] + inteW;
                if (inte_end > N) inte_end = N;
                int interval = HitBTime[i] - HitETime[i - 1];
                if (interval > 2 * inteW) {
                    inte_begin = HitBTime[i] - inteW + 1;
                } else
                    inte_begin = int(HitBTime[i] - interval / 2.);
            } else {
                int interval = HitBTime[i + 1] - HitETime[i];
                if (interval > inteW * 2) {
                    inte_end = HitETime[i] + inteW;
                } else
                    inte_end = int(HitETime[i] + interval / 2.);
                interval = HitBTime[i] - HitETime[i - 1];
                if (interval > 2 * inteW) {
                    inte_begin = HitBTime[i] - inteW + 1;
                } else
                    inte_begin = int(HitBTime[i] - interval / 2.);
            }
        }
        double tempC = 0;
        for (int j = inte_begin; j < inte_end; j++) tempC += wf[j];
        charge.push_back(tempC/m_SPEratio[pmtid]);
    }

    double ttq = 0;
    for(auto it=charge.begin();it!=charge.end();++it) ttq += *it;
    if(enableCalibCorr){
        for(auto itT=time.begin();itT!=time.end();++itT){
            *itT -= m_toffset[pmtid];
        }
    }
    qtinfo.nPE = ttq;
};

double DeconvolutionTool::getInterpolateValue(double detlta_x, double y0, double y1)
{
  return y0 + (y1-y0)*detlta_x;
}

JM::CalibPmtChannel * DeconvolutionTool::recChannel(int, std::vector<uint16_t>&){
    return nullptr;
}