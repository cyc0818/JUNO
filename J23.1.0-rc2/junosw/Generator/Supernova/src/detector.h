#ifndef _DETECTOR_
#include <vector>
#include <Math/Interpolator.h>
#include "def.h"

class TDetector
{
public:
	TDetector(double mass, double hRatio);	//kton
	~TDetector();
	/*Cross-sections are in cm^2 or cm^2/MeV*/
	double GetIBD(double eNu, double costheta);
	double GetProtonES(double eNu, double eP, bool isAnti);
	double GetElectronES(double eNu, double eE, NuType type);
	double GetTotalProtonES(double eNu, bool isAnti);
	double GetTotalElectronES(double eNu, NuType type);
	double GetN12(double eNu);
	double GetB12(double eNu);
	double GetC12(double eNu, bool isAnti);
private:
	double hnum;
	double cnum;
	ROOT::Math::Interpolator *n12, *b12, *c12nu, *c12nubar;
};
#define _DETECTOR_
#endif
