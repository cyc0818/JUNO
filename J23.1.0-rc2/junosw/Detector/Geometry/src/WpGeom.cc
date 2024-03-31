//
//  Author: Kaijie Li  2016-09-24
//

#include "Geometry/WpGeom.h"
#include "Geometry/GeoUtil.h"
#include "Identifier/WpID.h"
#include "TSystem.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoPhysicalNode.h"
#include "TGeoTube.h"
#include "TString.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TMath.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <cassert>

WpGeom::WpGeom()
    : m_geom(0)
    , m_fastInit(false)
    , m_layerNum(0)
    , m_verb(0)
{
}

WpGeom::~WpGeom()
{
}

bool WpGeom::init()
{
    initRootGeo();

    return true;
}

bool WpGeom::initRootGeo()
{
    readRootGeoFile();
    setPhyNodes();
    orgnizePmt();
    printPmt();
    initWpInfo();

    return true;
}

bool WpGeom::readRootGeoFile()
{
    TGeoManager::SetVerboseLevel(0);  // silence

    m_useDefaultGeom = true;
    if ( m_geomFileName != std::string("default") ) m_useDefaultGeom = false;

    if ( m_useDefaultGeom ) {
        std::string geometryPath = getenv("JUNO_GEOMETRY_PATH");
        if ( getVerb() >= 1) std::cout << "geometryPath" << geometryPath << std::endl;
        std::string geoDataPath  = geometryPath + "/share/";
        std::string geoFileName  = "JunoGeom";
        std::string geoFileType  = ".root";
        TString geoFile = TString(geoDataPath) + geoFileName + geoFileType;
        if ( getVerb() >= 1) std::cout << "geoFile " << geoFile << std::endl; 

        bool geoRootFileExist = false;
        if (!gSystem->AccessPathName( geoFile )) {
            // exist, do nothing check if file in local directory exists
            geoRootFileExist = true;
        }
        else {
            geoFileType = ".gdml";
        }

        if ( getVerb() >= 1) {
            std::cout << "WpGeom::readRootGeoFile() Path: " << TString(geoDataPath) + geoFileName << std::endl;
            std::cout << "WpGeom::readRootGeoFile() verbose " << m_geom->GetVerboseLevel() << std::endl;
        }
        if (!geoRootFileExist) {
            std::cout << "If you see \"Error: Unsupported GDML Tag Used : xxx\", it is OK." << std::endl
                << "Because Detector/Geometry/share/" << geoFileName << ".root is missing," << std::endl
                << "you are reading from " << geoFileName << ".gdml instead, please wait." << std::endl
                << geoFileName << ".root is being written out for faster reading without Error msg." << std::endl
                << "Move it from current directory to Detector/Geometry/share/"
                << std::endl;
        }
        m_geom = TGeoManager::Import( geoFile, TString(m_geomPathName) );
        if (!geoRootFileExist) {
            TString geoOutput = TString("./") + geoFileName + ".root";
            std::cout << "Write out geometry file " << geoOutput << std::endl;
            m_geom->Export( geoOutput, geoFileName.c_str() );
        }
    }
    else {
        if (gGeoManager) m_geom = gGeoManager;
        else {
            m_geom = TGeoManager::Import( TString(m_geomFileName),
                                          TString(m_geomPathName));
        }
    }

    std::cout << "m_geom: " << m_geom << std::endl;
    assert(m_geom);

    return true;
}

bool WpGeom::setPhyNodes()
{
    bool status = false;
    // if ( m_useDefaultGeom ) {
    //     status = setPhyNodesManually();
    // }
    // else {
    //     status = setPhyNodesAuto();
    // }
    status = setPhyNodesAuto();

    return status;
}

bool WpGeom::setPhyNodesManually()
{
    // need to set Pmt Max
    WpID::setModuleMax(18305);  

    TGeoPhysicalNode *phyNode = 0;
    int copyNoBegin = 9; // Det0: 5, Det1: 6, Det1&18306 Pmt: 9
    for (int iPmt = WpID::moduleMin(); iPmt <= WpID::moduleMax(); iPmt++) {
        Identifier pmtID = WpID::id(iPmt, 0);
        //cout << iPmt << endl;

        // The following for Det0
        //TString phyNodeName("/lWorld_1/lSteelBall_14165/lTarget_14164/PMT_20inch_water_"); //5/Pmt_20inch_water_body_log_4/Pmt_20inch_water_inner1_log_2");
        //phyNodeName += (copyNoBegin+iPmt);
        //phyNodeName += "/PMT_20inch_water_body_log_4";
        //phyNodeName += "/PMT_20inch_water_inner1_log_2";

        // The following for Det1
        //TString phyNodeName("/lWorld_1/lSteelBall_16727/Waterlogic_16726/PMT_20inch_log_"); //6/PMT_20inch_body_log_4/Pmt_20inch_water_inner1_log_2");
        //phyNodeName += (copyNoBegin+iPmt);
        //phyNodeName += "/PMT_20inch_body_log_4";
        //phyNodeName += "/PMT_20inch_inner1_log_2";

        // The following for Det1 with 18306 PMT
        TString phyNodeName("/lWorld_1/lBtmRock_57134/"); //9/Pmt_20inch_log_7/PMT_20inch_body_log_4/Pmt_20inch_water_inner1_log_2");
        phyNodeName += (copyNoBegin+iPmt);
        phyNodeName += "/PMT_20inch";
        phyNodeName += "/PMT_20inch_body";
        phyNodeName += "/PMT_20inch_inner1";

        //cout << phyNodeName << endl;
        phyNode = m_geom->MakePhysicalNode(phyNodeName);
        addPmt(pmtID, phyNode, 1);
    }
    if ( getVerb() >= 1) std::cout << "WpDetector Pmt size " << m_mapIdToPmt.size() << std::endl;

    return true;
}

int WpGeom::getPmtType(TString name)
{
    if ( name.Contains("20inch") || name.Contains("Mask") ) return 1;
    else if ( name.Contains("3inch") ) return 2;

    return 0;
}

bool WpGeom::setPhyNodesAuto()
{
    analyzeGeomStructure();

    TGeoPhysicalNode *phyNode = 0;
    int nNodesPmtLevel = m_nodePmtMother->GetNdaughters();
    int iPmt = 0;
    for (int iNode = 0; iNode < nNodesPmtLevel; iNode++) {
        TGeoNode *nodePmt = m_nodePmtMother->GetDaughter(iNode);
        TString phyNodeName = m_pathMother + "/" + nodePmt->GetName();
        int pmtType = getPmtType(nodePmt->GetName());

        if ( pmtType == 1 ) {
            phyNodeName += m_pathBottom20inch;
        }
        else if ( pmtType == 2 ) {
            phyNodeName += m_pathBottom3inch;
        }   

        if ( pmtType == 1 || pmtType == 2 ) {
            Identifier pmtID = WpID::id(iPmt, 0);

            PmtGeom* pmt = 0;
            if (m_fastInit) { // fast init PmtGeom with center and axisDir
                pmt = addPmt(pmtID, 0, pmtType);

                double local[3]  = {0.0, 0.0, 0.0};
                double master[3] = {0.0, 0.0, 0.0};
                nodePmt->LocalToMaster(local, master);
                TVector3 center(master[0], master[1], master[2]);
                center *= GeoUtil::cm2mm();
                if (getVerb() >= 2) std::cout << "node " << iNode << " center (" << master[0] << ", " << master[1] << ", " << master[2] << ")" << std::endl;
                local[2] = 1.0;
                nodePmt->LocalToMaster(local, master);
                TVector3 dirEnd(master[0], master[1], master[2]);
                dirEnd *= GeoUtil::cm2mm();

                pmt->setCenterFast( center );
                pmt->setAxisDirFast( dirEnd-center );
            }
            else {  // full init PmtGeom with a TGeoPhysicalNode
                phyNode = m_geom->MakePhysicalNode(phyNodeName);
                pmt = addPmt(pmtID, phyNode, pmtType);
            }

            if ( (iNode < 10 || nNodesPmtLevel-iNode  <= 970) && getVerb() >= 2 ) {
                if (m_fastInit) {
                    std::cout << "pmt " << iPmt << " center "; pmt->getCenter().Print();
                }
                else {    
                    std::cout << phyNode->GetName() << std::endl;
                    phyNode->Print();
                }
            }
            iPmt++;
        }
    }
    if ( getVerb() >= 1) std::cout << "Auto WpDetector Pmt size " << m_mapIdToPmt.size() << std::endl;

    return true;
}

void WpGeom::analyzeGeomStructure()
{
    m_isPmtMotherFound = false;
    m_nodeMotherVec.clear();
    searchPmtMother( m_geom->GetTopNode() );
    if ( getVerb() >= 1) std::cout << "pmtMother " << m_nodePmtMother->GetName() << " nChild " << m_nodePmtMother->GetNdaughters() << std::endl;
    if ( getVerb() >= 1) std::cout << "pathMother " << m_pathMother << std::endl;
    
    searchPmt();
    if ( getVerb() >= 1) std::cout << "nPmt " << findPmtNum() << std::endl;
    if ( getVerb() >= 1) {
        std::cout << "Before set Module Range" << std::endl
            << "Module Range [" << WpID::moduleMin() << ", " << WpID::moduleMax() << "]" << std::endl
            << "Module20inch Range [" << WpID::module20inchMin() << ", " << WpID::module20inchMax() << "]" << std::endl;
    }
    WpID::setModule20inchMin( 0 );
    WpID::setModule20inchMax( findPmt20inchNum()-1 );
    WpID::setModuleMax( findPmtNum()-1 );

    if ( getVerb() >= 1) {
        std::cout << "After set Module Range" << std::endl
            << "Module Range [" << WpID::moduleMin() << ", " << WpID::moduleMax() << "]" << std::endl
            << "Module20inch Range [" << WpID::module20inchMin() << ", " << WpID::module20inchMax() << "]" << std::endl;
    }

    m_isPmtBottomFound = false;
    m_nodeBottomVec.clear();
    if ( findPmt20inchNum() > 0 ) {
        searchPmtBottom( m_nodePmt20inch );
        m_pathBottom20inch = setPathBottom();
        if ( getVerb() >= 1) std::cout << "pathBottom20inch " << m_pathBottom20inch << std::endl;
    }

    m_isPmtBottomFound = false;
    m_nodeBottomVec.clear();
}

void WpGeom::searchPmt()
{
    m_nPmt = 0;
    m_nPmt20inch = 0;
    assert(m_nodePmtMother);
    int nNodesPmtLevel = m_nodePmtMother->GetNdaughters();

//    m_volPmt20inch = m_nodePmtMother->GetDaughter(nNodesPmtLevel/2)->GetVolume();
//    TString volPmtName = m_volPmt20inch->GetName();
//    if ( getVerb() >= 1) std::cout << "PmtName " << volPmtName << std::endl;

    m_nodePmt20inch = 0;
    for ( int iNode = 0; iNode < nNodesPmtLevel; iNode++ ) {
        TString volPmtName = m_nodePmtMother->GetDaughter(iNode)->GetVolume()->GetName();
        if ( getVerb() >= 2) std::cout << iNode << " " << volPmtName << std::endl;
  
        if ( volPmtName.Contains("PMT_20inch") || volPmtName.Contains("Mask") ) { 
            m_nodePmt20inch = m_nodePmtMother->GetDaughter(iNode);
            m_nPmt20inch++;
        }
    }
    m_nPmt = m_nPmt20inch;
    std::cout << "nPmt " << m_nPmt << " nPmt20inch " << m_nPmt20inch << std::endl;
}

void WpGeom::searchPmtMother(TGeoNode* node)
{   
    if ( !m_isPmtMotherFound ) {
      m_nodeMotherVec.push_back(node);
    }

    assert(node);
    int nChild = node->GetNdaughters();
    if ( nChild > 1000 ) {  // assume the total PMT in CD is greater than 10000, so this level; this is WP;
        m_nodePmtMother = node;
        m_isPmtMotherFound = true;
    }

    for (int iChild = 0; iChild < nChild && (!m_isPmtMotherFound); iChild++) {
        TGeoNode* childNode = node->GetDaughter(iChild);
        searchPmtMother(childNode);
        if ( !m_isPmtMotherFound ) {
            m_nodeMotherVec.pop_back();
        }
    } 

    setPathMother();
}   

void WpGeom::searchPmtBottom(TGeoNode* node)
{   
    assert(node);

    if ( getVerb() >= 1) std::cout << __func__ << " node " << node->GetName() << std::endl;
    if ( !m_isPmtBottomFound ) {
        m_nodeBottomVec.push_back(node);
        if ( getVerb() >= 2) std::cout << "nodeSize " << m_nodeBottomVec.size() << std::endl;
    }

    if ( TString(node->GetVolume()->GetName()).Contains("inch_inner2") ) {  // PMT_20inch_inner2
        m_isPmtBottomFound = true;
    }

    int nChild = node->GetNdaughters();
    for ( int iChild = 0; iChild < nChild && (!m_isPmtBottomFound); iChild++ ) {
        TGeoNode* childNode = node->GetDaughter(iChild);
        searchPmtBottom(childNode);
        if ( !m_isPmtBottomFound ) {
            m_nodeBottomVec.pop_back();
        }
    } 
}


void WpGeom::setPathMother()
{
    m_pathMother = TString("");
    if ( getVerb() >= 3) std::cout << m_nodeMotherVec.size() << std::endl;

    for (int iNode = 0; iNode < (int)m_nodeMotherVec.size(); iNode++) {
        m_pathMother += "/";
        m_pathMother += m_nodeMotherVec[iNode]->GetName();
        if ( getVerb() >= 3) std::cout << m_nodeMotherVec[iNode]->GetName() << std::endl;
    }
}

TString WpGeom::setPathBottom()
{
    TString pathBottom = TString("");
    if ( getVerb() >= 2) std::cout << m_nodeBottomVec.size() << std::endl;

    for (int iNode = 1; iNode < (int)m_nodeBottomVec.size(); iNode++) {
        pathBottom += "/";
        pathBottom += m_nodeBottomVec[iNode]->GetName();
        if ( getVerb() >= 2) std::cout << m_nodeBottomVec[iNode]->GetName() << std::endl;
    }

    return pathBottom;
}

PmtGeom* WpGeom::addPmt(Identifier id, TGeoPhysicalNode *phyNode, int pmtType)
{
    std::map<Identifier, PmtGeom*>::iterator it = m_mapIdToPmt.find(id);
    if ( it == m_mapIdToPmt.end() ) {
        PmtGeom* pmt = new PmtGeom(id);
        pmt->setPhyNode(phyNode);

        if (pmtType == 1) pmt->set20inch(true); 
        else if (pmtType == 2) pmt->set20inch(false);

        m_mapIdToPmt[id] = pmt;

        return pmt;
    }
    else {
        return it->second;
    }
}

PmtGeom* WpGeom::getPmt(Identifier id)
{
    std::map<Identifier, PmtGeom*>::iterator it = m_mapIdToPmt.find(id);
    if ( it == m_mapIdToPmt.end() ) {
        std::cerr << "id " << id << "(" << WpID::module(id) << ", " << WpID::pmt(id)
            << ")'s PmtGeom does not exist " << std::endl;
        return 0;
    }
    return m_mapIdToPmt[id];
}

PmtGeom* WpGeom::getPmt(int layer, int azimuth, int pmt)
{
    int module = 0;
    for (int i = 0; i < layer; i++) {
        module += getAzimuthNum(i);
    }
    module += azimuth;

    if ( getVerb() >= 2) std::cout << "layer " << layer << " azimuth " << azimuth << " module " << module << std::endl;
    Identifier pmtID = WpID::id(module, pmt);
  
    return getPmt(pmtID);
}

void WpGeom::printPmt()
{
    std::cout << "waterpool " << __func__ << " begin " << getVerb() << std::endl;

    if ( getVerb() >= 2) {

        std::cout << "Print first 15 Pmt... " << std::endl;
        for (PmtMapIt it = m_mapIdToPmt.begin(); it != m_mapIdToPmt.end(); it++) {
            Identifier pmtID = it->first;
            PmtGeom *pmt = it->second;
            //std::cout << WpID::module(pmtID) << std::endl;
            if (WpID::module(pmtID) < 15) {
                pmt->print();
            }
	    if (WpID::module(pmtID) > 2300) {
                pmt->print();
            }

        }
    }

    if ( getVerb() >= 2) {

        std::cout << "Print last Pmt on each layer..." << std::endl;
        for (int layer = 0; layer < getLayerNum(); layer++) {
            getPmt(layer, getAzimuthNum(layer)-1, 0)->print();
        }

        int KS = 6;
        for (int layer = 0; layer < getLayerNum(); layer++) {
            std::cout << "Layer " << std::setw(KS) << layer 
                << " AzimuthNum " << std::setw(KS) << getAzimuthNum(layer)
                << std::endl;        
        }
        std::cout << "LayerNum " << getLayerNum() << std::endl;
    }
    std::cout << "waterpool " << __func__ << " end " << getVerb() << std::endl;
}

PmtGeom* WpGeom::findCrossedPmt(const TVector3 vtx, const TVector3 dir)
{
    for (PmtMapIt it = m_mapIdToPmt.begin(); it != m_mapIdToPmt.end(); it++) {
        Identifier pmtID = it->first;
        PmtGeom *pmt = it->second;

        bool isCrossed = pmt->isCrossed(vtx, dir);
        if (isCrossed) {
            return pmt;
        }
    }

    return 0;
}

PmtGeom* WpGeom::findCrossedPmtFast(const TVector3 vtx, const TVector3 dir)
{
    return findCrossedPmt(vtx, dir);
/*
    //FIXME: Faster algorithm to be developed by esitmation and compare neighboring Pmt distance

    // get first Pmt id, use it to calculate sphere radius
    Identifier id0 = WpID::id(0, 0);
    double sphereR = getPmt(id0)->getCenter().Mag();

    // get intersection with sphere
    TVector3 intersection = GeoUtil::getSphereIntersection(vtx, dir, sphereR);
    double cosTheta = intersection.z()/intersection.Mag();

    // estimate start and end Pmt 
    int startPmt = int((0.5*(1-cosTheta)-0.03)*getPmtNum());
    int endPmt   = int((0.5*(1-cosTheta)+0.03)*getPmtNum());
    if (cosTheta >=  0.9) startPmt = 0;
    if (cosTheta <= -0.9) endPmt   = getPmtNum();
    int KS = 6;
    if ( getVerb() >= 3) std::cout << "cos " << std::setw(2*KS) << cosTheta << " start " << std::setw(KS) << startPmt << " end " << std::setw(KS) << endPmt << " ";

    for (int iPmt = startPmt; iPmt < endPmt; iPmt++) {
        Identifier PmtId = WpID::id(iPmt, 0);
        PmtGeom *pmt = getPmt(PmtId);

        bool isCrossed = pmt->isCrossed(vtx, dir);
        if (isCrossed) {
            return pmt;
        }
    }

    return 0;*/
}

bool WpGeom::orgnizePmt()
{
    int layer = -1;
    int azimuth = 0;
    std::vector<int> azimuthOnLayer;
    TVector3 center;
    TVector3 center_old(-9, -9, -9);

    for (PmtMapIt it = m_mapIdToPmt.begin(); it != m_mapIdToPmt.end(); it++) {
        Identifier pmtID = it->first;
        PmtGeom *pmt = it->second;
        if ( getVerb() >= 3) std::cout << WpID::module(pmtID) << std::endl;
 
        center = pmt->getCenter();
        if ( fabs(center.z() - center_old.z()) > 3 ) { // new layer
            if (layer >= 0) {
                if ( getVerb() >= 3) std::cout << "layer " << layer << " azimuth " << azimuth << std::endl;
                azimuthOnLayer.push_back(azimuth+1);
            }
            layer++;
            azimuth = 0;
        }
        else { // same layer
            azimuth++;
        }

        pmt->setLayer(layer);
        pmt->setAzimuth(azimuth);
        pmt->setPmt(0);
        center_old = center;
    }
    if ( getVerb() >= 2) std::cout << "layer " << layer << " azimuth " << azimuth << std::endl;
    azimuthOnLayer.push_back(azimuth+1);

    for (std::vector<int>::iterator it = azimuthOnLayer.begin(); it != azimuthOnLayer.end(); it++) {
        m_azimuthNum.push_back(*it);
    }
    m_layerNum = m_azimuthNum.size();

    return true;
}

int WpGeom::getAzimuthNum(int layer)
{
    if ( layer >= 0 && layer < (int)m_azimuthNum.size() ) {
        return m_azimuthNum[layer];
    }
    else {
        std::cerr << "WpGeom::getAzimuthNum at layer " << layer << " is wrong" << std::endl;
        return -9;
    }
}

void WpGeom::initWpInfo()
{
    //std::cout << __func__ << " begins" << std::endl;

    //Wp
    TGeoNode *nodeWp = m_nodePmtMother;  
    TString phyNodeName = m_pathMother;
    TString volWpName = nodeWp->GetVolume()->GetName();
    pWp = (TGeoTube *)(nodeWp->GetVolume()->GetShape());
    //pWp->InspectShape();

    if ( getVerb() >= 2){
    std::cout << __func__ << " volWpName " << volWpName << std::endl;
    std::cout << __func__ << " phyNodeName " << phyNodeName << std::endl;
    std::cout << __func__ << " volWpMaterial " << nodeWp->GetVolume()->GetMaterial()->GetName() << std::endl;
    }

    //std::cout << __func__ << " end" << std::endl;
}

void WpGeom::printWp()
{
    std::cout << "WaterPool " << __func__ << " begin" << std::endl;

    std::cout << "WaterPool Rmax = " << getWpRmax() << " , Dz = " << getWpDz() << std::endl;

    std::cout << "WaterPool " << __func__ << " end" << std::endl;
}
