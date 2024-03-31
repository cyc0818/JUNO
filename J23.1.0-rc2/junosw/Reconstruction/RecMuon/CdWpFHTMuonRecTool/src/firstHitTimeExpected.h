#include "RecTools/PmtProp.h"
#include "RecTools/Params.h"
#include"TVector3.h"

#define XL 25000
#define YL 25000
#define ZL 25000

double firstHitTimeExpected(TVector3/*injecting point*/,
		double/*injecting time*/,
		TVector3/*track direction*/,
		double/*track length in lS*/,
		const PmtProp&/*pmt info*/,
		const Params& pars/*transmit in the env parameters:
							Params pars;
							pars.set("LightSpeed",299.) //speed of light, eg. 299.0
							pars.set("MuonSpeed",299.) //speed of muon, eg. 299.0
							pars.set("LSRefraction",1.485) // refraction index of light in the LS, eg. 1.485
							*/
		);
