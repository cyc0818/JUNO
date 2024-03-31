#ifndef _EVENT_
#include <TTree.h>
typedef struct
{
	ULong64_t nparticles;
	Int_t pdgid[2];
	Double_t px[2];	//MeV
	Double_t py[2];	//MeV
	Double_t pz[2];	//MeV
	Double_t m[2];	//MeV
	Double_t t[2];	//ns    
	Double_t nuE;	//MeV
  Int_t origPDGID; //neutrino (mu & tau = 14, anti mu & anti tau = -14)
} EVENT;

class Myevent
{
public:
	void Set(EVENT &mEvent);
	void Get(EVENT &mEvent);
	double GetTime();

	bool operator<(Myevent &e2);
	bool operator>(Myevent &e2);
	bool operator<=(Myevent &e2);
	bool operator>=(Myevent &e2);
	void operator=(Myevent &e2);
	
private:
	EVENT privevt;
};


extern EVENT evt;

#define _EVENT_
#endif
