#ifndef _INPUT_
#include <TTree.h>
#include <TFile.h>
#include "event.h"

class TSNEventsInput
{
public:
	TSNEventsInput(const char *filename);
	~TSNEventsInput();
	int getEventsNumber();
	void getEvent(int nEntry);
private:
	TTree *evttree;
	TFile *evtfile;
};

#define _INPUT_
#endif
