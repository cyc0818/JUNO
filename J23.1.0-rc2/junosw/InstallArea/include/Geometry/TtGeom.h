#ifndef JUNO_TT_GEOM_H
#define JUNO_TT_GEOM_H

//
//  This class provides geometry for Top Tracker
//
//  Author: kaijie Li
//

#include "Identifier/Identifier.h"
#include "Geometry/PmtGeom.h"
#include "TString.h"
#include <map>
#include <vector>
#include "TGeoBBox.h"
#include "TGeoTube.h"
#include "GeoUtil.h"
#include "TGeoMatrix.h"

class TGeoManager;
class TGeoVolume;
class TGeoNode;
class TGeoPhysicalNode;

class TtGeom
{
    public :

        typedef std::map<Identifier, PmtGeom*>::iterator ChannelMapIt; 
        typedef std::map<Identifier, PmtGeom*>::const_iterator ChannelMapCIt;

        TtGeom();
        ~TtGeom();

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
        unsigned int findWallNum() { return m_nWall; }

        PmtGeom* getChannel(Identifier id);
        unsigned int getChannelNum() { return m_mapIdToChannel.size(); }
        int getChannelType(TString name);

        ChannelMapCIt cbegin() const { return m_mapIdToChannel.begin(); }
        ChannelMapCIt cend  () const { return m_mapIdToChannel.end  (); }
        ChannelMapCIt cfind (Identifier id) const { return m_mapIdToChannel.find(id); }

        // Useful functions  
        //-----------------------------------------------
	void printChannel();

        // Find the intersected Strip from a vertex + direction -- comment out until implemented
        // As a side note, for the Tt we might have more than one strip intersecting a line.
        // PmtGeom* findCrossedStrip(const TVector3 vtx, const TVector3 dir);
        // PmtGeom* findCrossedStripFast(const TVector3 vtx, const TVector3 dir);

        // distance from PMT in strip.
        static double distanceInStrip(ChannelMapCIt id_pmt, const TVector3 vtx, const TVector3 dir);
        // distance for mu to get to strip. (crossing not checked)
        static double distanceToStripZ(ChannelMapCIt id_pmt, const TVector3 vtx, const TVector3 dir);

        // Verbosisty
        void setVerb(int value) { m_verb = value; }
        int  getVerb() { return m_verb; }

        // Functions defining the TT bounding box volume (TTAir) dimensions
        double getBoundingDx();
        double getBoundingDy();
        double getBoundingDz();

        // Functions to get specific TT strips dimensions -- half widths given
        static double getStripDx(PmtGeom* pmt);
        static double getStripDy(PmtGeom* pmt);
        static double getStripDz(PmtGeom* pmt);

    private :

        // analyze TGeo tree and set 
        //-----------------------------------------------
        void analyzeGeomStructure();
        void searchWall();
        void searchChannelMother(TGeoNode* node);
        void searchChannelBottom(TGeoNode* node);
        void setPathMother();
        TString setPathBottom();

        bool setPhyNodes();
        bool setPhyNodesManually();
        bool setPhyNodesAuto();
        PmtGeom* addChannel(Identifier id, TGeoPhysicalNode *phyNode, int channelType);

        static TVector3 GetStripSizeFromCache(PmtGeom* pmt);

        // members

        bool m_useDefaultGeom;
        std::string m_geomFileName;
        std::string m_geomPathName;
        TGeoManager *m_geom;
        std::map<Identifier, PmtGeom*> m_mapIdToChannel;
        bool m_fastInit;

        // Cache for strip sizes (useful to know if the strip is longer along x or y)
        static std::map<Identifier, TVector3> * ms_StripSizeMap;

        // root geometry structur analysis
        int m_nWall;
        TGeoNode*   m_nodeWall;

        bool m_isChannelMotherFound;
        TGeoNode* m_nodeChannelMother;
        std::vector<TGeoNode*> m_nodeMotherVec; 
        TString m_pathMother;

        bool m_isChannelBottomFound;
        std::vector<TGeoNode*> m_nodeBottomVec;
        TString m_pathBottomWall;

        int m_verb;

};

#endif // JUNO_TT_GEOM_H
