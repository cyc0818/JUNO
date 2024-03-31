//to do review
#include "zyQCTool.h"
#include "SniperKernel/ToolFactory.h"
#include "Event/CalibPmtChannel.h"

DECLARE_TOOL(zyQCTool);

zyQCTool::zyQCTool(const std::string &name): ToolBase(name){
}

zyQCTool::~zyQCTool(){
}

bool zyQCTool::configure(){
    return true;
}

void zyQCTool::rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo){
    m_Hit_buffer.reserve(pmttable.size());
    m_Hit_buffer.clear();
    //fill the hit buffer
    for (auto* thisPMT: hitpmtvector){
        for (auto timeit: thisPMT->pmtcalib->time()){
            m_Hit_buffer.emplace_back(Hit(thisPMT->id, timeit));
        }
    }

    //Rec the Vertex X,Y,Z
    std::sort(m_Hit_buffer.begin(),m_Hit_buffer.end());
    double x_sum = 0;
    double y_sum = 0;
    double z_sum = 0;
    double PE_sum = 0;
    double d_vtxX=0;
    double d_vtxY=0;
    double d_vtxZ=0;
    double promptRec_x = 0;
    double promptRec_y = 0;
    double promptRec_z = 0;

    int cut_num = 0.3*m_Hit_buffer.size();
    for (auto& h: m_Hit_buffer){
        if(PE_sum == cut_num) {
            promptRec_x = x_sum/PE_sum;
            promptRec_y = y_sum/PE_sum;
            promptRec_z = z_sum/PE_sum;
        }
        auto* pmtprop = pmttable[h.pmtid];
        //x_sum = x_sum + ALL_PMT_pos[h.pmtid].X();
        //y_sum = y_sum + ALL_PMT_pos[h.pmtid].Y();
        //z_sum = z_sum + ALL_PMT_pos[h.pmtid].Z();
        x_sum = x_sum + pmtprop->pos.X();
        y_sum = y_sum + pmtprop->pos.Y();
        z_sum = z_sum + pmtprop->pos.Z();
        PE_sum = PE_sum+1;
    }

    d_vtxX = x_sum/PE_sum;
    d_vtxY = y_sum/PE_sum;
    d_vtxZ = z_sum/PE_sum;
    double promptRec_r = sqrt(promptRec_x*promptRec_x+promptRec_y*promptRec_y+promptRec_z*promptRec_z);
    double ChaCenRec_r = sqrt(d_vtxX*d_vtxX+d_vtxY*d_vtxY+d_vtxZ*d_vtxZ);
    double  m_theta = TMath::ACos(d_vtxZ/ChaCenRec_r);
    double m_phi = TMath::ATan((d_vtxY/d_vtxX));
    m_phi = (m_phi+0.2556)/0.9979;
    m_theta = (m_theta-0.02116)/0.9786;

    if(promptRec_r>18500){
        double a = -1.894*1e-5;
        double b = 0.8247;
        promptRec_r = (-b+sqrt(b*b-4*a*(10730-promptRec_r)))/(2*a);
        if(promptRec_r>17700)promptRec_r=17700;
        d_vtxX = TMath::Abs(promptRec_r*TMath::Sin(m_theta)*TMath::Cos(m_phi))*promptRec_x/TMath::Abs(promptRec_x);
        d_vtxY = TMath::Abs(promptRec_r*TMath::Sin(m_phi)*TMath::Sin(m_theta))*promptRec_y/TMath::Abs(promptRec_y);
        d_vtxZ = promptRec_r*TMath::Cos(m_theta);
    }
    else{
        double a1=1.475*1e-6;
        double b1=0.7841;
        ChaCenRec_r = (-b1+sqrt(b1*b1-4*a1*(228.4-ChaCenRec_r)))/(2*a1);
        d_vtxX = TMath::Abs(ChaCenRec_r*TMath::Sin(m_theta)*TMath::Cos(m_phi))*d_vtxX/TMath::Abs(d_vtxX);
        d_vtxY = TMath::Abs(ChaCenRec_r*TMath::Sin(m_phi)*TMath::Sin(m_theta))*d_vtxY/TMath::Abs(d_vtxY);
        d_vtxZ = ChaCenRec_r*TMath::Cos(m_theta);
    }
    LogDebug<<"X:" <<d_vtxX<<"  Y: "<<d_vtxY<<"  Z: "<<d_vtxZ << std::endl;

    recinfo.vtx = TVector3(d_vtxX, d_vtxY, d_vtxZ);
}
