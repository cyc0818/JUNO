#ifndef Chi2TimeSimple2_h
#define Chi2TimeSimple2_h 1
#include "RecTools/PmtProp.h"

class Chi2TimeSimple2
{
	public:
		Chi2TimeSimple2(const PmtTable&, TVector3);
		double operator()(const double* xx);

		void setWpMaxQ(double q) {m_Wpmaxq = q;}
		void setLSRadius(double r){m_LSRadius = r;}
		void setMuonSpeed(double v){m_vmuon = v;}
		void setLightSpeed(double c){m_clight = c;}
		void setLSRefraction(double index){m_nLSlight = index;}

	private:
		const PmtTable& m_ptable;

		double m_LSRadius;
		double m_Wpmaxq;
		double m_vmuon;
		double m_nLSlight;
		double m_clight;
		TVector3 dir;
};
#endif /* ifndef Chi2TimeSimple2_h */
