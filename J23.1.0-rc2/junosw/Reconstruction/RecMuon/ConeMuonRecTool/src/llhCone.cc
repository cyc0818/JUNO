#include "llhCone.h"
#include "TGraph.h"
#include "TMath.h"
#include "SniperKernel/SniperLog.h"
#include <math.h>

llhCone::llhCone(const PmtTable* ptab)
     : m_ptable(ptab)
{
}

void
llhCone::setclight(double c){
    m_clight = c;
}

void
llhCone::setnLS(double n){
    m_nLSlight = n;
    ang_c_ls = TMath::ACos(1./m_nLSlight);
    ang_c_ch = TMath::ACos(1./1.333); // Maybe 1.351 ?
}

void
llhCone::setvmuon(double v){
    m_vmuon = v;
}

void
llhCone::setused(std::vector<int> usedTable){
    m_usedTable = usedTable;
}

void
llhCone::setFhtTable(std::vector<double> fhtTable){
    m_fhtTable = fhtTable;
}

void
llhCone::setLKDE(TGraph *KDE_c[18], TGraph *KDE_s[18], TGraph *KDE_ch[18], TGraph *KDE_fs[18])
{
    for(unsigned int ikde=0; ikde<18; ikde++){
        m_L_KDE_c[0][ikde] = KDE_c[ikde];
        m_L_KDE_c[1][ikde] = KDE_ch[ikde];
        m_L_KDE_s[0][ikde] = KDE_s[ikde];
        m_L_KDE_s[1][ikde] = KDE_fs[ikde];
    }
}

void
llhCone::setSKDE(TGraph *KDE_c[18], TGraph *KDE_s[18], TGraph *KDE_ch[18], TGraph *KDE_fs[18])
{
    for(unsigned int ikde=0; ikde<18; ikde++){
        m_S_KDE_c[0][ikde] = KDE_c[ikde];
        m_S_KDE_c[1][ikde] = KDE_ch[ikde];
        m_S_KDE_s[0][ikde] = KDE_s[ikde];
        m_S_KDE_s[1][ikde] = KDE_fs[ikde];
    }
}

double
llhCone::operator()(const double * par)const{
    std::vector<double> p(par, par+7);
    return (*this)(p);
}

double
llhCone::operator()(const std::vector<double>&par)const{

    double res=0;

    // Setup track
    TVector3 r0; 
    r0.SetMagThetaPhi(par[0], par[1], par[2]); 
    double t0 = par[3]; 
    TVector3 direction; 
    direction.SetMagThetaPhi(1, par[4], par[5]); 
    double length = par[6];

    // Check distance from center
    double dfc = (-r0.Cross(direction)).Mag();
    if(dfc>17700) return 1e50; // TODO: Charge Check -> LS Crossed?

    // Get Exit Point
    // Fixed to LS edge for through going tracks
    /*
    length = 2*sqrt(17700*17700 - dfc*dfc);
    TVector3 exit = r0 + length * direction;
    double tex = t0 + length/m_vmuon;
    */
    // Variable endpoint:
    TVector3 exit = r0 + length*direction;
    double tex = t0 + length/m_vmuon;

    // Setup KDE identifier
    int low = floor(dfc/1000);
    int up;
    if(low == 17) up = 17;
    else up = low+1;

    const PmtTable& ptab = *m_ptable; 
    unsigned int pmtnum = ptab.size(); 
    unsigned int inum;
    // put counter
    unsigned int pmtCount = 0;
    for(inum=0; inum<pmtnum; inum++){
        if(m_usedTable.at(inum) == 1){
            // Inits
            double t = m_fhtTable.at(inum);
            TVector3 pmtCenter = ptab[inum].pos;
            double horizon = m_clight/1.333*(t-tex);  // Exit fixed at LS edge! Only water afterwards. Change for stopping!
            // Switches for transitions
            double switchBSC = (pmtCenter - r0).Angle(direction) - ang_c_ls;
            double switchCFS = (pmtCenter - exit).Angle(direction) - ang_c_ch;
            double cons = 25.;

            // Transition functions
            long double PBackSphere = 1./2. * (1+erfl(cons*switchBSC));
            long double PForwardSphere = 1./2. * erfcl(cons*switchCFS);
            long double PCherenkov = 1./2. * erfcl(0.5*cons*(horizon - (pmtCenter-exit).Mag()));

            // Cases: Special: "no cone" for edge events! Transition Backward Sphere -> Cone or Cone->ForwardSphere. Only Cone is done inside else.
            double prob = 0;

            double forSphereVal = spherefunc(pmtCenter-exit, tex, dfc, ptab[inum], inum, low, up, 1);
            double cherenkovVal = conefunc(exit, direction, tex, dfc, ptab[inum], inum, low, up, 1);
            double backSphereVal = spherefunc(pmtCenter-r0, t0, dfc, ptab[inum], inum, low, up, 0);
            double coneVal = conefunc(r0, direction, t0, dfc, ptab[inum], inum, low, up, 0);

            double norm = PBackSphere+PForwardSphere+(1-PBackSphere)*(1-PForwardSphere);
            prob = 1./norm * (PForwardSphere*( PCherenkov*cherenkovVal + (1-PCherenkov)*forSphereVal) + PBackSphere*backSphereVal + (1-PBackSphere)*(1-PForwardSphere)*coneVal);
            
            if(prob <= 0) continue;
            else{
                pmtCount++;
                res += log(prob);
            }
        }
    }
    double outside = exit.Mag() - 17700;
    // Norm the llh
    if(pmtCount < 1000) return 1e4;     // ensure that there are not too few PMTs (minimizer tries to kick out all PMTs sometimes)
    else{
        double llh = -2*(res-pow(outside,2)/400.);
        return llh/pmtCount;
    } 
}


double llhCone::conefunc(TVector3 r0,
        TVector3 direction,
        double t0,
        double dfc,
        const PmtProp& pmt,
        int inum,
        int low,
        int up,
        int pdf_id
        ) const
{
    // pdf_id: 0 for scntillation cone, 1 for cherenkov cone
    TVector3 pmtCenter = pmt.pos;
    double t = m_fhtTable.at(inum); 
    TVector3 a = -direction;
    TVector3 r0t = r0 - m_vmuon*(t-t0)*a;
    double openAngle = (pmtCenter-r0t).Angle(a);
    if(low==up){
        if(pmt.type == _PMTINCH20) return fastEval(m_L_KDE_c[pdf_id][up], openAngle);
        else return fastEval(m_S_KDE_c[pdf_id][up], openAngle);
    }
    else{
        double flow, fup;
        if(pmt.type == _PMTINCH20){
            flow = fastEval(m_L_KDE_c[pdf_id][low], openAngle);
            fup = fastEval(m_L_KDE_c[pdf_id][up], openAngle);
        }
        else{
            flow = fastEval(m_S_KDE_c[pdf_id][low], openAngle);
            fup = fastEval(m_S_KDE_c[pdf_id][up], openAngle);
        }
        return (fup + (dfc/1000 - up) * (flow - fup) / (low - up));
    }
    

}

double llhCone::spherefunc(TVector3 link,
        double t0,
        double dfc,
        const PmtProp& pmt,
        int inum,
        int low,
        int up,
        int pdf_id
        ) const
{
    // pdf_id: 0 for backward, 1 for forward moving sphere
    double linkLength = link.Mag();
    double trackLength = m_clight*(m_fhtTable.at(inum)-t0);
    
    if(linkLength <= trackLength && trackLength > 0.){
        double sphere_angle = asin(linkLength/trackLength);
        if(low==up){
            if(pmt.type == _PMTINCH20) return fastEval(m_L_KDE_s[pdf_id][up], sphere_angle);
            else return fastEval(m_S_KDE_s[pdf_id][up], sphere_angle);
        }
        else{
            double flow, fup;
            if(pmt.type == _PMTINCH20){
                flow = fastEval(m_L_KDE_s[pdf_id][low], sphere_angle);
                fup = fastEval(m_L_KDE_s[pdf_id][up], sphere_angle);
            }
            else{
                flow = fastEval(m_S_KDE_s[pdf_id][low], sphere_angle);
                fup = fastEval(m_S_KDE_s[pdf_id][up], sphere_angle);
            }
            return (fup + (dfc/1000 - up) * (flow - fup) / (low - up));
        }
    }
    else return 0;
}

double llhCone::fastEval(TGraph* g, double x) const { // TGraph eval is very slow, use own fast eval
    int idx = int(x*999./TMath::Pi());
    double xlow,ylow, xup, yup;
    g->GetPoint(idx,xlow,ylow);
    if(g->GetPoint(idx+1,xup,yup) < 0){
        return ylow;
    }
    else return (yup + (x - xup) * (ylow - yup) / (xlow - xup));
}

double llhCone::ASinCont(double x) const {
    double y;
    if(x>1) y=3.1415926535897;
    else if(x<0) y=0;
    else y=TMath::ASin(x);
    return y;
}