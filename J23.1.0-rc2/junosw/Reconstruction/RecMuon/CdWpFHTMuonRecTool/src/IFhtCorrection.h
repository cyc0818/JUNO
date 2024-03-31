#ifndef IFhtCorrection_h
#define IFhtCorrection_h 1
#include "RecTools/PmtProp.h"

class IFhtCorrection
{
	public:

		virtual double correction(const PmtProp& pmt, const TVector3& ri, const TVector3& dir) const = 0;
		virtual double sigma(const PmtProp& pmt,const TVector3&ri,const TVector3& dir, double* error_compensation = 0 ) const = 0;
		virtual bool setCorrectionFileAndNumBinDist(std::string file, int nbin) = 0;
		virtual ~IFhtCorrection ();
};
#endif /* ifndef IFhtCorrection_h */
