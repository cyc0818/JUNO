//to do review
#include "yzyQCTool.h"
#include "SniperKernel/ToolFactory.h"

DECLARE_TOOL(yzyQCTool);

yzyQCTool::yzyQCTool(const std::string &name): ToolBase(name){
    declProp("UsePerfectVtx", b_usePerfectVtx=false);
}

yzyQCTool::~yzyQCTool(){
}

bool yzyQCTool::configure(){
    // gamma correction function
    m_efun = new TF1("efun", "[0]/x-1100.35*(1+0.061766*log(x)-0.006967*log(x)*log(x)-0.00156527*log(x)*log(x)*log(x))");
    // eplus correction fuction
    //m_efun = new TF1("efun", "[0]-1273.57*(1+0.78184*log(x)+0.320446*log(x)*log(x)+0.37355*log(x)*log(x)*log(x))");
    //Non-linearity Correction for positron, copied from RecTimeLikeAlg
    double  non_li_parameter[4] = {0.122495, 1.04074, 1.78087, 0.00189743};
    m_non_li_positron = new TF1("pos","(([1]+[3]*(x-1.022))/(1+[0]*exp(-[2]*(x-1.022)))*(x-1.022)+0.935802158)",0.2,12);
    m_non_li_positron->SetParameters(non_li_parameter);

    return true;
}

void yzyQCTool::rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo){
    recVtx(hitpmtvector, pmttable, recinfo);
    recE(hitpmtvector, pmttable, recinfo);
}

void yzyQCTool::recE(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo){
    double pe_sum= 0;
    for (auto* thisPMT: hitpmtvector){
        pe_sum+= thisPMT->totq;
    }

    //correction of energy
    // Use Calib function to correct energy
    double a = 10000.0;
    double r = 0.001*recinfo.vtx.Mag();
    double r3 = r*r*r;
    double k = pe_sum/ (a+r3);
    m_efun->SetParameter(0, k*a);
    recinfo.energy = m_efun->GetX(0, 0.1, 11.0);
    //double y = m_efun->Eval(1.1);
    //std::cout << "y= " << y << std::endl;

    //// deposit energy with Non-linear correction for positron
    //m_eprec = m_non_li_positron->GetX(d_energy);
    LogDebug<<"energy:"<<recinfo.energy<<std::endl;
}

void yzyQCTool::recVtx(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo){
    TVector3 qCtr(0, 0, 0);
    double pe_sum= 0;
    for (auto* thisPMT: hitpmtvector){
        const TVector3& center=thisPMT->pos;
        qCtr+=center*thisPMT->totq;
        pe_sum += thisPMT->totq;
    }

    if (pe_sum>0.0001){
        qCtr*=(1./pe_sum);
    }
    else{
        LogWarn<<"The sum of pe is less equal than 0!"<<std::endl;
        return;
    }

    //correction of vertex
    double ratioVtxCorrection = 1.1444;
    if (b_usePerfectVtx) ratioVtxCorrection = 1.5;
    qCtr*=ratioVtxCorrection;
    recinfo.vtx = qCtr;
    LogDebug<<"X:" <<recinfo.vtx.X()<<"  Y: "<<recinfo.vtx.Y()<<"  Z: "<<recinfo.vtx.Z()<< std::endl;
}
