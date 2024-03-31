#ifndef JUNO_VIS_TOP_H
#define JUNO_VIS_TOP_H

//
//  Juno Visualization Top
//
//  Author: Zhengyun You  2013-12-25
//

#include "Identifier/Identifier.h"
#include "Identifier/IDService.h"

#include "TString.h"
#include <vector>
#include <map>

enum curDraw {PMTQPROJ, PMTTPROJ};

class JVisGeom;
class JVisEvtMgr;
class JVisStatus;
class JVisTimer;
class TH2F;
class TMarker;
class TVector3;
class TGWindow;
class TEvePointSet;
class TEveQuadSet;
class TEveBoxSet;
class TEveTrackList;
class TEveManager;
class TEveRGBAPalette;
class TEveRGBAPaletteOverlay;
class TEveArrow;
class TEveElement;

class JVisTop 
{
    public :

        JVisTop();
        virtual ~JVisTop();

        bool initialize();
        bool initStyle();
        bool initStatus();
        bool initGeom();
        bool initEvtMgr();
        bool initHisto();
        bool initEve();

        bool initPmtQProj();
        bool initPmtTProj();
        bool initEvePmt();
        bool initEveRec();
        bool initEveSimOp();
        bool initEveGeom();
        bool initEveTimer();

        void openRootFile();
        void closeRootFile();

        TEveManager* getEve();
        TGWindow* getEveWindow();

        void setEvtFile(TString simFile, TString calibFile, TString recFile, TString simusFile)
            { m_simFile = simFile; m_calibFile = calibFile; m_recFile = recFile; m_simusFile = simusFile; }
        void setGeomFile(TString geomFile) { m_geomFile = geomFile; }

        void setqtMode(int mode) { m_qtMode = mode; }
        int  getqtMode() {return m_qtMode; }

        void setEveRecConeMode(int mode) { m_eveRecConeMode = mode; }
        int  getEveRecConeMode() { return m_eveRecConeMode; }
        void changeEveRecConeMode();

        void setEvePaletteMode(int mode) { m_evePaletteMode = mode; }
        int  getEvePaletteMode() { return m_evePaletteMode; }
        void changeEvePaletteMode();

        bool runEvt(int iEvt);
        bool drawCurrentPad();
        bool drawPmtQ();
        bool drawPmtT(double tCut = 9e99);
        bool drawPmtQProj();
        bool drawPmtTProj(double tCut);
        bool drawTrkProj();
        bool drawEve();
        bool drawEvePmt();
        bool drawEveRec();
        bool drawEveTrk();
        bool drawEveSimOp();
        bool drawEveRecInfo(double peSum, TVector3 &recVtx, TVector3 &axisDir, double energy, double eprec, double dist=-1, double t=-1, double eqSpeed=-1);
        bool drawEveEvtInfo(); 

        bool animateEvePmtT(double tCut = 9e99);

        double getFirstHitTimeMin() { return m_firstHitTimeMin; }
        double getFirstHitTimeMax() { return m_firstHitTimeMax; }

        int  getnPEColor(double nPE);
        int  getTimeColor(double t);

        bool drawEveHomeView();
        bool drawEveMaxView();
        bool drawEveInnerView();

        JVisGeom*   getGeom();
        JVisEvtMgr* getEvtMgr();
        TH2F* getPmtQProj();
        TH2F* getPmtTProj();

        void setVerbosity(int verb) { m_verb = verb; }
        int  getVerbosity() { return m_verb; }

        int  getTotalEvent() { return totalEvent; }
    private :

        JVisGeom* m_geom;

        JVisEvtMgr* m_evtMgr;

        JVisStatus* m_status;

        TString  m_simFile;
        TString  m_calibFile;
        TString  m_recFile;
        TString  m_simusFile;
        TString  m_geomFile;

        bool   m_evtLoad;

        double m_firstHitTimeMin;
        double m_firstHitTimeMax;

        double m_nPEMin;
        double m_nPEMax;
        double m_tMin;
        double m_tMax;
        double m_tCut;
        double m_dMin;  // The minimum distance from vertex to the first hit PMT
        int m_nPaletteColor;

        std::map<Identifier, TMarker*> m_mapPmtQMarker;
        std::map<Identifier, TMarker*> m_mapPmtTMarker;

        curDraw m_curDraw;
        
        // TH2F
        TH2F *m_h2PmtQProj;
        TH2F *m_h2PmtTProj;
        TH2F *m_h2ProjRuler;

        // TEve
        //TEvePointSet *m_eveCdPmtTSet;
        //TEveQuadSet *m_eveCdPmtTSet;
        TEveBoxSet *m_eveCdPmtTSet;
        TEveBoxSet *m_eveWpPmtTSet;
        TEveBoxSet *m_eveRecSet;

        int m_qtMode;
        int m_eveRecConeMode;
        //TEveTrackList *m_eveTrkList;
        std::vector<TEveElement*> m_eveArrowVec;

        TEveTrackList *m_eveSimOpTrkList;

        double m_aniRecRatio;

        // Palette
        int m_evePaletteMode;
        TEveRGBAPaletteOverlay* m_palT_o;
        TEveRGBAPalette* m_palZ;
        TEveRGBAPalette* m_palT;
        TEveRGBAPalette* m_palR;
        TEveRGBAPalette* m_palC;

        Identifier m_firstPmtId;
        double m_firstPmtT;

        // Display
        JVisTimer* m_timer;

        int m_verb;

        int totalEvent;
};

#endif // JUNO_VIS_TOP_H


