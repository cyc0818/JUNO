#include "WPChargeClusterRec.h"

#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/WpCalibHeader.h"
#include "Event/WpRecHeader.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/SimHeader.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/WpID.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "ClusterFinder.h"
#include "TFile.h"
#include "TMath.h"
#include "TTree.h"
#include "TVector3.h"
#include "TH3D.h"
#include "TH2D.h"

DECLARE_ALGORITHM(WPChargeClusterRec); 

    WPChargeClusterRec::WPChargeClusterRec(const std::string& name)
    : AlgBase(name)
    , m_iEvt(1)
    , m_totPmtNum(0)
    , m_wpGeom(NULL)
    , m_buf(NULL)
{ 
    declProp("Pmt20inchTimeReso",  m_sigmaPmt = 8);
    declProp("MinChargeInCluster",  m_minCiC = 29);
    declProp("Use20inchPMT",m_flagUse20inch=true);
    declProp("OutputPmtPos",m_flagOpPmtpos=false);
    declProp("UserOutfile", m_userOutfile = "userOutfile.root");
}
WPChargeClusterRec::~WPChargeClusterRec(){}


bool WPChargeClusterRec::initialize(){

    if(not iniBufSvc())return false; 
    if(not iniGeomSvc())return false; 
    if(not iniPmtPos())return false;

    LogInfo  << objName()
        << "   initialized successfully"
        << std::endl; 
    return true;
}

bool WPChargeClusterRec::finalize(){
    LogInfo  << objName()
        << "   finalized successfully" 
        << std::endl; 
    return true;
}

bool WPChargeClusterRec::execute(){
    LogInfo << "---------------------------------------" 
        << std::endl; 
    LogInfo << "Processing event by WPChargeClusterRec : " 
        << m_iEvt << std::endl; 

    if(not freshPmtData())return true;
    //printStats();
 
    int nClusters=0;
    int **clustersss=new int*[40];
    clustersss = findClustersSimple(&nClusters);
    LogDebug << "WPChargeClusterRec received " << nClusters << " Clusters for further work" << std::endl;
    /*LogDebug << "There are so many PMTs in each cluster: "<< std::endl;
    for(int i=0;i<nClusters;i++){
        LogDebug << "Cluster #"<< i << ": " << clustersss[i][0] << std::endl;
    }*/
    // Get mean positions
    TVector3 *meanPos=new TVector3[nClusters];
    double* meanCharge = new double[nClusters];
    for(int j=0;j<nClusters;j++){
        meanPos[j]=TVector3(0,0,0);
        double chargeSum=0;
        for(int i=1;i<=clustersss[j][0];i++){
            double charge = m_pmtTable[clustersss[j][i]].q;
            TVector3 v = m_pmtTable[clustersss[j][i]].pos;
            meanPos[j].SetX(meanPos[j].X()+v.X()*charge);
            meanPos[j].SetY(meanPos[j].Y()+v.Y()*charge);
            meanPos[j].SetZ(meanPos[j].Z()+v.Z()*charge);
            chargeSum+=charge;
            //std::cout << meanPos[j].X() << " " << meanPos[j].Y() << " "<< meanPos[j].Z() << std::endl;
        }
        //std::cout <<"charge: " <<chargeSum << std::endl;
        meanPos[j]=meanPos[j]*(1.0/chargeSum);
        meanCharge[j]=chargeSum/clustersss[j][0];
    }
    TVector3 *finalClusterPositions=new TVector3[nClusters];
    finalClusterPositions = mergeClusters(meanPos, meanCharge, &nClusters);
    LogDebug << "ClusterMerger left us with " << nClusters << " Clusters for fitting" << std::endl;
    if(nClusters>1){
        TVector3 *res=new TVector3[2];
        res[1]=finalClusterPositions[0]-finalClusterPositions[1];
        if(res[1].Theta()<1.57){
            res[1]=(-1*(res[1].Unit()));
            res[0]=finalClusterPositions[0];
        }else{
            res[1]=res[1].Unit();
            res[0]=finalClusterPositions[1];
        }
        //writeClusters(finalClusterPositions, meanCharge, nClusters);
        //exportClustersToTxt(finalClusterPositions, meanCharge, nClusters, 1);
        writeUserOutput(res[0], res[1], 0, 0);
    }
    else{
        // REDO with lower threshold
        m_minCiC = 15;
        int **clusters_redo=new int*[40];
        clusters_redo = findClustersSimple(&nClusters);

        TVector3 *meanPos_redo=new TVector3[nClusters];
        double* meanCharge_redo = new double[nClusters];
        for(int j=0;j<nClusters;j++){
            meanPos_redo[j]=TVector3(0,0,0);
            double chargeSum_redo=0;
            for(int i=1;i<=clusters_redo[j][0];i++){
                double charge = m_pmtTable[clusters_redo[j][i]].q;
                TVector3 v = m_pmtTable[clusters_redo[j][i]].pos;
                meanPos_redo[j].SetX(meanPos_redo[j].X()+v.X()*charge);
                meanPos_redo[j].SetY(meanPos_redo[j].Y()+v.Y()*charge);
                meanPos_redo[j].SetZ(meanPos_redo[j].Z()+v.Z()*charge);
                chargeSum_redo+=charge;
                //std::cout << meanPos[j].X() << " " << meanPos[j].Y() << " "<< meanPos[j].Z() << std::endl;
            }
            //std::cout <<"charge: " <<chargeSum << std::endl;
            meanPos_redo[j]=meanPos_redo[j]*(1.0/chargeSum_redo);
            meanCharge_redo[j]=chargeSum_redo/clusters_redo[j][0];
        }
        TVector3 *finalClusterPositions_redo=new TVector3[nClusters];
        finalClusterPositions_redo = mergeClusters(meanPos_redo, meanCharge_redo, &nClusters);

        //writeClusters(finalClusterPositions_redo, meanCharge_redo, nClusters);
        //exportClustersToTxt(finalClusterPositions, meanCharge, nClusters, 1);
        if(nClusters>1){
            TVector3 *res=new TVector3[2];
            res[1]=finalClusterPositions_redo[0]-finalClusterPositions_redo[1];
            if(res[1].Theta()<1.57){
                res[1]=(-1*(res[1].Unit()));
                res[0]=finalClusterPositions_redo[0];
            }else{
                res[1]=res[1].Unit();
                res[0]=finalClusterPositions_redo[1];
            }
            writeUserOutput(res[0], res[1], 0, 0);
        }
        else{
            writeUserOutput(finalClusterPositions_redo[0],(-1.*(finalClusterPositions_redo[0].Unit())), -1, 0);
        }
    }
    
    /*double re_dfc = (-res[0].Cross(res[1])).Mag();
    double re_theta = TMath::ACos(res[1].Z());
    LogDebug  << "Reconstructed Track has: " << std::endl;
    LogDebug  << "Distance from Center D = " << re_dfc/1000. << std::endl;
    LogDebug  << "Inclination Theta = " << re_theta*180./TMath::Pi() << std::endl;
    */
    //JM::RecTrackHeader * rh = new JM::RecTrackHeader;

    //JM::EvtNavigator* nav = m_buf->curEvt(); 
    //nav->addHeader("/Event/RecTrack", rh); 
    LogDebug  << "Done with finding charge clusters" << std::endl; 
    ++m_iEvt; 

    return true;
}

bool WPChargeClusterRec::iniBufSvc(){

    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" 
            << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    return true; 
}
bool WPChargeClusterRec::iniGeomSvc(){

    //Retrieve Geometry service
    SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc"); 
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" 
            << std::endl;
        return false;
    }
    m_wpGeom = rgSvc->getWpGeom(); 
    return true; 
}
bool WPChargeClusterRec::iniPmtPos(){

    m_totPmtNum = m_wpGeom->getPmtNum(); 
    m_pmtTable.reserve(m_totPmtNum); 
    m_pmtTable.resize(m_totPmtNum); 
    LogDebug << "Total Pmt num from GeomSvc : " 
        << m_totPmtNum << std::endl; 
    for(unsigned int pid=0;pid<m_totPmtNum;++pid){
        Identifier Id = Identifier(WpID::id(pid, 0)); 
        PmtGeom *pmt = m_wpGeom->getPmt(Id); 
        if(!pmt){
            LogError << "Wrong Pmt Id" << Id << std::endl; 
            return false; 
        }
        TVector3 pmtCenter = pmt->getCenter(); 

        m_pmtTable[pid].pos = pmtCenter;

        if(WpID::is20inch(Id)) {
            m_pmtTable[pid].res = m_sigmaPmt; 
            m_pmtTable[pid].type = _PMTINCH20; 
        }
        else {
            m_pmtTable[pid].type = _PMTNULL; 
            LogError  <<  "Pmt ["  <<  pid  
                <<  "] is in WP but not 20 inch!"  
                <<  std::endl; 
            return false; 
        }

    }

    //-----print out pmt positions  
    if(m_flagOpPmtpos){
        std::ofstream of("pmt_info.dat"); 
        for(unsigned int pid=0;pid<m_totPmtNum;++pid){
            of << pid << " "
                << m_pmtTable[pid].pos.X() << " " 
                << m_pmtTable[pid].pos.Y() << " " 
                << m_pmtTable[pid].pos.Z() 
                << std::endl;
        }
    }

    return true; 
}
bool WPChargeClusterRec::freshPmtData(){

    //reset values
    for (unsigned int pid = 0; pid < m_totPmtNum; ++pid)
    {
        m_pmtTable[pid].q=-1; 
        m_pmtTable[pid].fht=999999; 
        m_pmtTable[pid].used=false; 

    }

    //read CalibHit data
    auto nav = m_buf->curEvt(); 
    if(not nav){
        LogError << "Can not retrieve the current navigator!!!" 
            << std::endl; 
        return false; 
    }
    auto chcol =
        JM::getHeaderObject<JM::WpCalibHeader>(nav); 
    if(not chcol){
        LogError << "Can not retrieve \"/Event/Calib\" \
            from current navigator!!!" 
            << std::endl; 
        return false; 
    }
    const auto& chhlist = 
        chcol->event()->calibPMTCol(); 
    auto chit = chhlist.begin(); 

    while (chit!=chhlist.end()) {

        auto calib = *chit++; 
        Identifier id = Identifier(calib->pmtId());
        Identifier::value_type value = id.getValue(); 
        //std::cout << "CD/WP PMT. " << id << " " << ((value&0xFF000000)>>24 == 0x20)<< std::endl;
        if (not ((value&0xFF000000)>>24 == 0x20)) { //current 0x10 CD, 0x20 WP, 0x30 TT
            continue;
        } else {
            //LogDebug << "WP PMT. " << id << std::endl;
        }
        double nPE = calib->nPE(); 
        float firstHitTime = calib->firstHitTime(); 

        unsigned pid = WpID::module(id); 

        assert(pid<m_totPmtNum);
        m_pmtTable[pid].q = nPE;
        m_pmtTable[pid].fht =firstHitTime;
        if(
                (m_flagUse20inch  &&  WpID::is20inch(id)) 
          ) {
            m_pmtTable[pid].used = true; 
        }


        LogTest <<"PMT id"<<pid << "(" 
            << m_pmtTable[pid].pos.x() << "," 
            << m_pmtTable[pid].pos.y() << "," 
            << m_pmtTable[pid].pos.z() << ")"
            <<" ; nPE ="<<nPE<<" ;  firsthit ="<<firstHitTime<<std::endl;

    }

    LogDebug << "Loading calibrated data done !" << std::endl; 
    return true; 
}
bool WPChargeClusterRec::printStats(){
    double tmin = 9999999;
    TVector3 minPMT;
    TVector3 maxPMT;
    double tmax = 0;
    double qmin = 9999999;
    double qmax = 0;
    double counter = 0;
    int totalPE = 0;
    for (unsigned int pid = 0; pid < m_totPmtNum; ++pid){
        if(m_pmtTable[pid].used){
            if(m_pmtTable[pid].q > qmax) qmax = m_pmtTable[pid].q;
            if(m_pmtTable[pid].q < qmin) qmin = m_pmtTable[pid].q;
            if(m_pmtTable[pid].fht > tmax) {
                tmax = m_pmtTable[pid].fht;
                maxPMT = m_pmtTable[pid].pos;
            }
            if(m_pmtTable[pid].fht < tmin){
                tmin = m_pmtTable[pid].fht;
                minPMT = m_pmtTable[pid].pos;
            }
            totalPE += m_pmtTable[pid].q;
            counter++;
        }
    }
    LogDebug  << "#################################" << std::endl;
    LogDebug  << "############# Stats #############" << std::endl;
    LogDebug  << "### Fired WP PMT " << counter << std::endl;
    LogDebug  << "### Total PE in WP PMT " << totalPE << std::endl;
    LogDebug  << "### Earliest hit time " << tmin << " ns" << std::endl;
    LogDebug  << "### Latest  hit  time " << tmax << " ns" << std::endl;
    LogDebug  << "### Duration  " << tmax-tmin << " ns" << std::endl;
    LogDebug  << "### Distance of PMTs  " << (maxPMT - minPMT).Mag() << " mm" << std::endl;
    LogDebug  << "### Travel time between  " << (maxPMT - minPMT).Mag()/299.8 << " ns" << std::endl;
    LogDebug  << "### Minimal charge  " << qmin << " PE" << std::endl;
    LogDebug  << "### Maximal charge  " << qmax << " PE" << std::endl;
    LogDebug  << "#################################" << std::endl;
    return true;
}
bool WPChargeClusterRec::writeUserOutput(TVector3 re_R0, TVector3 re_dir, double minResult, double minVal){
    // Get Service
    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" 
            << std::endl;
        return false;
    }
    auto m_buf = navBuf.data();
    // Get Truth
    auto nav = m_buf->curEvt();
    auto simheader = JM::getHeaderObject<JM::SimHeader>(nav);
    auto se = (JM::SimEvt*)simheader->event();
    auto simtrk = (JM::SimTrack*) se->findTrackByTrkID(0);
    TVector3 mc_R0, mc_dir; 
    mc_R0.SetXYZ(simtrk->getInitX(), simtrk->getInitY(), simtrk->getInitZ());
    mc_dir.SetXYZ(simtrk->getInitPx(), simtrk->getInitPy(), simtrk->getInitPz());
    mc_dir = mc_dir.Unit();
    TFile* userOutfile = new TFile(m_userOutfile.c_str(),"UPDATE");
    TTree* t1 = new TTree("t1d","Tree with extracted RecResult");
    t1->Branch("mc_R0", &mc_R0);
    t1->Branch("mc_dir", &mc_dir);
    t1->Branch("re_R0", &re_R0);
    t1->Branch("re_dir", &re_dir);
    t1->Branch("minResult", &minResult);
    t1->Branch("minVal", &minVal);
    t1->Fill();
    userOutfile->Write();
    userOutfile->Close();
    return true;
}
TVector3 *WPChargeClusterRec::mergeClusters(TVector3 *meanPos, double *meanCharge, int *nClusters){
    // Setup output array
    TVector3 *out = new TVector3[(*nClusters)];
    double *outcharge = new double[(*nClusters)];
    for(int i=0;i<(*nClusters);i++){
        out[i] = meanPos[i];
        outcharge[i] = meanCharge[i];
    }
    int currentN=(*nClusters);
restart:
    LogDebug  << "Starting the comparison with " << currentN << " clusters" << std::endl;
    int counter = 0;
    int maxCount = sumToZero(currentN-1);
    while(counter<maxCount){
        for(int i=0;i<currentN;i++){
            for(int j=i+1;j<currentN;j++){
                double dist = (out[i]-out[j]).Mag();
                if(dist < 5700){
                    LogDebug  << "After " << counter << " comparisons two close clusters were found!" << std::endl;
                    //MERGE i and j, put it a place of i
                    //out[i] = 0.5*(meanPos[i]+meanPos[j]);
                    out[i] = 1./(outcharge[i]+outcharge[j])*(outcharge[i]*out[i] + outcharge[j]*out[j]);
                    outcharge[i] = 0.5*(outcharge[i]+outcharge[j]);
                    LogDebug  << "Merged Clusters " << i << " and " << j << " with a distance of " << dist/1000. << "m" << std::endl;
                    // shift all other values
                    for(int k=j+1; k<currentN; k++){
                        out[k-1] = out[k];
                        outcharge[k-1] = outcharge[k];
                    }
                    // restart with new values:
                    currentN--;
                    LogDebug  << "Now we have " << currentN << " clusters and restart" << std::endl;
                    goto restart;
                }
                else counter++;
            }
        }
    }
    LogDebug  << "Made " << counter << " comparisons of " << currentN << " clusters. Nothing more to merge" << std::endl;
    (*nClusters) = currentN;
    return out;
}
int WPChargeClusterRec::sumToZero(int start){
    int out=0;
    for(int i=start; i>0; i--) out = out+i;
    return out;
}
void WPChargeClusterRec::writeClusters(TVector3* clusterPos, double* meanCharge, int nClusters){ // only for debugging!
    std::vector<double> cx;
    std::vector<double> cy;
    std::vector<double> cz;
    std::vector<double> cq;
    for(int i=0; i<nClusters; i++){
        cx.push_back(clusterPos[i].X());
        cy.push_back(clusterPos[i].Y());
        cz.push_back(clusterPos[i].Z());
        cq.push_back(meanCharge[i]);
    }

    /*getMCTruth();
    double peEntry, peExit;
    peEntry = getCharge(0);
    peExit = getCharge(1);*/

    int *chargeId = get2HighCharges();
    int charge1= m_pmtTable[chargeId[0]].q;
    int charge2= m_pmtTable[chargeId[1]].q;
    double dist1 = getDist(chargeId[0]);
    double dist2 = getDist(chargeId[1]);


    TFile* clusterFile = new TFile(m_userOutfile.c_str(),"UPDATE");
    TTree* t1 = new TTree("t1c","Tree with clusters");
    t1->Branch("cx", &cx);
    t1->Branch("cy", &cy);
    t1->Branch("cz", &cz);
    t1->Branch("cq", &cq);
    t1->Branch("nClusters", &nClusters);
    t1->Branch("charge1", &charge1);
    t1->Branch("charge2", &charge2);
    t1->Branch("dist1", &dist1);
    t1->Branch("dist2", &dist2);
    t1->Fill();
    //hPMTAll->Write();
    clusterFile->Write();
    clusterFile->Close();
}
void WPChargeClusterRec::exportClustersToTxt(TVector3* clusterPos, double* meanCharge, int nClusters, int option){ // only for debugging!
    std::ofstream myfile;
    std::stringstream filename;
    std::string rootname = m_userOutfile.substr(0, m_userOutfile.size()-5);
    if(option == 0) filename << rootname << "_clusters.txt";
    else if(option == 1) filename << rootname << "_mergedclusters.txt";
    else filename << "wrongOption.txt";
    myfile.open(filename.str().c_str());
    for(int i=0; i<nClusters; i++){
        myfile  << i << " "
                << clusterPos[i].X() << " "
                << clusterPos[i].Y() << " "
                << clusterPos[i].Z() << " "
                << meanCharge[i] << std::endl; 
    }
    myfile.close();
}
void WPChargeClusterRec::exportEventToTxt(){    // only for debugging!
    std::ofstream myfile;
    std::stringstream filename;
    std::string rootname = m_userOutfile.substr(0, m_userOutfile.size()-5);
    filename << rootname << "_event.txt";
    myfile.open(filename.str().c_str());
    for(unsigned int pid = 0; pid < m_totPmtNum; ++pid){
        if(m_pmtTable[pid].used){
            myfile  << pid << " "
                << m_pmtTable[pid].pos.X() << " "
                << m_pmtTable[pid].pos.Y() << " "
                << m_pmtTable[pid].pos.Z() << " "
                << m_pmtTable[pid].q << " "
                << m_pmtTable[pid].fht << std::endl;
        }
        else{
            myfile  << pid << " "
                << m_pmtTable[pid].pos.X() << " "
                << m_pmtTable[pid].pos.Y() << " "
                << m_pmtTable[pid].pos.Z() << " "
                << 0 << " "
                << -1 << std::endl;
        }
    }
    myfile.close();
}

bool WPChargeClusterRec::getMCTruth(){  // only for debugging!
    // Get Service
    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" 
            << std::endl;
        return false;
    }
    auto m_buf = navBuf.data();
    // Get Truth
    auto nav = m_buf->curEvt();
    auto simheader = JM::getHeaderObject<JM::SimHeader>(nav);
    auto se = (JM::SimEvt*)simheader->event();
    auto simtrk = (JM::SimTrack*) se->findTrackByTrkID(0); 
    mc_R0.SetXYZ(simtrk->getInitX(), simtrk->getInitY(), simtrk->getInitZ());
    mc_dir.SetXYZ(simtrk->getInitPx(), simtrk->getInitPy(), simtrk->getInitPz());
    mc_dir = mc_dir.Unit();
    return true;
}

TVector3 WPChargeClusterRec::getIntersectionPoint(int option){  // only for debugging!
    // setup
    double Rc = 21655;  //Cylinder mantle
    double Rs = 20752;  //half sphere
    TVector3 output;
    // special case: parallel to z axis
    double Rtest2 = mc_R0.X()*mc_R0.X()+mc_R0.Y()*mc_R0.Y();
    if(mc_dir.Z() == -1 && mc_R0.Z() > 0 && Rtest2 < Rs*Rs){
        if(option == 0){
            output.SetXYZ(mc_R0.X(), mc_R0.Y(), sqrt(Rs*Rs-Rtest2));
            return output;
        }
        else if(option==1){
            output.SetXYZ(mc_R0.X(), mc_R0.Y(), -21655.);
            return output;
        }
        else{
            output.SetXYZ(0,0,1e7);
            std::cout << "Wrong Option! Returning (0,0,1e7)" << std::endl;
            return output;
        }
    }
    if(mc_dir.Z() == 1 && mc_R0.Z() < -21655. && Rtest2 < Rs*Rs){
        if(option == 0){
            output.SetXYZ(mc_R0.X(), mc_R0.Y(), -21655.);
            return output;
        }
        else if(option==1){
            output.SetXYZ(mc_R0.X(), mc_R0.Y(), sqrt(Rs*Rs-Rtest2));
            return output;
        }
        else{
            output.SetXYZ(0,0,1e7);
            std::cout << "Wrong Option! Returning (0,0,1e7)" << std::endl;
            return output;
        }
    }
    // Cross outer cylinder in 2D
    // ### Get projection vectors
    TVector3 proj_vtx = mc_R0;
    TVector3 proj_dir = mc_dir;
    proj_vtx.SetZ(0);
    proj_dir.SetZ(0);
    // ### solve quadratic equation
    double a = proj_vtx*proj_dir;
    double b = proj_vtx.Mag2();
    double c = proj_dir.Mag2();
    double t1;
    if(option==0) t1 = (-1.*a - sqrt(a*a-c*(b-Rc*Rc)))/c;  // smaller value = entry point
    else if(option==1) t1 = (-1.*a + sqrt(a*a-c*(b-Rc*Rc)))/c;  // larger value = exit point
    else{
        output.SetXYZ(0,0,1e7);
        std::cout << "Wrong Option! Returning (0,0,1e7)" << std::endl;
        return output;
    }
    if(t1 != t1 ){
        output.SetXYZ(0,0,1e7);
        std::cout << "No crossection found! Returning (0,0,1e7)" << std::endl;
        return output;
    }
    // Check Z value
    double z = (mc_R0+ t1*mc_dir).Z();
    if(z > 0){
        // it's half sphere
        // solve quad equation for crosssection
        double a3 = mc_R0*mc_dir;
        double b3 = mc_R0.Mag2();
        double c3 = mc_dir.Mag2();
        double l1 = (-1.*a3 - sqrt(a3*a3-c3*(b3-Rs*Rs)))/c3;
        double l2 = (-1.*a3 + sqrt(a3*a3-c3*(b3-Rs*Rs)))/c3;
        if(l1 != l1 || l2 != l2){
            output.SetXYZ(0,0,1e7);
            std::cout << "No crossection with sphere found! negative sqrt!  Returning (0,0,1e7)" << std::endl;
            return output;
        }
        double z1 = (mc_R0+ l1*mc_dir).Z();
        double z2 = (mc_R0+ l2*mc_dir).Z();
        if(z1 < 0 && z2 < 0){
            output.SetXYZ(0,0,1e7);
            std::cout << "No crossection with sphere found! both points < 0! Returning (0,0,1e7)" << std::endl;
            return output;
        }
        if(option == 0 && z1 > 0){
            // entry -> take first
            std::cout << "--> Crossection with sphere found! Entry, z1>0" << std::endl;
            output = mc_R0 + l1*mc_dir;
            return output;
        }
        else if(option == 1 && z2 > 0){
            // exit-> take second
            std::cout << "--> Crossection with sphere found! Exit, z2>0" << std::endl;
            output = mc_R0 + l2*mc_dir;
            return output;
        }
        else{
            output.SetXYZ(0,0,1e7);
            std::cout << "No crossection with sphere found! Returning (0,0,1e7)" << std::endl;
            std::cout << "Values: " << l1 << ", " << l2 << ", " << z1 << ", " << z2 << ", " << option << ", " << z << std::endl;
            mc_R0.Print();
            mc_dir.Print();
            return output;
        }
    }
    else if(z > -21650){
        // it's mantle
        std::cout << "Mantle crossection found" << std::endl;
        output = mc_R0+ t1*mc_dir;
        return output;
    }
    else{
        // it's pool floor
        double zfloor = -21655.;
        double lfloor = (zfloor - mc_R0.Z())/mc_dir.Z();
        TVector3 fp = mc_R0+ lfloor*mc_dir;
        if(fp.X()*fp.X()+fp.Y()*fp.Y() > Rc*Rc){
            output.SetXYZ(0,0,1e7);
            std::cout << "No crossection with detector found! Returning (0,0,1e7)" << std::endl;
            return output;
        }
        else{
            std::cout << "--> Crossection with floor found!" << std::endl;
            return fp;
        }
    }
}

double WPChargeClusterRec::getCharge(int option){   // only for debugging!
    TVector3 ip = getIntersectionPoint(option);

    std::vector< std::pair<double,int> > distances;
    for(int nId=0; nId < m_totPmtNum; nId++){
        if(m_pmtTable[nId].used) distances.push_back( std::make_pair( (m_pmtTable[nId].pos - ip).Mag(), nId) );
    }
    std::sort(distances.begin(), distances.end());

    return m_pmtTable[distances.at(0).second].q;
}

int * WPChargeClusterRec::get2HighCharges(){    // only for debugging!
    int *charge=new int[2];
    int *idd=new int[2];
    charge[0] = 0; charge[1] = 0;
    idd[0] = 0; idd[1] = 0;
    for(int pmtId=0; pmtId < m_totPmtNum; pmtId++){
        if(m_pmtTable[pmtId].used){
            int nPE = m_pmtTable[pmtId].q;
            if(nPE > charge[0]){
                charge[1] = charge[0];
                idd[1] = idd[0];
                charge[0] = nPE;
                idd[0] = pmtId;
            }
            else if(nPE > charge[1]){
                charge[1] = nPE;
                idd[1] = pmtId;
            }
            else continue;
        }
    }
    return idd;
}

double WPChargeClusterRec::getDist(int pmtId){  // only for debugging!
    getMCTruth();
    TVector3 pmtCenter = m_pmtTable[pmtId].pos;
    return (mc_dir.Dot((pmtCenter-mc_R0))*mc_dir+mc_R0-pmtCenter).Mag();
}

int **WPChargeClusterRec::findClustersSimple(int *nClusters){   // Logic to define clusters
    // Get break condition
    double totalCharge = 0;
    double maxCharge = 0;
    int usedPmt = 0;
    for(unsigned int pid = 0; pid < m_totPmtNum; ++pid){
        if(m_pmtTable[pid].used){
            double charge = m_pmtTable[pid].q;
            if(charge>maxCharge) maxCharge = charge;
            totalCharge += charge;
            usedPmt++;
        }
    }
    double meanCharge = totalCharge/usedPmt;
    LogDebug << "The mean charge is " << meanCharge << " PE in " << usedPmt << " PMTs" << std::endl;

    // Get Clusters
    int maxCl = 40;         // object to change
    ClusterFinder* cf = new ClusterFinder(m_pmtTable);
    int **tempclusters=new int*[maxCl];
    int tempCounter=0;
    bool getMoreCluster = true;
    while(getMoreCluster){
        tempclusters[tempCounter] = cf->getClusterSimple();
        double chargeSum = 0;
        double maxCinC = m_pmtTable[tempclusters[tempCounter][1]].q;
        for(int j=1;j<=tempclusters[tempCounter][0];j++){
            double charge = m_pmtTable[tempclusters[tempCounter][j]].q;
            chargeSum+= charge;
        }
        double mcc = chargeSum/tempclusters[tempCounter][0];
        double mccThreshold = (m_minCiC + 8*meanCharge)/9.;
        if(maxCinC > m_minCiC && mcc > mccThreshold && tempCounter < maxCl-1){
            LogDebug << "Mean Charge Threshold is" << mccThreshold << ", and the maximum charge in the Cluster is " << maxCinC << " which is larger than " << m_minCiC << std::endl;
            LogDebug << "Mean charge of Cluster #" << tempCounter << " is " << chargeSum << "/" << tempclusters[tempCounter][0]<< " = " << mcc << " PE" << std::endl;
            tempCounter++;
        }
        else{
            getMoreCluster = false;
        }
    }
    LogDebug << "ClusterFinder found " << tempCounter << " Clusters" << std::endl;

    int **outclusters=new int*[tempCounter];
    int counter = 0;
    for(int i=0;i<tempCounter;i++){
        outclusters[counter] = tempclusters[i];
        counter++;
    }
    (*nClusters) = counter; 
    return outclusters;
}


