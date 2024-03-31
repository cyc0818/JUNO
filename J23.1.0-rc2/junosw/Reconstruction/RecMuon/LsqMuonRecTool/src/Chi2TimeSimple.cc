#include "Chi2TimeSimple.h"
#include "math.h"
#include "firstHitTimeExpected.h"
Chi2TimeSimple::Chi2TimeSimple(const PmtTable& ptab)
  :m_ptable(ptab)
   ,m_LSRadius(17700)
   ,m_vmuon(299.)
   ,m_nLSlight(1.485)
   ,m_clight(299.)
{}
double 
Chi2TimeSimple::operator()(const double*xx){
  /* xx[0] : ri_theta
   * xx[2] : ri_t
   * xx[1] : ri_phi
   * xx[3] : dir_theta
   * xx[4] : dir_phi
   * */
  double chisq = 0;
  TVector3 ri,direction;
  ri.SetMagThetaPhi(m_LSRadius,xx[0],xx[1]);
  direction.SetMagThetaPhi(1,xx[3],xx[4]);
  if (ri*direction){
    chisq = 1e4;
  }
  double ti = xx[2];
  double tl = 2*fabs(ri*direction);

  Params pars;
  pars.set("LSRefraction",m_nLSlight);
  pars.set("MuonSpeed",m_vmuon);
  pars.set("LightSpeed",m_clight);
  double numPmtUsed = 0;
  unsigned int pmtNum = m_ptable.size();
  for (unsigned int ipmt = 0; ipmt < pmtNum; ++ipmt) {
    if(not m_ptable[ipmt].used) continue;
    double fhtexp = firstHitTimeExpected(ri,ti,direction,tl,m_ptable[ipmt],pars);
    chisq += (fhtexp-m_ptable[ipmt].fht)*(fhtexp-m_ptable[ipmt].fht)/(m_ptable[ipmt].res*m_ptable[ipmt].res);
    numPmtUsed += 1;
  }
  double ndf = numPmtUsed-5;
  return chisq/ndf;
}
