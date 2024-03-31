#ifndef _OUTPUT_
#include <TTree.h>
#include <TFile.h>
#include "event.h"
class TSNEventsOutput
{
public:
	TSNEventsOutput(const char *filename);
	~TSNEventsOutput();
	void pushEvents();
	void writeFile();
private:
	TTree *evttree;
	TFile *evtfile;
};

#define _OUTPUT_
#endif
