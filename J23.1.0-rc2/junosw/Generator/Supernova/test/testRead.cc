#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "sneventsinput.h"
using namespace std;
int main(int argc, char **argv)
{
	if(argc != 2)
	{
		cerr<<"Need filename"<<endl;
		exit(1);
	}

	ofstream ibde, pese, eese, n12pe, n12de, b12pe, b12de, ibdt, pest, eest, n12t, b12t, c12t;

	TSNEventsInput *input = new TSNEventsInput(argv[1]);
	for(int i = 0; i < input->getEventsNumber(); i++)
	{
		input->getEvent(i);
		for(int j = 0; j < evt.nparticles; j++)
		{
//			cout<<evt.t[j]<<"\t"<<evt.pdgid[j]<<"\t"
			cout<<sqrt(evt.px[j] * evt.px[j] + evt.py[j] * evt.py[j] 
				+ evt.pz[j] * evt.pz[j] + evt.m[j] * evt.m[j])<<"\t"
				<<evt.pz[j] / sqrt(evt.px[j] * evt.px[j] + evt.py[j] * evt.py[j] 
                                + evt.pz[j] * evt.pz[j])<<"\t";
		}
		cout<<endl;
	}
	delete input;
	return 0;
}
