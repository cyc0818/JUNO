//
//  Author: Zhengyun You  2013-11-20
//

#include "Geometry/CdGeom.h"
#include "Geometry/GeoUtil.h"
#include "Identifier/CdID.h"
#include "TSystem.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoPhysicalNode.h"
#include "TString.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TMath.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <cassert>
#include <vector>
#include "TGeoSphere.h"
#include "TGeoCompositeShape.h"
#include "TGeoBoolNode.h"
#include "TGeoTube.h"


// for output root files
#include "TFile.h"
//#include "TTree.h"

CdGeom::CdGeom()
    : m_geom(0)
    , m_fastInit(true)
    , m_layerNum(0)
    , m_verb(0)
	, pAcrylicBall(0)
	, pAcrylicChimney(0)
	, pLsBall(0)
	, pLsChimney(0)
	, m_AcrylicIsComposite(false)
	, m_LsIsComposite(false)
{
}

CdGeom::~CdGeom()
{
}

bool CdGeom::init()
{
    initRootGeo();
    
    return true;
}

bool CdGeom::initRootGeo()
{   
    //setVerb(1);
    readRootGeoFile();
    setPhyNodes();
    orgnizePmt();
    printPmt();
    initCdInfo(); 
    //testfunction();

    return true;
}

bool CdGeom::readRootGeoFile()
{
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    TGeoManager::SetVerboseLevel(1);  // silence

    // whether to use the default
    m_useDefaultGeom = true;
    if ( m_geomFileName != std::string("default") ) m_useDefaultGeom = false;

    if ( m_useDefaultGeom ) {
        if( getVerb() >=1) std::cout << " It use default geoFile "<<std::endl;

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
            geoFile = TString(geoDataPath) + geoFileName + geoFileType;
        }

        if ( getVerb() >= 1) {
            std::cout << "CdGeom::readRootGeoFile() Path: " << TString(geoDataPath) + geoFileName << std::endl;
            std::cout << "CdGeom::readRootGeoFile() verbose " << m_geom->GetVerboseLevel() << std::endl;
        }
        if (!geoRootFileExist) {
            std::cout << "If you see \"Error: Unsupported GDML Tag Used : xxx\", it is OK." << std::endl
                << "Because Detector/Geometry/share/" << geoFileName << ".root is missing," << std::endl
                << "you are reading from " << geoFileName << ".gdml instead, please wait." << std::endl
                << geoFileName << ".root is being written out for faster reading without Error msg." << std::endl
                << "Move it from current directory to Detector/Geometry/share/"
                << std::endl;
        }

        // // set the default Units to cm
        // TGeoManager::SetDefaultUnits(TGeoManager::EDefaultUnits::kRootUnits);

        m_geom = TGeoManager::Import( geoFile, TString(m_geomPathName) );
        if (!geoRootFileExist) {
            TString geoOutput = TString("./") + geoFileName + ".root";
            std::cout << "Write out geometry file " << geoOutput << std::endl;
            // m_geom->Export( geoOutput, geoFileName.c_str() );
            TFile* f = TFile::Open(geoOutput, "RECREATE");
            m_geom->SetName("JunoGeom");
            m_geom->Write();
            f->Write();
            f->Close();
        }
    }
    // not default geomFile
    else {
        if( getVerb() >=1) std::cout <<__FILE__<<__func__<< " didn't use default geoFile "<<std::endl;
        // Fixme: where did we define gGeomanager
        if (gGeoManager) m_geom = gGeoManager;
        else {
            m_geom = TGeoManager::Import( TString(m_geomFileName), 
                                          TString(m_geomPathName));
        }
    }
    
    if( getVerb() >=1){
        std::cout << "m_geom: " << m_geom << std::endl;
        std::cout << "geometryPathName : " << m_geomPathName << std::endl;
        std::cout << "geoFileName : " << m_geomFileName << std::endl; 
        std::cout << "CdGeom::readRootGeoFile() TGeoManager verbose " << m_geom->GetVerboseLevel() << std::endl;
    }

    assert(m_geom);
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " end " << std::endl;

    return true;
}

bool CdGeom::setPhyNodes()
{
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    bool status = false;
    status = setPhyNodesAuto();

    return status;
}

bool CdGeom::setPhyNodesManually()
{
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    // need to set Pmt Max
    CdID::setModuleMax(18305);

    TGeoPhysicalNode *phyNode = 0;
    int copyNoBegin = 9; // Det0: 5, Det1: 6, Det1&18306 Pmt: 9
    for (int iPmt = CdID::moduleMin(); iPmt <= CdID::moduleMax(); iPmt++) {
        Identifier pmtID = CdID::id(iPmt, 0);
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
        TString phyNodeName("/lWorld_1/lSteelBall_18316/Waterlogic_18315/_Cover_OUT_"); //9/Pmt_20inch_log_7/PMT_20inch_body_log_4/Pmt_20inch_water_inner1_log_2");
        phyNodeName += (copyNoBegin+iPmt);
        phyNodeName += "/PMT_20inch_log_7";
        phyNodeName += "/PMT_20inch_body_log_4";
        phyNodeName += "/PMT_20inch_inner1_log_2";

        //cout << phyNodeName << endl;
        phyNode = m_geom->MakePhysicalNode(phyNodeName);
        addPmt(pmtID, phyNode, 1);
    }
    if ( getVerb() >= 1) std::cout << "CdDetector Pmt size " << m_mapIdToPmt.size() << std::endl;
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " end " << std::endl;

    return true;
}

int CdGeom::getPmtType(TString name)
{   
    //if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    if ( name.Contains("20inch") || name.Contains("Mask") || name.Contains("LPMT") ) return 1;
    else if ( name.Contains("3inch") ) return 2;
    
    return 0;
}

bool CdGeom::setPhyNodesAuto()
{   
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    
    analyzeGeomStructure();

    TGeoPhysicalNode *phyNode = 0;
    int nNodesPmtLevel = m_nodePmtMother->GetNdaughters();
    int iPmt = 0;

    m_Pmt_r_20inch_Hama = -1;
    m_Pmt_r_20inch_NNVT = -1;
    m_Pmt_r_3inch = -1;

    for (int iNode = 0; iNode < nNodesPmtLevel; iNode++) 
    {
        TGeoNode *nodePmt = m_nodePmtMother->GetDaughter(iNode);
        TString phyNodeName = m_pathMother + "/" + nodePmt->GetName();

        int pmtType = getPmtType(nodePmt->GetName());

        if ( pmtType == 1 ) 
        {
            // need to separate NNVT and Hamamatsu PMTs
            if (phyNodeName.Contains("NNVT")) 
            {
                phyNodeName += m_pathBottom20inchNNVT;
            } else if (phyNodeName.Contains("Hama"))
            {
                phyNodeName += m_pathBottom20inchHama;
            } else 
            {
                phyNodeName += m_pathBottom20inch;
            }
        }
        else if ( pmtType == 2 ) 
        {
            phyNodeName += m_pathBottom3inch;
        }   

        //std::cout << "wuchx debug here name" << phyNodeName << std::endl;

        if ( pmtType == 1 || pmtType == 2 ) 
        {
            Identifier pmtID = CdID::id(iPmt, 0);

            PmtGeom* pmt = 0;

            //PMT Boundary Box for PMT radius
            if(m_Pmt_r_20inch_NNVT <0&&phyNodeName.Contains("NNVT"))
            {
                Double_t *shapebox;
                shapebox = nodePmt->GetVolume()->GetVoxels()->GetBoxes();
                m_Pmt_r_20inch_NNVT = shapebox[0]*GeoUtil::cm2mm();
            }

            if(m_Pmt_r_20inch_Hama <0&&phyNodeName.Contains("Hama"))
            {
                Double_t *shapebox;
                shapebox = nodePmt->GetVolume()->GetVoxels()->GetBoxes();
                m_Pmt_r_20inch_Hama = shapebox[0]*GeoUtil::cm2mm();
            }

            if(m_Pmt_r_3inch <0&&phyNodeName.Contains("3inch"))
            {
                Double_t *shapebox;
                shapebox = nodePmt->GetVolume()->GetVoxels()->GetBoxes();
                m_Pmt_r_3inch = shapebox[0]*GeoUtil::cm2mm();
            }


            // fast init PmtGeom with center and axisDir
            if (m_fastInit)
            { 
                pmt = addPmt(pmtID, 0, pmtType);
            }else
            {
                // full init PmtGeom with a TGeoPhysicalNode
                phyNode = m_geom->MakePhysicalNode(phyNodeName);
                pmt = addPmt(pmtID, phyNode, pmtType);
            }
            
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
        
            
            if ( getVerb() >= 2 && (iNode < 10 || nNodesPmtLevel-iNode  <= 970))
            {
                if (m_fastInit) 
                {
                    std::cout << "pmt " << iPmt << " center "; pmt->getCenter().Print();
                }
                else 
                {    
                    std::cout << phyNode->GetName() << std::endl;
                    phyNode->Print();
                }
            }
            iPmt++;
        }

    }
    //mean radius of PMT
    m_Pmt_r_20inch = (m_Pmt_r_20inch_Hama + m_Pmt_r_20inch_NNVT)/2;
    if ( getVerb() >= 1) std::cout << "Auto CdDetector Pmt size " << m_mapIdToPmt.size() << std::endl;
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " end " << std::endl;
    return true;
}

void CdGeom::analyzeGeomStructure()
{   
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    m_isPmtMotherFound = false;
    m_nodeMotherVec.clear();
    searchPmtMother( m_geom->GetTopNode() );
    if ( getVerb() >= 1) std::cout << "pmtMother " << m_nodePmtMother->GetName() << " nChild " << m_nodePmtMother->GetNdaughters() << std::endl;
    //if ( getVerb() >= 1) std::cout << "pmtMother " << m_geom->GetTopNode()->GetName() << " nChild " << m_geom->GetTopNode()->GetNdaughters() << std::endl;
    if ( getVerb() >= 1) std::cout << "pathMother " << m_pathMother << std::endl;
    
    searchPmt();
    if ( getVerb() >= 1) std::cout << "nPmt " << findPmtNum() << std::endl;
    if ( getVerb() >= 1) {
        std::cout << "Before set Module Range" << std::endl
            << "Module Range [" << CdID::moduleMin() << ", " << CdID::moduleMax() << "]" << std::endl
            << "Module20inch Range [" << CdID::module20inchMin() << ", " << CdID::module20inchMax() << "]" << std::endl
            << "Module3inch Range [" << CdID::module3inchMin() << ", " << CdID::module3inchMax() << "]" << std::endl;
    }
    CdID::setModule20inchMin( 0 );
    CdID::setModule20inchMax( findPmt20inchNum()-1 );
    CdID::setModule3inchMin( findPmt20inchNum() );
    CdID::setModule3inchMax( findPmt20inchNum()+findPmt3inchNum()-1 );
    CdID::setModuleMax( findPmtNum()-1 );

    if ( getVerb() >= 1) {
        std::cout << "After set Module Range" << std::endl
            << "Module Range [" << CdID::moduleMin() << ", " << CdID::moduleMax() << "]" << std::endl
            << "Module20inch Range [" << CdID::module20inchMin() << ", " << CdID::module20inchMax() << "]" << std::endl
            << "Module3inch Range [" << CdID::module3inchMin() << ", " << CdID::module3inchMax() << "]" << std::endl;
    }

    m_isPmtBottomFound = false;
    m_nodeBottomVec.clear();
    if ( findPmt20inchNum() > 0 ) {
        searchPmtBottom( m_nodePmt20inch );
        m_pathBottom20inch = setPathBottom();
        if ( getVerb() >= 1) std::cout << "pathBottom20inch " << m_pathBottom20inch << std::endl;

        if (m_nodePmt20inchNNVT) {
            m_isPmtBottomFound = false;
            m_nodeBottomVec.clear();
            searchPmtBottom( m_nodePmt20inchNNVT );
            m_pathBottom20inchNNVT = setPathBottom();
        }
        if (m_nodePmt20inchHama) {
            m_isPmtBottomFound = false;
            m_nodeBottomVec.clear();
            searchPmtBottom( m_nodePmt20inchHama );
            m_pathBottom20inchHama = setPathBottom();
        }
    }

    m_isPmtBottomFound = false;
    m_nodeBottomVec.clear();
    if ( findPmt3inchNum() > 0 ) {
        searchPmtBottom( m_nodePmt3inch );
        m_pathBottom3inch = setPathBottom();
        if ( getVerb() >= 1) std::cout << "pathBottom3inch " << m_pathBottom3inch << std::endl;
    }
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " end " << std::endl;
}

void CdGeom::searchPmt()
{
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    m_nPmt = 0;
    m_nPmt20inch = 0;
    m_nPmt20inchHama = 0;
    m_nPmt20inchNNVT = 0;
    m_nPmt3inch = 0;
    assert(m_nodePmtMother);
    int nNodesPmtLevel = m_nodePmtMother->GetNdaughters();

//    m_volPmt20inch = m_nodePmtMother->GetDaughter(nNodesPmtLevel/2)->GetVolume();
//    TString volPmtName = m_volPmt20inch->GetName();
//    if ( getVerb() >= 1) std::cout << "PmtName " << volPmtName << std::endl;

    m_nodePmt20inch = 0;
    m_nodePmt20inchNNVT = 0;
    m_nodePmt20inchHama = 0;
    m_nodePmt3inch = 0;
    for ( int iNode = 0; iNode < nNodesPmtLevel; iNode++ ) {
        TString volPmtName = m_nodePmtMother->GetDaughter(iNode)->GetVolume()->GetName();
        if ( getVerb() >= 2) std::cout << iNode << " " << volPmtName << std::endl;
  
        if ( volPmtName.Contains("PMT_20inch") || volPmtName.Contains("Mask") ) {
            m_nodePmt20inch = m_nodePmtMother->GetDaughter(iNode);
            m_nPmt20inch++;
            if ( volPmtName.Contains("NNVT") ) {
                m_nodePmt20inchNNVT = m_nodePmt20inch;
                ++m_nPmt20inchNNVT;
            } else if ( volPmtName.Contains("Hama") ) {
                m_nodePmt20inchHama = m_nodePmt20inch;
                ++m_nPmt20inchHama;
            }
        }
        else if ( volPmtName.Contains("PMT_3inch") ) {
            m_nodePmt3inch = m_nodePmtMother->GetDaughter(iNode);
            m_nPmt3inch++;
        }       
    }
    m_nPmt = m_nPmt20inch + m_nPmt3inch;
    std::cout << "nPmt " << m_nPmt << " nPmt20inch " << m_nPmt20inch << " nPmt3inch " << m_nPmt3inch << std::endl;
    std::cout << " nPmt20inch(NNVT): " << m_nPmt20inchNNVT
              << " nPmt20inch(Hama): " << m_nPmt20inchHama
              << std::endl;
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " end " << std::endl;
}

void CdGeom::searchPmtMother(TGeoNode* node)
{   
    if ( getVerb() >= 3) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    if ( !m_isPmtMotherFound ) {
      m_nodeMotherVec.push_back(node);
    }

    assert(node);
    int nChild = node->GetNdaughters();
    if ( nChild > 10000 ) {  // assume the total PMT in CD is greater than 10000, so this level
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

void CdGeom::searchPmtBottom(TGeoNode* node)
{   
    assert(node);

    if ( getVerb() >= 1) std::cout << __func__ << " node " << node->GetName() << std::endl;
    if ( !m_isPmtBottomFound ) {
        m_nodeBottomVec.push_back(node);
        if ( getVerb() >= 2) std::cout << "nodeSize " << m_nodeBottomVec.size() << std::endl;
    }

    if ( TString(node->GetVolume()->GetName()).Contains("inch_inner2") ) {  // PMT_20inch_inner2 or PMT_3inch_inner2
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


void CdGeom::setPathMother()
{
    if ( getVerb() >= 3) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    m_pathMother = TString("");
    if ( getVerb() >= 3) std::cout << "m_nodeMotherVec.size :" << m_nodeMotherVec.size() << std::endl;

    for (int iNode = 0; iNode < (int)m_nodeMotherVec.size(); iNode++) {
        m_pathMother += "/";
        m_pathMother += m_nodeMotherVec[iNode]->GetName();
        if ( getVerb() >= 3) std::cout << m_nodeMotherVec[iNode]->GetName() << std::endl;
    }
}

TString CdGeom::setPathBottom()
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

PmtGeom* CdGeom::addPmt(Identifier id, TGeoPhysicalNode *phyNode, int pmtType)
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

PmtGeom* CdGeom::getPmt(Identifier id)
{
    auto it = m_mapIdToPmt.find(id);
    if ( it == m_mapIdToPmt.end() ) {
        std::cerr << "id " << id << "(" << CdID::module(id) << ", " 
            << ")'s PmtGeom does not exist " << std::endl;
        return 0;


    }
    return it->second;
}

PmtGeom* CdGeom::getPmt(int layer, int azimuth, int pmt)
{
    int module = 0;
    for (int i = 0; i < layer; i++) {
        module += getAzimuthNum(i);
    }
    module += azimuth;

    if ( getVerb() >= 2) std::cout << "layer " << layer << " azimuth " << azimuth << " module " << module << std::endl;
    Identifier pmtID = CdID::id(module, pmt);
  
    return getPmt(pmtID);
}

void CdGeom::printPmt()
{  
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    if ( getVerb() >= 2) {

        std::cout << "Print first 17 Pmt... " << std::endl;
        for (PmtMapIt it = m_mapIdToPmt.begin(); it != m_mapIdToPmt.end(); it++) {
            Identifier pmtID = it->first;
            PmtGeom *pmt = it->second;
            //std::cout << CdID::module(pmtID) << std::endl;
            if (CdID::module(pmtID) < 17) {
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
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " end " << std::endl;
}

PmtGeom* CdGeom::findCrossedPmt(const TVector3 vtx, const TVector3 dir)
{
    for (PmtMapIt it = m_mapIdToPmt.begin(); it != m_mapIdToPmt.end(); it++) {
        //Identifier pmtID = it->first;
        PmtGeom *pmt = it->second;

        bool isCrossed = pmt->isCrossed(vtx, dir);
        if (isCrossed) {
            return pmt;
        }
    }

    return 0;
}

PmtGeom* CdGeom::findCrossedPmtFast(const TVector3 vtx, const TVector3 dir)
{
    //FIXME: Faster algorithm to be developed by esitmation and compare neighboring Pmt distance

    // get first Pmt id, use it to calculate sphere radius
    Identifier id0 = CdID::id(0, 0);
    double sphereR = getPmt(id0)->getCenter().Mag();

    // get intersection with sphere
    TVector3 intersection = GeoUtil::getSphereIntersection(vtx, dir, sphereR);
    double cosTheta = intersection.z()/intersection.Mag();

    // estimate start and end Pmt 
    int startPmt = int((0.5*(1-cosTheta)-0.03)*findPmt20inchNum());
    int endPmt   = int((0.5*(1-cosTheta)+0.03)*findPmt20inchNum());
    if (cosTheta >=  0.9) startPmt = 0;
    if (cosTheta <= -0.9) endPmt   = getPmtNum();
    int KS = 6;
    if ( getVerb() >= 3) std::cout << "cos " << std::setw(2*KS) << cosTheta << " start " << std::setw(KS) << startPmt << " end " << std::setw(KS) << endPmt << " ";

    for (int iPmt = startPmt; iPmt < endPmt; iPmt++) {
        Identifier PmtId = CdID::id(iPmt, 0);
        PmtGeom *pmt = getPmt(PmtId);

        bool isCrossed = pmt->isCrossed(vtx, dir);
        if (isCrossed) {
            return pmt;
        }
    }

    return 0;
}

bool CdGeom::orgnizePmt()
{
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
    int layer = -1;
    int azimuth = 0;
    std::vector<int> azimuthOnLayer;
    TVector3 center;
    TVector3 center_old(-9, -9, -9);

    for (PmtMapIt it = m_mapIdToPmt.begin(); it != m_mapIdToPmt.end(); it++) {
        Identifier pmtID = it->first;
        PmtGeom *pmt = it->second;
        if ( getVerb() >= 3) std::cout << CdID::module(pmtID) << std::endl;
 
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

int CdGeom::getAzimuthNum(int layer)
{
    if ( layer >= 0 && layer < (int)m_azimuthNum.size() ) {
        return m_azimuthNum[layer];
    }
    else {
        std::cerr << "CdGeom::getAzimuthNum at layer " << layer << " is wrong" << std::endl;
        return -9;
    }
}

void CdGeom::printCd()
{
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;
 
    
	std::cout << "CenterDetAcrylicBall getCdBallRmax() = " << getCdBallRmax()       << std::endl
	          << "CenterDetAcrylicBall getCdBallRmin() = " << getCdBallRmin()       << std::endl;

    if (m_AcrylicIsComposite) 
    {
    std::cout << "CenterDetChimney getCdChimneyRmax() = "     << getCdChimneyRmax()    << std::endl
              << "CenterDetChimney getCdChimneyRmin() = "     << getCdChimneyRmin()    << std::endl
              << "CenterDetChimney getCdChimneyDz() = "       << getCdChimneyDz()      << std::endl
              << "CenterDetChimneyPos getChimneyTrans()[0] = "<< getChimneyTrans()[0]  << ", getChimneyTrans()[1] = " << getChimneyTrans()[1] << ", getChimneyTrans()[2] = " << getChimneyTrans()[2] << std::endl;
    }
    std::cout << "CenterDetPmtFromTheCenter getCdPmtR() = "<< getCdPmtR() <<std::endl
              << "CenterDet20inchPmtRadius(NNVT) getPmt_r_20inch_NNVT() = "<< getPmt_r_20inch_NNVT() <<" mm"<<std::endl
              << "CenterDet20inchPmtRadius(Hamamatasu) getPmt_r_20inch_Hama() = "<< getPmt_r_20inch_Hama() <<" mm"<<std::endl
              << "CenterDet3inchPmtRadius getPmt_r_3inch() = "<< getPmt_r_3inch() <<" mm"<<std::endl;
    

    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " end " << std::endl;
} 

void CdGeom::initCdInfo()
{
    if ( getVerb() >= 1) std::cout << "----CdGeom function  " << __func__ << " begin " << std::endl;

    //Cd Acrylic
    TGeoNode *nodeAcrylic = m_nodePmtMother->GetDaughter(0);
    TString phyNodeName = m_pathMother + "/" + nodeAcrylic->GetName();
    TString volAcrylicName = nodeAcrylic->GetVolume()->GetName();

    if ( getVerb() >= 2){ 
    std::cout << __func__ << " volCdName " << volAcrylicName << std::endl;
    std::cout << __func__ << " phyNodeName " << phyNodeName << std::endl;
    }

    TGeoCompositeShape *pAcrylic = (TGeoCompositeShape *)(nodeAcrylic->GetVolume()->GetShape());
    m_AcrylicIsComposite = pAcrylic->IsComposite();
    if (m_AcrylicIsComposite) {
	std::cout << "Cd AcrylicBall is composite shape" << std::endl;
	TGeoBoolNode *pAcrylicBoolNode = pAcrylic->GetBoolNode();
	if (!pAcrylicBoolNode) std::cout << "pAcrylicBoolNode is null" << std::endl;
  
	pAcrylicBall = (TGeoSphere *)(pAcrylicBoolNode->GetLeftShape());
	pAcrylicChimney = (TGeoTube *)(pAcrylicBoolNode->GetRightShape());
	pChimneyTrans = pAcrylicBoolNode->GetRightMatrix(); 
    }
    else {
	std::cout << "Cd AcrylicBall is not composite shape" << std::endl;
	pAcrylicBall = (TGeoSphere *)(nodeAcrylic->GetVolume()->GetShape());
    }
    //pAcrylicBall->InspectShape();
    //pAcrylicChimney->InspectShape();

    // Target liquid scintillator
    TGeoNode *nodeLs = m_nodePmtMother->GetDaughter(0)->GetDaughter(0);
    TGeoCompositeShape *pLs = (TGeoCompositeShape *)(nodeLs->GetVolume()->GetShape());
    m_LsIsComposite = pLs->IsComposite();
    if (m_LsIsComposite) {
	std::cout << "LS is composite shape" << std::endl;
	TGeoBoolNode *pLsBoolNode = pLs->GetBoolNode();
	if (!pLsBoolNode) std::cout << "pLsBoolNode is null" << std::endl;

	pLsBall = (TGeoSphere *)(pLsBoolNode->GetLeftShape());
	pLsChimney = (TGeoTube *)(pLsBoolNode->GetRightShape());
    }
    else {
	std::cout << "LS is not composite shape" << std::endl;
	pLsBall = (TGeoSphere *)(nodeLs->GetVolume()->GetShape());
    }

    // PMT R form center and r of itself
    // 20inch
    Identifier PmtId0 = CdID::id(CdID::module20inchMin(), 0);  // north
    Identifier PmtId1 = CdID::id(int((CdID::module20inchMin()+CdID::module20inchMax())/2), 0); // equator
    PmtGeom *pmt0 = getPmt(PmtId0);
    PmtGeom *pmt1 = getPmt(PmtId1);
    m_Pmt_R_20inch_0 = pmt0->getR();
    m_Pmt_R_20inch_1 = pmt1->getR();

    if ( findPmt3inchNum() > 0 ) {
    // 3inch Pmt seem don't have Rules of id arrangement in space. Just give two points for instance.
    PmtId0 = CdID::id(CdID::module3inchMin(), 0);  
    PmtId1 = CdID::id(int((CdID::module3inchMin()+CdID::module3inchMax())/2), 0);
    pmt0 = getPmt(PmtId0);
    pmt1 = getPmt(PmtId1);
    m_Pmt_R_3inch_0 = pmt0->getR();
    m_Pmt_R_3inch_1 = pmt1->getR();
    }
    
    //std::cout << __func__ << " end" << std::endl;
}

double CdGeom::getCdPmtR_20inch( const int type )
{
    if (type == 1 ) // default one; probably the equator
    {
        return m_Pmt_R_20inch_1; 
    }
    else if (type == 0 ) // probably the north pole
    {
        return m_Pmt_R_20inch_0;
    }
    else if ( type == -1 ) // check all pmts
    {
        double R_Max = 0;
        double R_Min = 9999999;
        int n_Max = 0;
        int n_Min = 0;
        int startPmt = CdID::module20inchMin();
        int endPmt   = CdID::module20inchMax();
        for (int iPmt = startPmt; iPmt < endPmt; iPmt++) 
        {
            Identifier PmtId = CdID::id(iPmt, 0);
            PmtGeom *pmt = getPmt(PmtId);
            double R = pmt->getR();
            if (R >= R_Max) { R_Max = R; }
            if (R <= R_Min) { R_Min = R; }
        }
        for (int iPmt = startPmt; iPmt < endPmt; iPmt++) 
        {
            Identifier PmtId = CdID::id(iPmt, 0);
            PmtGeom *pmt = getPmt(PmtId);
            double R = pmt->getR();
            if (abs(R-R_Max)<0.01) { n_Max++; }
            if (abs(R-R_Min)<0.01) { n_Min++; }
        }
        
        std::cout<<"INFO: "<<n_Max<<" PMTs at radius R_Max "<<R_Max<<" mm "<<std::endl;
        std::cout<<"INFO: "<<n_Min<<" PMTs at radius R_Min "<<R_Min<<" mm "<<std::endl;
        if (n_Min == n_Max ){n_Min = 0; };
        std::cout<<"INFO: total "<<n_Max+n_Min<<" out of all "<<m_nPmt20inch<<"  20inch PMTs"<<std::endl;
        std::cout<<"INFO: now return R_Max "<<R_Max<<std::endl;
        return R_Max;
    }else{
        std::cout<<"ERROR: "<<"Wrong type input!"<<std::endl;
        return 0;
    }
}

double CdGeom::getCdPmtR_3inch( const int type )
{
    if (type == 1 ) // default one; 
    {
        return m_Pmt_R_3inch_1; 
    }
    else if (type == 0 ) 
    {
        return m_Pmt_R_3inch_0;
    }
    else if ( type == -1 ) // check all pmts
    {
        double R_Max = 0;
        double R_Min = 9999999;
        int n_Max = 0;
        int n_Min = 0;
        int startPmt = CdID::module3inchMin();
        int endPmt   = CdID::module3inchMax();
        for (int iPmt = startPmt; iPmt < endPmt; iPmt++) 
        {
            Identifier PmtId = CdID::id(iPmt, 0);
            PmtGeom *pmt = getPmt(PmtId);
            double R = pmt->getR();
            if (R >= R_Max) { R_Max = R; }
            if (R <= R_Min) { R_Min = R; }
        }
        for (int iPmt = startPmt; iPmt < endPmt; iPmt++) 
        {
            Identifier PmtId = CdID::id(iPmt, 0);
            PmtGeom *pmt = getPmt(PmtId);
            double R = pmt->getR();
            if (abs(R-R_Max)<0.01) { n_Max++; }
            if (abs(R-R_Min)<0.01) { n_Min++; }
        }
        
        std::cout<<"INFO: "<<n_Max<<" PMTs at radius R_Max "<<R_Max<<" mm "<<std::endl;
        std::cout<<"INFO: "<<n_Min<<" PMTs at radius R_Min "<<R_Min<<" mm "<<std::endl;
        if (n_Min == n_Max ){n_Min = 0; };
        std::cout<<"INFO: total "<<n_Max+n_Min<<" out of all "<<m_nPmt3inch<<"  3inch PMTs"<<std::endl;
        std::cout<<"INFO: now return R_Max "<<R_Max<<std::endl;
        return R_Max;
    }else{
        std::cout<<"ERROR: "<<"Wrong type input!"<<std::endl;
        return 0;
    }
}  

bool CdGeom::getNearby(Identifier id, const double radius, const int pmtType)
{   
    // initialize variable
    // pmtType: // 1: 20 inch; 2: 3inch;  -1: all types;
    int num = 0;
    m_numNearby= num;   // number of neighbors
    // radius determines how many neighbors can be found
    m_PmtIdVec.clear();
    m_PmtGeomVec.clear();
    m_isNearbyPmtFound = false;

    // the center pmt
    PmtGeom *pmt0 = getPmt(id);
    // Estimate search Nearby range by where the pmt is. The range is from startPmt to endPmt
    TVector3 posit0 = pmt0 ->getCenter();
    double cosTheta = posit0.z()/posit0.Mag();
    int startPmt[2]={0,0};
    int endPmt[2]={0,0};

    if(pmtType == 1 || pmtType == -1 ){
        startPmt[0] = int((0.5*(1-cosTheta)-0.03)*findPmt20inchNum());
        endPmt[0]   = int((0.5*(1-cosTheta)+0.03)*findPmt20inchNum());
        if (cosTheta >=  0.9) startPmt[0] = 0;
        if (cosTheta <= -0.9) endPmt[0]   = findPmt20inchNum();
    }

    if(pmtType == 2 || pmtType == -1 ){
        //startPmt[1] = CdID::module3inchMin() + int((0.5*(1-cosTheta)-0.05)*findPmt3inchNum());
        //endPmt[1]   = CdID::module3inchMin() + int((0.5*(1-cosTheta)+0.05)*findPmt3inchNum());
        //if (cosTheta >=  0.9) 
        //seems the 3inch pmts don't have regular named order.
        startPmt[1] = CdID::module3inchMin();
        //if (cosTheta <= -0.9) 
        endPmt[1]   = CdID::module3inchMax();
    }

    if ( getVerb() >= 1)
    {
        int KS = 6;
        std::cout << "cos " << std::setw(2*KS) << cosTheta <<std::endl;
        std::cout << "start[0]" << std::setw(KS) << startPmt[0] << " end[0] " << std::setw(KS) << endPmt[0] << " \n";
        std::cout << "start[1]" << std::setw(KS) << startPmt[1] << " end[1] " << std::setw(KS) << endPmt[1] << " \n";
        std::cout << "Module Range [" << CdID::moduleMin() << ", " << CdID::moduleMax() << "]" << std::endl
                << "Module20inch Range [" << CdID::module20inchMin() << ", " << CdID::module20inchMax() << "]" << std::endl
                << "Module3inch Range [" << CdID::module3inchMin() << ", " << CdID::module3inchMax() << "]" << std::endl;
    }

    if(pmtType == 1 || pmtType == -1)
    {
        for (int iPmt = startPmt[0]; iPmt < endPmt[0]; iPmt++) 
        {
            Identifier PmtId1 = CdID::id(iPmt, 0);
            PmtGeom *pmt1 = getPmt(PmtId1);
            
            TVector3 posit1 = pmt1 ->getCenter();
            // calculate distance
            TVector3 dis01= posit1 - posit0;
            double distance = dis01.Mag(); //unit mm
            
            // determine if it is less than 25inch(dis0 = 63.49cm)
            if(distance > 0.1 &&distance <= radius){
                num++;
                if ( getVerb() >= 1) std::cout<<"NearbyPmt No."<<std::setw(2)<< num <<" id:"<< std::setw(8)<<CdID::module(PmtId1)<<" "<<PmtId1.getString()
                <<"  distance:"<<distance<<"mm    "
                <<"is20inch "<<int(CdID::is20inch(PmtId1))<<std::endl;
                m_PmtIdVec.push_back(PmtId1);
                m_PmtGeomVec.push_back(pmt1);
            }
        }      
    }

    if(pmtType == 2 || pmtType == -1)
    {
        for (int iPmt = startPmt[1]; iPmt < endPmt[1]; iPmt++) 
        {
            Identifier PmtId1 = CdID::id(iPmt, 0);
            PmtGeom *pmt1 = getPmt(PmtId1);
            
            TVector3 posit1 = pmt1 ->getCenter();
            // calculate distance
            TVector3 dis01= posit1 - posit0;
            double distance = dis01.Mag(); //unit mm
            
            // determine if it is less than 25inch(dis0 = 63.49cm)
            if(distance > 0.1 &&distance <= radius){
                num++;
                if ( getVerb() >= 1) std::cout<<"NearbyPmt No."<<std::setw(2)<< num <<" id:"<< std::setw(8)<<CdID::module(PmtId1)<<" "<<PmtId1.getString()
                <<"  distance:"<<distance<<"mm    "
                <<"is20inch "<<int(CdID::is20inch(PmtId1))<<std::endl;
                m_PmtIdVec.push_back(PmtId1);
                m_PmtGeomVec.push_back(pmt1);
            }
        }      
    }
    // end of search
    m_numNearby = num;
    if(m_numNearby>0)
    {
        m_isNearbyPmtFound = true;
        return true;
    }
    else 
    {
    m_isNearbyPmtFound = false;
    return false;
    }
        
}

PmtIdVec CdGeom::getNearbyPmtId(Identifier id, const double radius, const int pmtType)
{   
    getNearby(id, radius,pmtType);
    if(m_isNearbyPmtFound==false){
        std::cout<<"Did not find any close pmt under this cut distance, please adjust parameters."<<std::endl;
    }else{
        if ( getVerb() >= 1) std::cout<<"The selected Pmt has "<<m_numNearby<<" close pmts, now they are in a Identifier container as the return value."<<std::endl;
    }
    return m_PmtIdVec;
}

PmtGeomVec CdGeom::getNearbyPmtGeom(Identifier id, const double radius, const int pmtType)
{   
    getNearby(id, radius,pmtType);
    if(m_isNearbyPmtFound==false){
        std::cout<<"Did not find any close pmt under this cut distance, please adjust parameters."<<std::endl;
    }else{
        if ( getVerb() >= 1) std::cout<<"The selected Pmt has "<<m_numNearby<<" close pmts, now they are in a PmtGeom* container as the return value."<<std::endl;
    }
    return m_PmtGeomVec;
}

PmtIdVec CdGeom::getCrossedPmtsId(const TVector3 vtx, const TVector3 dir, const double radius, const int pmtType)
{
    // Find the crossed Pmt
    PmtGeom *pmt = findCrossedPmtFast(vtx,dir);
    Identifier id = pmt->getId();

    // Get the nearby Pmts around this Pmt by a limited radius.
    // Set a container to store the pmt around(vector)
    getNearbyPmtId(id,radius,pmtType);

    return m_PmtIdVec;
}


PmtGeomVec CdGeom::getCrossedPmtsGeom(const TVector3 vtx, const TVector3 dir, const double radius, const int pmtType)
{
    // Find the crossed Pmt
    PmtGeom *pmt = findCrossedPmtFast(vtx,dir);
    Identifier id = pmt->getId();
    // Get the nearby Pmts around this Pmt by a limited radius.
    // Set a container to store the pmt around(vector)
    
    getNearbyPmtGeom(id,radius,pmtType);

    return m_PmtGeomVec;
}


bool CdGeom::getTotalReflection(const TVector3 vtx, const int pmtType)
{   

    // variable
    m_numTotalReflectionPmt= 0;   // number of PMT
    m_numALLTotalReflectionPmt = 0;
    // radius determines how many neighbors can be found
    m_PmtIdVec.clear();
    m_PmtGeomVec.clear();
    m_isTotalReflectionPmtFound = false;

    // Numerically calculated area first
    double ball_R = getCdBallRmin();
    double r = vtx.Mag();

    if (r > ball_R)
    {   
        std::cout<<"ERROR: the given vertex is out of ball"<<std::endl;
        m_isTotalReflectionPmtFound = false;
        return m_isTotalReflectionPmtFound;
    }else if (r <15000)
    {
        std::cout<<"WARNING: the given vertex is too small to have total reflection region"<<std::endl;
        m_isTotalReflectionPmtFound = false;
        return m_isTotalReflectionPmtFound;
    }

    double n_water = 4./3.;
    double n_LS = 1.496;
    double phi_LS_total_reflection = TMath::ASin(n_water/n_LS);
    double theta_0 = TMath::ASin(ball_R/r*n_water/n_LS) - phi_LS_total_reflection;
    double theta_1 = TMath::Pi()- TMath::ASin(ball_R/r*n_water/n_LS) - phi_LS_total_reflection;
    if ( getVerb() >= 0) printf("For Vertex: %.1f, %.1f, %.1f. Total Reflection Zone: theta_1 %.4f to %.4f\n", vtx.X(), vtx.Y(), vtx.Z(), theta_0, theta_1); //rad


    for (PmtMapIt it = m_mapIdToPmt.begin(); it != m_mapIdToPmt.end(); it++) {
        //Identifier pmtID = it->first;
        PmtGeom *pmt = it->second;
        TVector3 pmt_pos = pmt ->getCenter();

        TVector3 l = pmt_pos -vtx;

        double theta = TMath::ACos(
            (pmt_pos.Mag2() + vtx.Mag2() - l.Mag2())/(2*pmt_pos.Mag()* vtx.Mag())
                    );
        
        if ( theta < theta_1 && theta > theta_0)
        {
            if (pmtType == 2 || pmtType == -1)
            {
                if (! pmt->is20inch())
                {
                    m_numTotalReflectionPmt++;
                    m_PmtIdVec.push_back(pmt->getId());
                    m_PmtGeomVec.push_back(pmt);
                }      
            }

            if(pmtType == 1 || pmtType == -1 )
            {
                if (pmt->is20inch())
                {
                    m_numTotalReflectionPmt++;
                    m_PmtIdVec.push_back(pmt->getId());
                    m_PmtGeomVec.push_back(pmt);
                }
            }
        
            m_numALLTotalReflectionPmt++;
        }
    }

    if (m_numTotalReflectionPmt>=0) m_isTotalReflectionPmtFound = true;

    std::cout<<"Select type: m_numTotalReflectionPmt="<<m_numTotalReflectionPmt<<"  ALL type: m_numALLTotalReflectionPmt = "<<m_numALLTotalReflectionPmt<<std::endl;

    return m_isTotalReflectionPmtFound;
}

PmtIdVec CdGeom::getTotalReflectionPmtId(const TVector3 vtx, const int pmtType)
{   
    getTotalReflection(vtx, pmtType);
    if(m_isTotalReflectionPmtFound==false){
        std::cout<<"Did not find any pmt in Total Reflection region for given vertex, please adjust parameters."<<std::endl;
    }else{
        if ( getVerb() >= 0) std::cout<<"The Total Reflection region for given vertex havs "<<m_numTotalReflectionPmt<<"  pmts, now they ara in a Identifier container as return value."<<std::endl;
    }
    return m_PmtIdVec;
}

PmtGeomVec CdGeom::getTotalReflectionPmtGeom(const TVector3 vtx, const int pmtType)
{   
    getTotalReflection(vtx, pmtType);
    if(m_isTotalReflectionPmtFound==false){
        std::cout<<"Did not find any pmt in Total Reflection region for given vertex, please adjust parameters."<<std::endl;
    }else{
        if ( getVerb() >= 0) std::cout<<"The Total Reflection region for given vertex havs "<<m_numTotalReflectionPmt<<"  pmts, now they ara in a PmtGeom* container as return value."<<std::endl;
    }
    return m_PmtGeomVec;
}

void CdGeom::testfunction()
{   
    std::cout<<"****************Begin of testfunction()***************"<<std::endl;
    std::cout<<"********************************************************"<<std::endl;
    /*
    PmtIdVec v;
    PmtIdIt it;
    v = getNearbyPmtId();

    it = v.begin();
    Identifier id = *it;
   

    PmtGeomVec v;
    PmtGeomIt it;
    v = getNearbyPmtGeom(test_id);
    it = v.begin();
    PmtGeom *pmt = (*it);
    
    pmt->print();
    
    Identifier test_id = CdID::id(0, 0);
    PmtGeom *test_pmt = getPmt(test_id);
    TVector3 test_pos = test_pmt->getCenter();
    test_pmt->print();
    test_pos.Print();
    std::cout<<test_pos.Mag()<<std::endl;

    int startPmt = CdID::module20inchMin();
    int endPmt   = CdID::module20inchMax();
    std::cout<<"start ID "<<startPmt<<";"<<"end ID "<<endPmt<<std::endl;
    for (int iPmt = startPmt+1; iPmt < endPmt; iPmt=iPmt+int(findPmt20inchNum()*0.5)) 
    {
        Identifier PmtId1 = CdID::id(iPmt, 0);
        PmtGeom *pmt1 = getPmt(PmtId1);
        TVector3 posit1 = pmt1 ->getCenter();
        //TVector3 zero(0,0,0);
        //TVector3 posit2 = pmt1 ->getGlobal(zero);
        std::cout<<PmtId1<<"            Mag: "<<posit1.Mag()<<std::endl;
        //std::cout<<PmtId1<<"local2globalMag: "<<posit2.Mag()<<std::endl;
        double cosTheta = posit1.z()/posit1.Mag();
        std::cout<<CdID::module(PmtId1)<<"    cosTheta: "<<cosTheta<<std::endl;

    }
    
    TVector3 vtx(9584.014468547788,9584.014468547788,9584.014468547788);
    getTotalReflectionPmtGeom(vtx,-1);
    */

    std::cout<<"********************************************************"<<std::endl;
    std::cout<<"******************End of testfunction()***************"<<std::endl;
}
