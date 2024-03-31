#ifndef JUNO_CD_GEOM_H
#define JUNO_CD_GEOM_H

//
//  This class provides geometry for Central Detector
//
//  Author: Zhengyun You  2013-11-20
//

#include "Identifier/Identifier.h"
#include "Geometry/PmtGeom.h"
#include "TString.h"
#include <map>
#include <vector>
#include "TGeoSphere.h"
#include "TGeoTube.h"
#include "GeoUtil.h"
#include "TGeoMatrix.h"
#include "TGeoVoxelFinder.h"
#include "TStopwatch.h"

class TGeoManager;
class TGeoVolume;
class TGeoNode;
class TGeoPhysicalNode;

typedef  std::vector<Identifier> PmtIdVec;
typedef  std::vector<PmtGeom*> PmtGeomVec;

class CdGeom
{
    public :

        typedef std::map<Identifier, PmtGeom*>::iterator PmtMapIt;
        typedef std::vector<PmtGeom*>::iterator PmtGeomIt;
        typedef std::vector<Identifier>::iterator PmtIdIt; 

        CdGeom();
        ~CdGeom();

        // Initialization
        //-----------------------------------------------
        bool init();
        bool initRootGeo();
        void setGeomFileName(std::string name) { m_geomFileName = name; }
        void setGeomPathName(std::string name) { m_geomPathName = name; }
        void setFastInit(bool fastInit) { m_fastInit = fastInit; }
        bool readRootGeoFile();

        // Getter
        //-----------------------------------------------
        unsigned int findPmtNum() { return m_nPmt; }
        unsigned int findPmt20inchNum() { return m_nPmt20inch; }
        unsigned int findPmt3inchNum() { return m_nPmt3inch; }

        PmtGeom* getPmt(Identifier id);
        PmtGeom* getPmt(int layer, int azimuth, int pmt);
        unsigned int getPmtNum() { return m_mapIdToPmt.size(); }
        int getPmtType(TString name);

        // Useful functions  
        //-----------------------------------------------
        void printPmt();
        void initCdInfo();
       
        // Find the intersected Pmt from a vertex + direction
        PmtGeom* findCrossedPmt(const TVector3 vtx, const TVector3 dir);
        PmtGeom* findCrossedPmtFast(const TVector3 vtx, const TVector3 dir);

        // Orgnize Pmts into layers
        bool orgnizePmt(); 
        int getLayerNum() { return m_layerNum; }
        int getAzimuthNum(int layer);
    
        // Verbosisty
        void setVerb(int value) { m_verb = value; }
        int  getVerb() { return m_verb; }

        // Get Cd info; unit mm
        double getCdBallRmax() { return pAcrylicBall->GetRmax()*GeoUtil::cm2mm(); }
        double getCdBallRmin() { return pLsBall->GetRmax()*GeoUtil::cm2mm(); }
        double getCdChimneyRmax() { return pAcrylicChimney->GetRmax()*GeoUtil::cm2mm(); } 
        double getCdChimneyRmin() { return pAcrylicChimney->GetRmin()*GeoUtil::cm2mm(); }   
        double getCdChimneyDz() { return pAcrylicChimney->GetDz()*GeoUtil::cm2mm(); }
        TVector3 getChimneyTrans() { return TVector3((pChimneyTrans->GetTranslation())[0], (pChimneyTrans->GetTranslation())[1], (pChimneyTrans->GetTranslation())[2])*GeoUtil::cm2mm(); } 
        void printCd();

        // Get Cd Pmt info; unit mm
        double getCdPmtR() { return m_Pmt_R_20inch_1; }
        // 1: default one; probably the equator ; 0: probably the north pole ; -1: check all pmts and return R_max;
        double getCdPmtR_20inch( const int type = 1 );
        double getCdPmtR_3inch( const int type = 1 );
        double getPmt_r_20inch() {return m_Pmt_r_20inch; }
        double getPmt_r_20inch_NNVT() {return m_Pmt_r_20inch_NNVT; }
        double getPmt_r_20inch_Hama() {return m_Pmt_r_20inch_Hama; }
        double getPmt_r_3inch() { return m_Pmt_r_3inch; }

        // input a pmt id, then return some pmts around it
        // pmtType means which type you want to find.
        // 1 means 20inch and 2 means 3inch. default -1 means that find all type Pmt.
        // There is a example of use in testFunction()
        PmtIdVec getNearbyPmtId(Identifier id, const double radius = 635,const int pmtType = 1);
        PmtGeomVec getNearbyPmtGeom(Identifier id, const double radius = 635,const int pmtType = 1);
        PmtIdVec getCrossedPmtsId(const TVector3 vtx, const TVector3 dir,const double radius = 635,const int pmtType = 1);
        PmtGeomVec getCrossedPmtsGeom(const TVector3 vtx, const TVector3 dir,const double radius = 635,const int pmtType = 1);

        // for given vertex, find all Pmts in total reflection zone as a return vector
        PmtIdVec getTotalReflectionPmtId(const TVector3 vtx, const int pmtType = -1);
        PmtGeomVec getTotalReflectionPmtGeom(const TVector3 vtx, const int pmtType = -1);
        
        void testfunction();
        

    private :

        // analyze TGeo tree and set 
        //-----------------------------------------------
        void analyzeGeomStructure();
        void searchPmt();
        void searchPmtMother(TGeoNode* node);
        void searchPmtBottom(TGeoNode* node);
        void setPathMother();
        TString setPathBottom();

        bool setPhyNodes();
        bool setPhyNodesManually();
        bool setPhyNodesAuto();
        PmtGeom* addPmt(Identifier id, TGeoPhysicalNode *phyNode, int pmtType);

        bool getNearby(Identifier id, const double radius, const int pmtType);

        bool getTotalReflection(const TVector3 vtx, const int pmtType);

        // members

        bool m_useDefaultGeom;
        std::string m_geomFileName;
        std::string m_geomPathName;
        TGeoManager *m_geom;
        std::map<Identifier, PmtGeom*> m_mapIdToPmt;
        bool m_fastInit;

        // root geometry structur analysis
        int m_nPmt;
        int m_nPmt20inch;
        int m_nPmt20inchNNVT;
        int m_nPmt20inchHama;
        int m_nPmt3inch;
        TGeoVolume* m_volPmt20inch;
        TGeoVolume* m_volPmt3inch;
        TGeoNode*   m_nodePmt20inch;
        TGeoNode*   m_nodePmt20inchHama;
        TGeoNode*   m_nodePmt20inchNNVT;
        TGeoNode*   m_nodePmt3inch;

        bool m_isPmtMotherFound;
        TGeoNode* m_nodePmtMother;
        std::vector<TGeoNode*> m_nodeMotherVec; 
        TString m_pathMother;

        bool m_isPmtBottomFound;
        std::vector<TGeoNode*> m_nodeBottomVec;
        TString m_pathBottom20inch; // default
        TString m_pathBottom20inchNNVT;
        TString m_pathBottom20inchHama;
        TString m_pathBottom3inch;

        // re-organize with layer
        int m_layerNum;
        std::vector<int> m_azimuthNum; 

        int m_verb;

        // get Cd info
        TGeoSphere *pAcrylicBall;
        TGeoTube *pAcrylicChimney;
        TGeoSphere *pLsBall;
        TGeoTube *pLsChimney;
        TGeoMatrix *pChimneyTrans;

        // cd PMT info
        double m_Pmt_R_20inch_0;
        double m_Pmt_R_20inch_1;
        double m_Pmt_R_3inch_0;
        double m_Pmt_R_3inch_1;
        double m_Pmt_r_20inch;
        double m_Pmt_r_20inch_Hama;
        double m_Pmt_r_20inch_NNVT;
        double m_Pmt_r_3inch;

		// is or not composite shape
		bool m_AcrylicIsComposite;
		bool m_LsIsComposite;

        // is or not Nearby found and its Nearby
        bool m_isNearbyPmtFound;
        int m_numNearby;
        PmtGeomVec m_PmtGeomVec;
        PmtIdVec m_PmtIdVec;

        // is or not TotalReflection PMTs found and all PMTs
        bool m_isTotalReflectionPmtFound;
        int m_numTotalReflectionPmt;
        int m_numALLTotalReflectionPmt;
        // used same temporary variable
        //PmtGeomVec m_PmtGeomVec;
        //PmtIdVec m_PmtIdVec;

};

#endif // JUNO_CD_GEOM_H
