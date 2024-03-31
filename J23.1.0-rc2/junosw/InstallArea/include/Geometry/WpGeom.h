#ifndef JUNO_WP_GEOM_H
#define JUNO_WP_GEOM_H

//
//  This class provides geometry for Water pool
//
//  Author: Kaijie Li  2016-09-24
//

#include "Identifier/Identifier.h"
#include "Geometry/PmtGeom.h"
#include "TString.h"
#include <map>
#include <vector>
#include "GeoUtil.h"
#include "TGeoTube.h"

class TGeoManager;
class TGeoVolume;
class TGeoNode;
class TGeoPhysicalNode;

class WpGeom
{
    public :

        typedef std::map<Identifier, PmtGeom*>::iterator PmtMapIt; 

        WpGeom();
        ~WpGeom();

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

        PmtGeom* getPmt(Identifier id);
        PmtGeom* getPmt(int layer, int azimuth, int pmt);
        unsigned int getPmtNum() { return m_mapIdToPmt.size(); }
        int getPmtType(TString name);

        // Useful functions  
        //-----------------------------------------------
        void printPmt();
        void initWpInfo(); 

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

        // Get Wp info; unit mm
        double getWpRmax() { return pWp->GetRmax()*GeoUtil::cm2mm(); }
        double getWpDz() { return pWp->GetDz()*GeoUtil::cm2mm(); }         
        void printWp();

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
        TGeoVolume* m_volPmt20inch;
        TGeoNode*   m_nodePmt20inch;

        bool m_isPmtMotherFound;
        TGeoNode* m_nodePmtMother;
        std::vector<TGeoNode*> m_nodeMotherVec; 
        TString m_pathMother;

        bool m_isPmtBottomFound;
        std::vector<TGeoNode*> m_nodeBottomVec;
        TString m_pathBottom20inch;
        TString m_pathBottom3inch;

        // re-organize with layer
        int m_layerNum;
        std::vector<int> m_azimuthNum; 

        int m_verb;

        // get Wp info
        TGeoTube *pWp;
};

#endif // JUNO_WP_GEOM_H
