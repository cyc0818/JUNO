#ifndef AFTERDEEXINFO_hh
#define AFTERDEEXINFO_hh

#include <vector>
#include <ostream>
struct  AfterDeexInfo{

	AfterDeexInfo();
	std::vector<double> Energy;
    std::vector<int>    Pdg;
    int  ResidualZ;
    int  ResidualN;
    int  DeexChannelID;
    int  NPars;
    int  ResidualPdg;
 };


std::ostream &operator<<(std::ostream & os , const AfterDeexInfo & adi);

#endif
