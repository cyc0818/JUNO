#include "readFromRecAlg.h"

#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/CdVertexRecHeader.h"
#include "Event/CdTrackRecHeader.h"
#include "Event/WpRecHeader.h"
#include "Event/TtRecHeader.h"
#include "Event/OecHeader.h"

#include "TMath.h"
#include <TVector3.h>
DECLARE_ALGORITHM(readFromRecAlg); 

readFromRecAlg::readFromRecAlg(const std::string& name)
:   AlgBase(name){
    m_energy=new CriteriaItemValue;//NULL;
    m_detID=new CriteriaItemValue;//NULL;
    m_vtxR = new CriteriaItemValue;//NULL;
    m_dEdx = new CriteriaItemValue;

}

readFromRecAlg::~readFromRecAlg(){
    if (m_energy!=NULL) delete m_energy;
    if (m_detID!=NULL) delete m_detID;
    if (m_vtxR!=NULL) delete m_vtxR;
    if (m_dEdx!=NULL) delete m_dEdx;
}

bool readFromRecAlg::initialize(){
    //for single event tagging
    SniperPtr<EvtStoreSvc>  stsvc(dynamic_cast<Task*>(getRoot()->find("LEC_Task")), "EvtStoreSvc");
    if ( stsvc.invalid() ) {
        LogError << "cannot get the EvtStoreSvc." << std::endl;
        return false;
    }
    m_storeSvc = stsvc.data();
    if (!m_storeSvc->put("detectorID",m_detID)){
        LogError<<"error when detector ID!!!!!!"<<endl;
        return false;
    }
    if (!m_storeSvc->put("energy",m_energy)){
        LogError<<"error when put energy!!!!!!"<<endl;
        return false;
    }
    if (!m_storeSvc->put("R",m_vtxR)){
        LogError<<"error when put vtxR!!!!!!"<<endl;
        return false;
    }
    if (!m_storeSvc->put("dEdx",m_dEdx)){
        LogError<<"error when put vtxR!!!!!!"<<endl;
        return false;
    }
    
    SniperDataPtr<JM::NavBuffer> navBuf(dynamic_cast<Task*>(getRoot()->find("LEC_Task")), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    LogInfo<<"initialze readFromRecAlg successfully!"<<std::endl;
    return true; 
}

bool readFromRecAlg::execute(){
    //get rec event
    JM::EvtNavigator* nav = m_buf->curEvt(); 
    TTimeStamp gtTime=nav->TimeStamp();
    LogDebug<<"Time of navigator is:"<<gtTime<<std::endl;

    //Get the headers to be used
    auto header_cdrec = JM::getHeaderObject<JM::CdVertexRecHeader>(nav);
    auto header_cdrec_track = JM::getHeaderObject<JM::CdTrackRecHeader>(nav);
    auto header_wprec = JM::getHeaderObject<JM::WpRecHeader>(nav);
    auto header_ttrec = JM::getHeaderObject<JM::TtRecHeader>(nav);
    if (header_cdrec==NULL && header_cdrec_track==NULL && header_wprec==NULL && header_ttrec==NULL){
        LogWarn<<"Rec headers are not found! skip this execute!"<<std::endl;
        return true;
    }

    //get OECEvent to be modified
    JM::OecHeader* header_oec = JM::getHeaderObject<JM::OecHeader>(nav);
    if (header_oec==NULL){
        LogError<<"The OecHeader is not found!"<<std::endl;
        return false;
    }
    JM::OecEvt* eventOEC = dynamic_cast<JM::OecEvt*>(header_oec->event("JM::OecEvt"));
    if (header_cdrec!=NULL){
        m_detID->reset();
        m_detID->setValue(1);
        //Get the cd rec event
        auto eventRec = header_cdrec->event();

        auto vertex = eventRec->getVertex(0);

        double denergy=vertex->energy();
        double dvtxX=vertex->x();///1000.;
        double dvtxY=vertex->y();///1000.;
        double dvtxZ=vertex->z();///1000.;
        LogDebug<<"Energy and Vertex: ("<<dvtxX<<", "<<dvtxY<<", "<<dvtxZ<<", "<<denergy<<")"<<std::endl;

        eventOEC->setVertexX(dvtxX);
        eventOEC->setVertexY(dvtxY);
        eventOEC->setVertexZ(dvtxZ);
        eventOEC->setEnergy(denergy);
        eventOEC->setTime(gtTime);

        m_energy->reset();
        m_energy->setValue(denergy);
        m_vtxR->reset();
        m_vtxR->setValue(TMath::Sqrt(dvtxX*dvtxX+dvtxY*dvtxY+dvtxZ*dvtxZ));
        LogDebug<<"energy and vertex are put successfully!"<<std::endl;
    }
    if (header_cdrec_track!=NULL){
       m_detID->reset();
       m_detID->setValue(1);

       auto eventRec_track = header_cdrec_track->event();
       int track_num = eventRec_track->nTracks();
       TotaltrackL = 0;
       TotalPE = 0;
       MuID.clear();
       MuInX.clear();
       MuInY.clear();
       MuInZ.clear();
       MuOutX.clear();
       MuOutY.clear();
       MuOutZ.clear();
       for(int i=0;i<track_num; i++){
           auto track = eventRec_track->getTrack(i);
           TVector3 recInit, recExit;
           CLHEP::HepLorentzVector st = track->start();
           CLHEP::HepLorentzVector ed = track->end();
           recInit.SetXYZ(st[0], st[1], st[2]);
           recExit.SetXYZ(ed[0], ed[1], ed[2]);
           TotaltrackL += (recInit-recExit).Mag();
           TotalPE = track->peSum();
           MuID.push_back(i);
           MuInX.push_back(recInit[0]);
           MuInY.push_back(recInit[1]);
           MuInZ.push_back(recInit[2]);
           MuOutX.push_back(recExit[0]);
           MuOutY.push_back(recExit[1]);
           MuOutZ.push_back(recExit[2]);
       }
       eventOEC->setMuID(MuID);
       eventOEC->setMuInX(MuInX);
       eventOEC->setMuInY(MuInY);
       eventOEC->setMuInZ(MuInZ);
       eventOEC->setMuOutX(MuOutX);
       eventOEC->setMuOutY(MuOutY);
       eventOEC->setMuOutZ(MuOutZ);
       m_energy->reset();
       m_energy->setValue(TotalPE/1677);  // 1677 p.e./MeV from doc-8631 for J22-rc0
       m_dEdx->reset();
       m_dEdx->setValue(TotalPE/(TotaltrackL*0.1));
       //n_cluster->reset();
       //n_cluster->setValue(ClusterNum);
    }

    if (header_wprec!=NULL){
        m_detID->reset();
        m_detID->setValue(2);
    }
    if (header_ttrec!=NULL){
        m_detID->reset();
        m_detID->setValue(3);
    }

    return true;
}
    
bool readFromRecAlg::finalize(){
    LogInfo<<"finalize readFromRecAlg!"<<std::endl;
    return true;
}

