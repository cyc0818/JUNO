#include "WPChargeClusterRec.h"
#include "WPChargeClusterRec/PmtProp.h"
#include "TVector3.h"
#include <vector>

class ClusterFinder{
	
	public:
	ClusterFinder(PmtTable pmtTable);

	~ClusterFinder();
	//int *getCluster();
	int *getClusterSimple();

	private:

	void loadCharge(PmtTable pmtTable);
	void loadPosition(PmtTable pmtTable);
	void loadTime(PmtTable pmtTable);
	double getMedianTime();
	double getCharge(int pmtnum);
	void setToZero(int pmtnum);
	void removePmtAbove(double cut);
	void findSingles();
	void removeSingles();
	//void AddPmtToCluster(int *Cluster, int *nPMTsInCluster, int pmtID, double MaxCharge);
	//int * getNeighbours(int pmtnum);
	std::vector< std::pair<double,int> > getNeighbourVector(int pmtnum);
	void writeDataToTxt();

	int m_nPMT;
	int m_writeCounter;
	double *ChargeMap;
	int *TimeMap;
	int *UsedMap;
	TVector3 *PositionMap;

};
