#include "firstHitTimeExpected.h"
#include "TMath.h"
double 
firstHitTimeExpected(TVector3 ri,
        double ti,
        TVector3 direction, 
        double length, 
        const PmtProp& pmt,
        const Params& params
        ) 
{
    double m_nLSlight = params.get("LSRefraction");
    double m_clight = params.get("LightSpeed");
    double m_vmuon = params.get("MuonSpeed");

    //this theta is the angle between the first hit photon's track and the muon track.
    double tantheta  = TMath::Sqrt(m_nLSlight*m_nLSlight-1); 

    TVector3 Vu = direction* m_vmuon; 
    TVector3 R_ipmt = pmt.pos; 

    //end point of the track
    TVector3 Re = ri+direction*length; 

    // the perpendicular foot of the ipmt to the track.
    TVector3 Rperp = ri+direction*(R_ipmt-ri)*direction; 

    //dtperp is the time muon fly from ri to Rperp; 
    double dtperp = (Rperp-ri)*direction/m_vmuon;  

    // photon emitted from point C of the track fistly hit to the ipmt;
    // dtc is the time of muon from ri to point C; 
    TVector3 Rc; 
    double dtc = dtperp-(R_ipmt-Rperp).Mag()/(m_vmuon*tantheta); 

    //dte is the time muon fly from ri to Re; 
    double dte = length/m_vmuon; 

    //if the dtc small than zero,  means the 'fastest light' point is out of the LS, 
    //so the injection point is the fastest light point.
    if(dtc<0){
        Rc = ri; 
        dtc = 0; 
    }
    //if the dtc is greater than dte, meaning that the 'fastest light' point is after the Re, 
    //so the end point is the fastest light point.
    else if(dtc>dte){
        Rc = Re; 
        dtc = dte; 
    }
    else{
        Rc = ri+Vu*dtc; 
    }

    double t_ipmt = ti + dtc + m_nLSlight/m_clight*(R_ipmt-Rc).Mag(); 
    return t_ipmt; 
}
