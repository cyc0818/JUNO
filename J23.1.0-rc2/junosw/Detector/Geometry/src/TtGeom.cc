//
//  Author: Kaijie Li
//

#include "Geometry/TtGeom.h"
#include "Geometry/GeoUtil.h"
#include "Identifier/TtID.h"
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
#include "TGeoBBox.h"
#include "TGeoCompositeShape.h"
#include "TGeoBoolNode.h"
#include "TGeoTube.h"

TtGeom::TtGeom()
    : m_geom(0)
    , m_fastInit(false)
    , m_verb(0)
{
}

TtGeom::~TtGeom()
{
}

bool TtGeom::init()
{
    initRootGeo();

    return true;
}

bool TtGeom::initRootGeo()
{
    readRootGeoFile();
    setPhyNodes();
    printChannel();
   
    return true;
}

bool TtGeom::readRootGeoFile()
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
            geoFile = TString(geoDataPath) + geoFileName + geoFileType;
        }

        if ( getVerb() >= 1) {
            std::cout << "TtGeom::readRootGeoFile() Path: " << TString(geoDataPath) + geoFileName << std::endl;
            std::cout << "TtGeom::readRootGeoFile() verbose " << m_geom->GetVerboseLevel() << std::endl;
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

bool TtGeom::setPhyNodes()
{
    bool status = false;
    status = setPhyNodesAuto();

    return status;
}

int TtGeom::getChannelType(TString name)
{
    if ( name.Contains("Coating") ) return 1;
    else return 2;

    return 0;
}

bool TtGeom::setPhyNodesAuto()
{
    analyzeGeomStructure();

    TGeoPhysicalNode *phyNode = 0;

    int nNodesWallLevel = m_nodeChannelMother->GetNdaughters();
    std::cout << "number of  Walls " << nNodesWallLevel << std::endl;

    int iChannel = 0;
    for (int iNode = 0; iNode < nNodesWallLevel; iNode++) {
	TGeoNode *nodeWall = m_nodeChannelMother->GetDaughter(iNode);
	int nNodePlaneLevel = nodeWall->GetNdaughters();
	for (int jNode = 0; jNode < nNodePlaneLevel; jNode++) {
	    TGeoNode *nodePlane = nodeWall->GetDaughter(jNode);
	    int nNodePanelLevel = nodePlane->GetNdaughters();
	    for (int kNode = 0; kNode < nNodePanelLevel; kNode++) {
		TGeoNode *nodePanel = nodePlane->GetDaughter(kNode);
		int nPanelTapeLevel = nodePanel->GetNdaughters();
		for (int lNode = 0; lNode < nPanelTapeLevel; lNode++) {
		    TGeoNode *nodePanelTape = nodePanel->GetDaughter(lNode);
		    int nChannelLevel = nodePanelTape->GetNdaughters();
		    for (int mNode = 0; mNode < nChannelLevel; mNode++) {

			TGeoNode *nodeChannel = nodePanelTape->GetDaughter(mNode);

			TString phyNodeName = m_pathMother + "/" + nodeWall->GetName() + "/" + nodePlane->GetName() + "/" + nodePanel->GetName() + "/" + nodePanelTape->GetName() + "/" + nodeChannel->GetName();

			if (iNode == 0 && jNode == 0 && kNode == 0 && lNode == 0 && mNode == 0){
			    std::cout << "TTDetector phyNodeName " << phyNodeName << std::endl;
			}
			int channelType = getChannelType(nodeChannel->GetName());

			if (channelType == 1) {
			    Identifier channelID = TtID::id(phyNodeName.Data());
			    int wall  = TtID::wall_id(channelID);
			    int pmt   = TtID::pmt   (channelID);
			    int strip = TtID::strip (channelID);
			    pmt = TtID::pmt_other_end(pmt);
			    Identifier channelID_otherend = TtID::id(TtID::getIntID(wall,pmt,strip));
			    //Identifier channelID = TtID::id(0,iChannel);
			    PmtGeom* channel = 0;
			    phyNode = m_geom->MakePhysicalNode(phyNodeName);
			    channel = addChannel(channelID, phyNode, channelType);
			    phyNode = m_geom->MakePhysicalNode(phyNodeName);
			    channel = addChannel(channelID_otherend, phyNode, channelType);

         	            //if (iNode == 0 && jNode == 0 && kNode == 0 && lNode == 0 && mNode == 0)
			    //phyNode->Print();

			iChannel++;
			}
		   }
		}
	    }
	}
    }
    std::cout << "Auto TTDetector size " << m_mapIdToChannel.size() << std::endl;
    return true;
}

double TtGeom::getBoundingDx(){
    TGeoVolume * TTAir_vol = m_nodeChannelMother->GetVolume();
    TGeoCompositeShape * TTAir_shape = (TGeoCompositeShape*) TTAir_vol->GetShape();
    double half_width  = TTAir_shape->GetDX();
    return half_width*GeoUtil::cm2mm();
}

double TtGeom::getBoundingDy(){
    TGeoVolume * TTAir_vol = m_nodeChannelMother->GetVolume();
    TGeoCompositeShape * TTAir_shape = (TGeoCompositeShape*) TTAir_vol->GetShape();
    double half_width  = TTAir_shape->GetDY();
    return half_width*GeoUtil::cm2mm();
}

double TtGeom::getBoundingDz(){
    TGeoVolume * TTAir_vol = m_nodeChannelMother->GetVolume();
    TGeoCompositeShape * TTAir_shape = (TGeoCompositeShape*) TTAir_vol->GetShape();
    double half_width  = TTAir_shape->GetDZ();
    return half_width*GeoUtil::cm2mm();
}

std::map<Identifier, TVector3> * TtGeom::ms_StripSizeMap = new std::map<Identifier, TVector3>;

TVector3 TtGeom::GetStripSizeFromCache(PmtGeom* pmt){
    Identifier id = pmt->getId();
    if(ms_StripSizeMap->find(id) == ms_StripSizeMap->end()){
        TVector3 strip_center = pmt->getCenter();
        strip_center *= 1./GeoUtil::cm2mm();

        double box[3];
        strip_center.GetXYZ(box);

        box[0] += ((TGeoBBox*)(pmt->getPhyNode()->GetShape()))->GetDX();
        box[1] += ((TGeoBBox*)(pmt->getPhyNode()->GetShape()))->GetDY();
        box[2] += ((TGeoBBox*)(pmt->getPhyNode()->GetShape()))->GetDZ();

        double half_width[3];

        pmt->getPhyNode()->GetMatrix()->MasterToLocal(box, half_width);

        (*ms_StripSizeMap)[id] = TVector3(half_width)*GeoUtil::cm2mm();
    }
    return (*ms_StripSizeMap)[id];
}

double TtGeom::getStripDx(PmtGeom* pmt){
    TVector3 strip_size = GetStripSizeFromCache(pmt);
    return std::abs(strip_size.x());
}

double TtGeom::getStripDy(PmtGeom* pmt){
    TVector3 strip_size = GetStripSizeFromCache(pmt);
    return std::abs(strip_size.y());
}

double TtGeom::getStripDz(PmtGeom* pmt){
    TVector3 strip_size = GetStripSizeFromCache(pmt);
    return std::abs(strip_size.z());
}

void TtGeom::analyzeGeomStructure()
{
    m_isChannelMotherFound = false;
    m_nodeMotherVec.clear();
    searchChannelMother( m_geom->GetTopNode() );
    if ( getVerb() >= 1) std::cout << "channelMother " << m_nodeChannelMother->GetName() << " nChild " << m_nodeChannelMother->GetNdaughters() << std::endl;
    if ( getVerb() >= 1) std::cout << "pathMother " << m_pathMother << std::endl;
    
    searchWall();

    m_isChannelBottomFound = false;
    m_nodeBottomVec.clear();
    if ( findWallNum() > 0 ) {
        searchChannelBottom( m_nodeWall );
        m_pathBottomWall = setPathBottom();
        if ( getVerb() >= 1) std::cout << "pathBottom " << m_pathBottomWall << std::endl;
    }
}

void TtGeom::searchWall()
{
    m_nWall = 0;
    assert(m_nodeChannelMother);
    int nNodesWallLevel = m_nodeChannelMother->GetNdaughters();

    m_nodeWall = 0;
    for ( int iNode = 0; iNode < nNodesWallLevel; iNode++ ) {
        TString volChannelName = m_nodeChannelMother->GetDaughter(iNode)->GetVolume()->GetName();
        if ( volChannelName.Contains("Wall")) {
            m_nodeWall = m_nodeChannelMother->GetDaughter(iNode);
            m_nWall++;
        }
    }
    std::cout << "nWall " << m_nWall << std::endl;
}

void TtGeom::searchChannelMother(TGeoNode* node)
{   
    if ( !m_isChannelMotherFound ) {
      m_nodeMotherVec.push_back(node);
    }

    assert(node);
    int nChild = node->GetNdaughters();
    if ( nChild > 50 && nChild < 70 ) {  // assume the total walls in TT is greater than 50 and less than 70, so this level
        m_nodeChannelMother = node;
        m_isChannelMotherFound = true;
    }

    for (int iChild = 0; iChild < nChild && (!m_isChannelMotherFound); iChild++) {
        TGeoNode* childNode = node->GetDaughter(iChild);
        searchChannelMother(childNode);
        if ( !m_isChannelMotherFound ) {
            m_nodeMotherVec.pop_back();
        }
    } 

    setPathMother();
}   

void TtGeom::searchChannelBottom(TGeoNode* node)
{   
    assert(node);

    if ( getVerb() >= 1) std::cout << __func__ << " node " << node->GetName() << std::endl;
    if ( !m_isChannelBottomFound ) {
        m_nodeBottomVec.push_back(node);
        if ( getVerb() >= 2) std::cout << "nodeSize " << m_nodeBottomVec.size() << std::endl;
    }

    if ( TString(node->GetVolume()->GetName()).Contains("Bar") ) {
        m_isChannelBottomFound = true;
    }

    int nChild = node->GetNdaughters();
    for ( int iChild = 0; iChild < nChild && (!m_isChannelBottomFound); iChild++ ) {
        TGeoNode* childNode = node->GetDaughter(iChild);
        searchChannelBottom(childNode);
        if ( !m_isChannelBottomFound ) {
            m_nodeBottomVec.pop_back();
        }
    } 
}


void TtGeom::setPathMother()
{
    m_pathMother = TString("");
    if ( getVerb() >= 3) std::cout << m_nodeMotherVec.size() << std::endl;

    for (int iNode = 0; iNode < (int)m_nodeMotherVec.size(); iNode++) {
        m_pathMother += "/";
        m_pathMother += m_nodeMotherVec[iNode]->GetName();
        if ( getVerb() >= 3) std::cout << m_nodeMotherVec[iNode]->GetName() << std::endl;
    }
}

TString TtGeom::setPathBottom()
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

PmtGeom* TtGeom::addChannel(Identifier id, TGeoPhysicalNode *phyNode, int channelType)
{
    std::map<Identifier, PmtGeom*>::iterator it = m_mapIdToChannel.find(id);
    if ( it == m_mapIdToChannel.end() ) {
	PmtGeom* channel = new PmtGeom(id);
	channel->setPhyNode(phyNode);

	m_mapIdToChannel[id] = channel;

	return channel;
    }
    else {
        std::cerr  << "TT Channel with ID " << id << " already added." << std::endl;
        throw std::runtime_error("Adding TT channel to TtGeom that was already added before");
    }
}

PmtGeom* TtGeom::getChannel(Identifier id)
{
    std::map<Identifier, PmtGeom*>::iterator it = m_mapIdToChannel.find(id);
    if ( it == m_mapIdToChannel.end() ) {
        std::cerr << "id " << id << "(" << TtID::wall_id(id) << ", " << TtID::channel(id)
		    << ")'s Geom does not exist " << std::endl;
        return 0;
    }
    return m_mapIdToChannel[id];
}


void TtGeom::printChannel()
{
    std::cout << "topTracker " << __func__ << " begin " << getVerb() << std::endl;

    if ( getVerb() >= 1) {
        std::cout << "Print a few channels... " << std::endl;
        for (ChannelMapIt it = m_mapIdToChannel.begin(); it != m_mapIdToChannel.end(); it++) {
            Identifier pmtID = it->first;
            PmtGeom *pmt = it->second;
            if (TtID::wall_id(pmtID) == 0 || TtID::channel(pmtID) == 0) {
                pmt->print(3);
            }
	}

    }
}

double TtGeom::distanceInStrip(ChannelMapCIt id_pmt, const TVector3 vtx, const TVector3 dir){
    Identifier id = id_pmt->first;
    int side = -1;
    if(TtID::pmt(id) >= 12) side = 1;
    if(TtID::pmt(id) < 4) side = 1;
    PmtGeom * pmt = id_pmt->second;
    TVector3 StripCenter = pmt->getCenter();
    TVector3 StripWidth(getStripDx(pmt), getStripDy(pmt), getStripDz(pmt));
    bool crossed = false;
    double distance=0;
    for(int i = -1 ; i <= 1; i++){
        double zref = StripCenter.z() + i* StripWidth.z();
        double lambda = ( zref - vtx.z() ) / dir.z();
        TVector3 Crossing = vtx + lambda * dir - StripCenter;
        if(fabs(Crossing.x()) > StripWidth.x()) continue;
        if(fabs(Crossing.y()) > StripWidth.y()) continue;
        crossed = true;
        if(StripWidth.x() > StripWidth.y()) distance = side*Crossing.x() + StripWidth.x();
        else                                distance = side*Crossing.y() + StripWidth.y();
        if(i==0) break;
    }
    if(!crossed) return -1;
    return distance;
}

double TtGeom::distanceToStripZ(ChannelMapCIt id_pmt, const TVector3 vtx, const TVector3 dir){
    PmtGeom * pmt = id_pmt->second;
    TVector3 StripCenter = pmt->getCenter();
    double zref = StripCenter.z();
    double lambda = ( zref - vtx.z() ) / dir.z();
    double distance = lambda * dir.Mag();
    return distance;
}
