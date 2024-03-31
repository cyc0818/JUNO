#include <stdlib.h>
#include <iostream>
#include <TSystem.h>
using namespace std;

void printENV(){
	// We provide some environment variables
	// top input dir: JUNOTEST_TOPDIR_INPUT 
	// tag list: JUNOTEST_TAG_LIST, different tags are seperated by ":"
	cout << "Top Dir = "<<gSystem->Getenv("JUNOTEST_TOPDIR_INPUT") << endl;
	cout << "Tag List = " << gSystem->Getenv("JUNOTEST_TAG_LIST") << endl;
	// Each analysis step has its own input/output environment variables
	// detsim : JUNOTEST_DETSIM_ANA_INPUT/OUTPUT
	if(gSystem->Getenv("JUNOTEST_DETSIM_ANA_INPUT")){
	// add your code
		cout << "Detsim_ana input = " << gSystem->Getenv("JUNOTEST_DETSIM_ANA_INPUT") << endl;
	}
	if(gSystem->Getenv("JUNOTEST_DETSIM_ANA_OUTPUT")){
		cout << "Detsim_ana output = " << gSystem->Getenv("JUNOTEST_DETSIM_ANA_OUTPUT") << endl;
	}
	// elecsim : JUNOTEST_ELECSIM_ANA_INPUT/OUTPUT
	if(gSystem->Getenv("JUNOTEST_ELECSIM_ANA_INPUT")){
	// add your code
		cout << "Elecsim_ana input = " << gSystem->Getenv("JUNOTEST_ELECSIM_ANA_INPUT") << endl;
	}
	if(gSystem->Getenv("JUNOTEST_ELECSIM_ANA_OUTPUT")){
		cout << "elecsim_ana output = " << gSystem->Getenv("JUNOTEST_ELECSIM_ANA_OUTPUT") << endl;
	}
	// calib : JUNOTEST_CALIB_ANA_INPUT/OUTPUT
	if(gSystem->Getenv("JUNOTEST_CALIB_ANA_INPUT")){
	// add your code
		cout << "Calib_ana input = " << gSystem->Getenv("JUNOTEST_CALIB_ANA_INPUT") << endl;
	}
	if(gSystem->Getenv("JUNOTEST_CALIB_ANA_OUTPUT")){
		cout << "calib_ana output = " << gSystem->Getenv("JUNOTEST_CALIB_ANA_OUTPUT") << endl;
	}
	// rec : JUNOTEST_REC_ANA_INPUT/OUTPUT
	if(gSystem->Getenv("JUNOTEST_REC_ANA_INPUT")){
	// add your code
		cout << "Rec_ana input = " << gSystem->Getenv("JUNOTEST_REC_ANA_INPUT") << endl;
	}
	if(gSystem->Getenv("JUNOTEST_REC_ANA_OUTPUT")){
		cout << "rec_ana output = " << gSystem->Getenv("JUNOTEST_REC_ANA_OUTPUT") << endl;
	}

}
