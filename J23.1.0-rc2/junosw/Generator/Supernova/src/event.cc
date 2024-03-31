#include "event.h"
#include <iostream>

using namespace std;
EVENT evt;
void Myevent::Set(EVENT &mEvent)
{
	privevt.nparticles = mEvent.nparticles;
	for(int i = 0; i < 2; i++)
	{
		privevt.pdgid[i] = mEvent.pdgid[i];
		privevt.px[i] = mEvent.px[i];	//MeV
		privevt.py[i] = mEvent.py[i];	//MeV
		privevt.pz[i] = mEvent.pz[i];	//MeV
		privevt.m[i] = mEvent.m[i];	//MeV
		privevt.t[i] = mEvent.t[i];	//ns
	}
  privevt.nuE = mEvent.nuE;
  privevt.origPDGID = mEvent.origPDGID;
}
void Myevent::Get(EVENT &mEvent)
{
	mEvent.nparticles = privevt.nparticles;
	for(int i = 0; i < 2; i++)
	{
		mEvent.pdgid[i] = privevt.pdgid[i];
		mEvent.px[i] = privevt.px[i];	//MeV
		mEvent.py[i] = privevt.py[i];	//MeV
		mEvent.pz[i] = privevt.pz[i];	//MeV
		mEvent.m[i] = privevt.m[i];	//MeV
		mEvent.t[i] = privevt.t[i];	//ns
	}
  mEvent.nuE = privevt.nuE;
  mEvent.origPDGID = privevt.origPDGID;
}

double Myevent::GetTime()
{
	return privevt.t[0];
}

bool Myevent::operator<(Myevent &e2)
{
	return (privevt.t[0] < e2.GetTime());
}
bool Myevent::operator>(Myevent &e2)
{
	return (privevt.t[0] > e2.GetTime());
}
bool Myevent::operator<=(Myevent &e2)
{
	return (privevt.t[0] <= e2.GetTime());
}
bool Myevent::operator>=(Myevent &e2)
{
	return (privevt.t[0] >= e2.GetTime());
}
void Myevent::operator=(Myevent &e2)
{
	EVENT tmp;
	e2.Get(tmp);
	Set(tmp);
}

