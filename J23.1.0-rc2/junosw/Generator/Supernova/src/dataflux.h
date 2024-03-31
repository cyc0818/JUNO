#ifndef _DATAFLUX_
#include <vector>
#include <cmath>
#include "def.h"

typedef struct
{
	double eStart;
	double eEnd;
	double fluxNuE;
	double fluxNuBarE;
	double fluxNuX;
	double lumNuE;
	double lumNuBarE;
	double lumNuX;
} SPECTRUM;

typedef struct
{
	double time;	//s
	SPECTRUM spectrum[60];
} NODE;

class TDataFlux
{
public:
	TDataFlux(const char *filename, double _D, int _isNH, bool _isSN);
	~TDataFlux();
	double GetLocalFlux(double t, double e, NuType type);
	double interpolate(double t, double e, NuType type);
private:
	std::vector <NODE> data;
    int isNH;
	double D;
    int Nepos;

};
#define _DATAFLUX_
#endif
