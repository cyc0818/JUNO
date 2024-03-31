/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn
# Last modified: 2022-03-01 14:44
# Filename: IReconTool.h is copied from RecCdMuonAlg and rename to IRecMuonTool
# Description: 
=============================================================================*/
#ifndef IRECMUONTOOL_H
#define IRECMUONTOOL_H
#include "RecTools/PmtProp.h"
#include "RecTools/Params.h"
#include "TVector3.h"
#include <vector>
#include <algorithm>

struct Trk {
	TVector3 st;
	TVector3 ed;
	double nPE;
	double quality;
	double iTime;
	double oTime;
  int cluster;
};

class RecTrks {
	public:
		RecTrks();
		~RecTrks();
		int size();
		TVector3 getStart(int);
		TVector3 getEnd(int);
		double getNPE(int);
		double getQuality(int);
		double getITime(int);
		double getOTime(int);
		int getClusterNum(int);
		void addTrk(TVector3, TVector3, double, double, double, double, int);
	private:
		std::vector<Trk> Trks;
};

namespace JM{
    class RecHeader; 
}
class IRecMuonTool 
{
    public:

        virtual bool reconstruct(RecTrks*) = 0; 

        virtual bool configure(const Params*, const PmtTable*) = 0; 

        virtual ~IRecMuonTool(){}; 
}; 
#endif
