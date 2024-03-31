#include "Chi2TimeSimple.h"
#include "math.h"
#include "firstHitTimeExpected.h"
	Chi2TimeSimple::Chi2TimeSimple(const PmtTable& ptab)
	:m_ptable(ptab)
	,m_LSRadius(17700)
	,m_vmuon(299.)
	,m_nLSlight(1.485)
	 ,m_clight(299.)
{
	int nPMT = ptab.size();
	// for (int i = 0; i < nPMT; i ++) {
	// 	if (m_Wpmaxq < ptab[i].q) {
	// 		m_Wpmaxq = ptab[i].q;
	// 	}
	// }
}
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
	// ri = GetInciPos(ri, direction);
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
	double dis = TMath::Sqrt(TMath::Power(ri.Mag(), 2) - TMath::Power(ri * direction, 2));
	double CdWeight = 1;
	double WpWeight = 1;
	m_Wpmaxq = 0;
	for (int i = 0; i < pmtNum; i ++) {
		if (m_Wpmaxq < m_ptable[i].q)
			m_Wpmaxq = m_ptable[i].q;
	}
	// if (dis > 16000) {
	//   WpWeight = 0.8;
	// }
	for (unsigned int ipmt = 0; ipmt < pmtNum; ++ipmt) {
		if(not m_ptable[ipmt].used) continue;
		// if (m_ptable[ipmt].loc == 2) continue;
		double fhtexp = firstHitTimeExpected(ri,ti,direction,tl,m_ptable[ipmt],pars);
		// if (fhtexp == -1)
		// 	continue;
		// chisq += (fhtexp-m_ptable[ipmt].fht)*(fhtexp-m_ptable[ipmt].fht)/(m_ptable[ipmt].res*m_ptable[ipmt].res);
		// numPmtUsed ++;
		if (m_ptable[ipmt].loc == 1) {
			chisq += CdWeight * (fhtexp-m_ptable[ipmt].fht)*(fhtexp-m_ptable[ipmt].fht)/(m_ptable[ipmt].res*m_ptable[ipmt].res);
			numPmtUsed += CdWeight;
		}
		else {
			// WpWeight = m_ptable[ipmt].q / 800;
			WpWeight = m_ptable[ipmt].q / m_Wpmaxq;
			chisq += WpWeight * (fhtexp-m_ptable[ipmt].fht)*(fhtexp-m_ptable[ipmt].fht)/(m_ptable[ipmt].res*m_ptable[ipmt].res);
			numPmtUsed += WpWeight;
		}
	}
	double ndf = numPmtUsed-5;
	return chisq/ndf;
}

// TVector3 Chi2TimeSimple::GetInciPos(TVector3 ri, TVector3 dir) {
// 	dir = dir.Unit();
// 	double len1 = abs(ri * dir);
// 	double dis = sqrt(ri.Mag2() - len1 * len1);
// 	double len2 = sqrt(pow(m_LSRadius, 2) - dis * dis);
// 	return ri + dir * (len1 - len2);
// }
