#include "ClusterFinder.h"
#include "WPChargeClusterRec/PmtProp.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>


ClusterFinder::ClusterFinder(PmtTable pmtTable){
	m_nPMT = 2307;
	m_writeCounter = 0;
	loadCharge(pmtTable);
	loadPosition(pmtTable);
	loadTime(pmtTable);
	//writeDataToTxt();	// function to output data for external event display
	removePmtAbove(getMedianTime());
	//writeDataToTxt();
	removeSingles();
	//writeDataToTxt();
	//removeSingles();
	//writeDataToTxt();
}

ClusterFinder::~ClusterFinder(){
	// Destructor, delete stuff
	delete [] ChargeMap;
	delete [] PositionMap;
	delete [] TimeMap;
	delete [] UsedMap;
}

void ClusterFinder::loadCharge(PmtTable pmtTable){
	ChargeMap=new double[m_nPMT];
	// Not needed!
	for(int i=0;i<m_nPMT;i++){
		ChargeMap[i]=0;
	}
	for(int i=0;i<m_nPMT;i++){
		if(pmtTable[i].used) ChargeMap[i] = pmtTable[i].q;
		else ChargeMap[i] = 0;
	}
}

void ClusterFinder::loadTime(PmtTable pmtTable){
	TimeMap=new int[m_nPMT];
	// Not needed!
	for(int i=0;i<m_nPMT;i++){
		TimeMap[i]=0;
	}
	for(int i=0;i<m_nPMT;i++){
		if(pmtTable[i].used) TimeMap[i] = pmtTable[i].fht;
		else TimeMap[i] = 0;
	}
}

double ClusterFinder::getMedianTime(){
	std::vector<int> times;
	for(int i=0;i<m_nPMT;i++){
		if(TimeMap[i] > 0)times.push_back(TimeMap[i]);
	}
	std::sort(times.begin(), times.end());
	int size = times.size();
	double median = size % 2 ? times.at(size / 2) : (times.at(size / 2 - 1) + times.at(size / 2)) / 2.;
	std::cout << "Median time is " << median << "ns" << std::endl;
	return median;
}

void ClusterFinder::loadPosition(PmtTable pmtTable){
	PositionMap=new TVector3[m_nPMT];
	for(int i=0;i<m_nPMT;i++){
		PositionMap[i]=TVector3(0,0,0);
	}
	for(int i=0;i<m_nPMT;i++){
		PositionMap[i] = pmtTable[i].pos;
	}
}

double ClusterFinder::getCharge(int pmtnum){
	return ChargeMap[pmtnum];
}

void ClusterFinder::setToZero(int pmtnum){
	ChargeMap[pmtnum]=0;
}

void ClusterFinder::removePmtAbove(double cut){
	for(int i=0;i<m_nPMT;i++){
		if(TimeMap[i] > cut) setToZero(i);
	}
}

void ClusterFinder::findSingles(){
	UsedMap=new int[m_nPMT];
	for(int i=0;i<m_nPMT;i++){
		std::vector< std::pair<double,int> > distances = getNeighbourVector(i);
		int counter=0;
		for(int j=0; j<8; j++){
            if(getCharge(distances.at(j).second) > 0) counter++;
        }
        if(counter>4) UsedMap[i] = 1;
        else UsedMap[i] = 0;
	}
}

void ClusterFinder::removeSingles(){
	findSingles();
	for(int i=0;i<m_nPMT;i++){
		if(UsedMap[i] == 1) continue;
		else if(UsedMap[i] == 0) setToZero(i);
		else std::cout << "Used Map is broken at entry " << i << " with value " << UsedMap[i] <<"!" << std::endl;
	}
}

void ClusterFinder::writeDataToTxt(){
    std::ofstream myfile;
    std::stringstream filename;
    filename << "cfinder_out_" << m_writeCounter <<".txt";
    myfile.open(filename.str().c_str());
    for(unsigned int pid = 0; pid < m_nPMT; ++pid){
        myfile  << pid << " "
            << PositionMap[pid].X() << " "
            << PositionMap[pid].Y() << " "
            << PositionMap[pid].Z() << " "
            << getCharge(pid) << " "
            << TimeMap[pid] << std::endl;
        
    }
    myfile.close();
    m_writeCounter++;
}

/*int *ClusterFinder::getCluster(){	// old, too complicated
	int maxCid=0;
	double maxCharge=0;
	for(int i=0;i<m_nPMT;i++){
		double charge = getCharge(i);
		if(charge>maxCharge){
			maxCharge=charge;
			maxCid=i;
		 }
	}
	std::cout << "Max Charge is "<< maxCharge << " PE in PMT#"<< maxCid << " with Position ("<< PositionMap[maxCid].X() <<","<<PositionMap[maxCid].Y() <<","<<PositionMap[maxCid].Z() <<")"<< std::endl;
	int * Cluster=new int[100];
	int nPMTsInCluster=0;
	AddPmtToCluster(Cluster, &nPMTsInCluster, maxCid, maxCharge);
	std::cout << nPMTsInCluster << std::endl;

	Cluster[0]=nPMTsInCluster;

	return Cluster;
}*/

/*void ClusterFinder::AddPmtToCluster(int *Cluster, int *nPMTsInCluster, int pmtID, double MaxCharge){ 	//old used by getCluster()
	if(getCharge(pmtID)>0.20*MaxCharge && *nPMTsInCluster<40){ // TUNING PARAMETERS!
		Cluster[(*nPMTsInCluster)+1] = pmtID;
		int *Neighbours = getNeighbours(pmtID);
		(*nPMTsInCluster)++;
		setToZero(pmtID);
		std::cout << "There are " << Neighbours[0] << " neigbours" << std::endl;
		for(int i=1;i<=Neighbours[0];i++){
			AddPmtToCluster(Cluster,nPMTsInCluster,Neighbours[i],MaxCharge);
		}
		delete [] Neighbours;
	}
}*/

/*int * ClusterFinder::getNeighbours(int pmtnum){		// old function, used my AddPmtToCluster(...)
	int counter=0;
	int *tmp=new int[m_nPMT];

	TVector3 refVec=PositionMap[pmtnum];
	std::vector<double>distances;

	for(int j=0;j<m_nPMT;j++){
		if(j==pmtnum) continue;
		double dist=(refVec-PositionMap[j]).Mag();
		distances.push_back(dist);
		if(dist<=3000){
			tmp[counter]=j;
			counter++;
		}
	}
	std::sort(distances.begin(), distances.end());
	for(int i=0;i<10;i++) std::cout << distances.at(i) << std::endl;
	int *result=new int[counter+1];
	result[0]=counter;
	for(int i=1;i<=counter;i++){
		result[i]=tmp[i-1];
	}

	delete [] tmp;

	return result;
}*/

std::vector< std::pair<double,int> > ClusterFinder::getNeighbourVector(int pmtnum){		// new neighbour-finder, faster with look-up table!
	std::vector< std::pair<double,int> > distances;
    TVector3 refPmt = PositionMap[pmtnum];
    for(int nId=0; nId < m_nPMT; nId++){
        if(nId == pmtnum) continue;
        distances.push_back( std::make_pair( (PositionMap[nId] - refPmt).Mag(), nId) );
    }
    std::sort(distances.begin(), distances.end());
    return distances;
}

int *ClusterFinder::getClusterSimple(){		// current ClusterFinder logic

	int maxCid=0;
	double maxCharge=0;
	for(int i=0;i<m_nPMT;i++){
		double charge = getCharge(i);
		if(charge>maxCharge){
			maxCharge=charge;
			maxCid=i;
		 }
	}
	std::cout << "Max Charge is "<< maxCharge << " PE in PMT#"<< maxCid << " with Position ("<< PositionMap[maxCid].X() <<","<<PositionMap[maxCid].Y() <<","<<PositionMap[maxCid].Z() <<")"<< std::endl;
	int * Cluster=new int[10];
	int nPMTsInCluster=9;
	std::vector< std::pair<double,int> > neighbours = getNeighbourVector(maxCid);
	Cluster[1] = maxCid;
	setToZero(maxCid);
	for(int i=0; i<nPMTsInCluster-1; i++){
		Cluster[i+2] = neighbours.at(i).second;
		setToZero(neighbours.at(i).second);
	}
	Cluster[0]=nPMTsInCluster;
	//writeDataToTxt();
	return Cluster;
}



