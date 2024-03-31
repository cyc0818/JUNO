#include "RecTools/IRecMuonTool.h"

RecTrks::RecTrks() {}

RecTrks::~RecTrks() {}

int RecTrks::size() {
	return Trks.size();
}

TVector3 RecTrks::getStart(int id) {
	return Trks[id].st;
}

TVector3 RecTrks::getEnd(int id) {
	return Trks[id].ed;
}

double RecTrks::getNPE(int id) {
	return Trks[id].nPE;
}

double RecTrks::getQuality(int id) {
	return Trks[id].quality;
}

double RecTrks::getITime(int id) {
	return Trks[id].iTime;
}

double RecTrks::getOTime(int id) {
	return Trks[id].oTime;
}

int RecTrks::getClusterNum(int id){
  return Trks[id].cluster;
}

void RecTrks::addTrk(TVector3 st, TVector3 ed, double it, double ot, double npe, double qua, int cluster_num) {
	Trk trk;
	trk.st = st;
	trk.ed = ed;
	trk.nPE = npe;
	trk.quality = qua;
	trk.iTime = it;
	trk.oTime = ot;
  trk.cluster = cluster_num;
	Trks.push_back(trk);
}
