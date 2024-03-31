#include "JVisLib/JVisTop.h"
#include "JVisLib/JVisStatus.h"
#include "JVisLib/JVisGeom.h"
#include "JVisLib/JVisEvtMgr.h"
#include "JVisLib/JVisOpMgr.h"
#include "JVisLib/JVisTimer.h"
#include "Event/SimEvt.h"
#include "Event/CdLpmtCalibEvt.h"
#include "Event/CdVertexRecHeader.h"
#include "Geometry/CdGeom.h"
#include "Geometry/WpGeom.h"
#include "Geometry/GeoUtil.h"
#include "Identifier/CdID.h"
#include "Identifier/WpID.h"
#include "Identifier/IDService.h"

// ROOT
#include "TStyle.h"
#include "TH2.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TMarker.h"
#include "TPad.h"
#include "TFrame.h"
#include "TMath.h"
#include "TEveManager.h"
#include "TEveEventManager.h"
#include "TEveViewer.h"
#include "TEvePointSet.h"
#include "TEveQuadSet.h"
#include "TEveBoxSet.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "TEveArrow.h"
#include "TEveGeoShape.h"
#include "TEveRGBAPaletteOverlay.h"
#include "TGeoSphere.h"
#include "TGeoTube.h"
#include "TGWindow.h"
#include "TGLViewer.h"
#include "TGLClip.h"
#include "TGLAnnotation.h"

#include "TFile.h"
#include "TTree.h"

// C/C++
#include <iostream>
#include <vector>

using namespace std;

JVisTop::JVisTop()
    : m_firstHitTimeMin(9e9)
    , m_firstHitTimeMax(0.0)
    , m_nPEMin(1.0)
    , m_nPEMax(5.0)
    , m_tMin(25.0)
    , m_tMax(125.0)
    , m_tCut(9e99)
    , m_dMin(9e99)
    , m_nPaletteColor(50)
    , m_curDraw(PMTQPROJ)
    , m_verb(1)
    , totalEvent(0)
{

}

JVisTop::~JVisTop()
{

}

bool JVisTop::initialize()
{
    initStyle();
    initStatus();
    initGeom();
    initEvtMgr();
    initHisto();
    initEve();
    // IDService::init();


    return true;
}

bool JVisTop::initStyle()
{
    std::cout << "initStyle" << std::endl;
    gStyle->SetOptTitle( 0 );
    gStyle->SetOptStat( 0 );
    //gStyle->SetFrameFillColor(1);

    return true;
}

bool JVisTop::initStatus()
{
    std::cout << "initStatus" << std::endl;
    m_status = new JVisStatus();
    m_status->init();

    return true;
}

bool JVisTop::initGeom()
{
    if ( m_verb > 0 ) std::cout << "initGeom" << std::endl;
    m_geom = new JVisGeom();
    m_geom->initialize( m_geomFile );

    return true;
}

bool JVisTop::initEvtMgr()
{
    if ( m_verb > 0 ) std::cout << "initEvtMgr" << std::endl;
    m_evtMgr = new JVisEvtMgr();
    m_evtMgr->setVerbosity( m_verb );
    m_evtMgr->initialize( m_simFile, m_calibFile, m_recFile, m_simusFile );
    m_evtLoad = false;

    return true;
}

bool JVisTop::initHisto()
{
    if ( m_verb > 0 ) std::cout << "initHisto" << std::endl;
    bool status = initPmtQProj();
    status = initPmtTProj();

    return status;
}

bool JVisTop::initEve()
{
    if ( m_verb > 0 ) std::cout << "initEve" << std::endl;
    bool status = initEveGeom();
    status = initEvePmt();

    //m_eveTrkList = 0;
    m_eveRecConeMode = 1; // 0: Single Cone; 1: Lines between rec vertex and fired PMTs; 2: Multiple Cones;
    m_qtMode = 1;

    status = initEveRec();

    status = initEveSimOp();  
 
    status = initEveTimer();

    return status;
}

bool JVisTop::initPmtQProj()
{
    if ( m_verb > 1 ) std::cout << __func__ << std::endl;
    m_h2PmtQProj = new TH2F("h2PmtQ", "Pmt Charge 2D Projection", 3600, -180, 180, 1800, -90, 90);

    int    qMarkerStyle = 8;
    double qMarkerSize  = 0.4;

    for (int module = CdID::moduleMin(); module <= CdID::moduleMax(); module++) {
        if ( m_verb > 2 ) {
            if ( module % 1000 == 0 ) std::cout << "module " << module << std::endl;
        }
        unsigned int pmt = 0;
        Identifier id = CdID::id(module, pmt);
        TVector2 proj = m_geom->getCdGeom()->getPmt(id)->getCenterAitoff();
        m_h2PmtQProj->Fill(proj.X(), proj.Y());

        TMarker *qMarker = new TMarker(proj.X(), proj.Y(), qMarkerStyle);
        qMarker->SetMarkerSize(qMarkerSize);
        m_mapPmtQMarker[id] = qMarker;
    }

    m_h2PmtQProj->SetMarkerColor(38);
    m_h2PmtQProj->SetMarkerStyle(1);

    m_h2PmtQProj->GetXaxis()->SetNdivisions(302);
    m_h2PmtQProj->GetYaxis()->SetNdivisions(302);
    m_h2PmtQProj->GetXaxis()->SetLabelOffset(0.010);
    m_h2PmtQProj->GetYaxis()->SetLabelOffset(0.010);

    if ( m_verb > 2 ) std::cout << __func__ << " ends " << std::endl;

    return true;
}

bool JVisTop::initPmtTProj()
{
    if ( m_verb > 1 ) std::cout << __func__ << std::endl;
    m_h2PmtTProj = new TH2F("h2PmtT", "Pmt Time 2D Projection", 3600, -180, 180, 1800, -90, 90);

    int    tMarkerStyle = 8;
    double tMarkerSize  = 0.4;

    for (int module = CdID::moduleMin(); module <= CdID::moduleMax(); module++) {
        unsigned int pmt = 0;
        Identifier id = CdID::id(module, pmt);
        TVector2 proj = m_geom->getCdGeom()->getPmt(id)->getCenterAitoff();
        m_h2PmtTProj->Fill(proj.X(), proj.Y());

        TMarker *tMarker = new TMarker(proj.X(), proj.Y(), tMarkerStyle);
        tMarker->SetMarkerSize(tMarkerSize);
        m_mapPmtTMarker[id] = tMarker;
    }

    m_h2PmtTProj->SetMarkerColor(38);
    m_h2PmtTProj->SetMarkerStyle(1);

    m_h2PmtTProj->GetXaxis()->SetNdivisions(302);
    m_h2PmtTProj->GetYaxis()->SetNdivisions(302);
    m_h2PmtTProj->GetXaxis()->SetLabelOffset(0.010);
    m_h2PmtTProj->GetYaxis()->SetLabelOffset(0.010);

    if ( m_verb > 2 ) std::cout << __func__ << " ends " << std::endl;

    return true;
}

void JVisTop::openRootFile()
{
    m_evtMgr->initialize( m_simFile, m_calibFile, m_recFile, m_simusFile );
    totalEvent = m_evtMgr->getTotalEvent();
}



int JVisTop::getnPEColor(double nPE)
{
    int index = int( float(nPE-m_nPEMin) / (m_nPEMax-m_nPEMin) * m_nPaletteColor );
   
    return index >= 49 ? 100 : 51+index;
}

int JVisTop::getTimeColor(double t)
{
    if (t < m_tMin) t = m_tMin;
    if (t > m_tMax) t = m_tMax;
    int index = int( float(t-m_tMin) / (m_tMax-m_tMin) * m_nPaletteColor );

    return index <= 0 ? 100 : 101-index;
}

JVisGeom* JVisTop::getGeom()
{
    if ( m_geom == 0 ) {
        std::cerr << "JVisTop::getGeom m_geom=0" << std::endl;
    }
    
    return m_geom;
}

JVisEvtMgr* JVisTop::getEvtMgr()
{
    if ( m_evtMgr == 0 ) {
        std::cerr << "JVisTop::getEvtMgr m_evtMgr=0" << std::endl;
    }

    return m_evtMgr;
}

TH2F* JVisTop::getPmtQProj()
{
    if (m_h2PmtQProj == 0) {
        std::cerr << "JVisTop::getPmtQProj m_h2PmtQProj=0" << std::endl;
    }
 
    return m_h2PmtQProj;
}

TH2F* JVisTop::getPmtTProj()
{
    if (m_h2PmtTProj == 0) {
        std::cerr << "JVisTop::getPmtTProj m_h2PmtTProj=0" << std::endl;
    }

    return m_h2PmtTProj;
}

void JVisTop::changeEveRecConeMode()
{
    setEveRecConeMode( (getEveRecConeMode()+1)%2 ); // nEveRecConeMode = 2;  
    drawEve();
    std::cout << "EveRecConeMode = " << getEveRecConeMode() << std::endl;
}

void JVisTop::changeEvePaletteMode()
{
    setEvePaletteMode( (getEvePaletteMode()+1)%2 );
    drawEve();
    std::cout << "EvePaletteMode = " << getEvePaletteMode() << std::endl;
}


bool JVisTop::runEvt(int iEvt)
{
    bool status = true;

    status = m_evtMgr->readEvt(iEvt);
    if ( !status ) return false;

    m_evtLoad = true;
    status = drawCurrentPad();
    if ( !status ) return false;

    return status;
}

bool JVisTop::drawCurrentPad()
{
    if (m_curDraw == PMTQPROJ) {
        drawPmtQ();
    }
    else if (m_curDraw == PMTTPROJ) {
        drawPmtT( m_tCut );
    }

    return true;    
}

bool JVisTop::drawPmtQ()
{
    bool status = false;

    status = drawPmtQProj();
    setqtMode(0);  // 0 for Q
    status = drawEve();

    return status;
}

bool JVisTop::drawPmtT(double tCut)
{
    bool status = false;

    m_tCut = tCut;
    status = drawPmtTProj(tCut);
    setqtMode(1);  // 1 for T
    status = drawEve();

    return status;
}

bool JVisTop::drawPmtQProj()
{
    if ( !m_h2PmtQProj ) return false;
    m_h2PmtQProj->Draw();

    if ( !m_evtLoad ) {
        m_evtMgr->readEvt(0);
        m_evtLoad = true;
    }

    if ( m_evtMgr->hasCalib() ) {
        auto ce = m_evtMgr->getCalibEvent();
        if ( 0 == ce ) {
            std::cout << "getCalibEvent error" << std::endl;
            return false;
        }

        int nCalibPMT = 0; 
        const auto& calibPMTCol = ce->calibPMTCol();    
        for (auto it = calibPMTCol.begin();
             it != calibPMTCol.end(); ++it) {
            if ( *it ) {
                auto cp = *it;

                if ( m_verb > 2 ) {
                    if ( nCalibPMT < 10 ) {
                        std::cout << __func__ << Identifier(cp->pmtId()) 
                            << "  " << cp->nPE() << "  " << cp->firstHitTime()
                            << std::endl;
                    }
                }

                if ( !CdID::isCd( Identifier(cp->pmtId()) ) ) {
                    // not a CD PMT
                    continue;
                }

                TMarker *qMarker = m_mapPmtQMarker[ Identifier(cp->pmtId()) ];
                int qColor = getnPEColor( cp->nPE() );
                qMarker->SetMarkerColor( qColor );
                qMarker->SetMarkerSize( 0.7 );
                qMarker->Draw("s"); 
            }
            nCalibPMT++;
        }
    }

    if ( m_evtMgr->hasSim() ) {
        drawTrkProj();
    }

    if ( m_verb > 1 ) cout << __func__ << " PMTQPROJ" << endl;
    m_curDraw = PMTQPROJ;
    gPad->Update();
    if ( m_verb > 1 ) cout << __func__ << " gPad Update" << endl;
 
    return true;
}

bool JVisTop::drawPmtTProj(double tCut)
{
    if ( !m_h2PmtTProj ) return false;
    m_h2PmtTProj->Draw();

    if ( !m_evtLoad ) {
        m_evtMgr->readEvt(0);
        m_evtLoad = true;
    }
    
    if ( m_evtMgr->hasCalib() ) {
        auto ce = m_evtMgr->getCalibEvent();
        if ( 0 == ce ) {
            std::cout << "getCalibEvent error" << std::endl;
            return false;
        }

        m_firstHitTimeMin = 9e9;
        m_firstHitTimeMax = 0.0;
        int nCalibPMT = 0;
        const auto& calibPMTCol = ce->calibPMTCol();
        for (auto it = calibPMTCol.begin();
             it != calibPMTCol.end(); ++it) {
            if ( *it ) {
                auto cp = *it;

                if ( m_verb > 2 ) {
                    if ( nCalibPMT < 10 ) {
                        std::cout << __func__ << Identifier(cp->pmtId())
                            << "  " << cp->nPE() << "  " << cp->firstHitTime()
                            << std::endl;
                    }
                }

                if ( !CdID::isCd( Identifier(cp->pmtId()) ) ) {
                    // not a CD PMT
                    continue;
                }

                double firstT = cp->firstHitTime();
                if (firstT < m_firstHitTimeMin) {
                    m_firstHitTimeMin = firstT;
                }
                if (firstT > m_firstHitTimeMax) {
                    m_firstHitTimeMax = firstT;
                }

                if (firstT < tCut) {
                    TMarker *tMarker = m_mapPmtTMarker[ Identifier(cp->pmtId()) ];
                    int tColor = getTimeColor( firstT );
                    tMarker->SetMarkerColor( tColor );
                    tMarker->SetMarkerSize( 0.7 );
                    tMarker->Draw("s");
                }
            }
            nCalibPMT++;
        }
    }

    if ( m_evtMgr->hasSim() ) {
        drawTrkProj();
    }

    if ( m_verb > 1 ) cout << __func__ << " PMTTPROJ" << endl;
    m_curDraw = PMTTPROJ;
    gPad->Update();
    if ( m_verb > 1 ) cout << __func__ << " gPad Update" << endl;

    return true;
}

bool JVisTop::drawTrkProj()
{
    if ( m_verb > 1 ) std::cout << __func__ << std::endl;
    if ( !m_evtMgr->hasSim() ) return false;

    auto se = m_evtMgr->getSimEvent();
    if ( 0 == se ) {
        std::cout << "getSimEvent error" << std::endl;
        return false;
    }

    const auto& stc = se->getTracksVec();
    const int nst = stc.size();
    if ( m_verb > 1 ) std::cout << "SimTrack size =" << nst << std::endl;
    TVector3 simVtx(0,0,0);
    double simEdep = 0.0;
    double simEnergy = 0.0;
    for (int ist = 0; ist < nst; ist++ ) {
        auto st = stc[ist];
        if ( m_verb > 1 ) {
            std::cout << "se pdg " << st->getPDGID() << " mass " << st->getInitMass() << " edep " << st->getEdep() << std::endl
                << "   gen  pos (" << st->getInitX() << ", " << st->getInitY() << ", " << st->getInitZ() << ") time " << st->getInitT() << std::endl
                << "   exit pos (" << st->getExitX() << ", " << st->getExitY() << ", " << st->getExitZ() << ") time " << st->getExitT() << std::endl;
        }

        TVector3 center(st->getInitX(), st->getInitY(), st->getInitZ());
        double org_theta = center.Theta()*TMath::RadToDeg();
        double org_phi   = center.Phi()*TMath::RadToDeg();
        org_theta = 90.0 - org_theta;

        double project_x, project_y;
        GeoUtil::projectAitoff2xy(org_phi, org_theta, project_x, project_y);
    
        TMarker *genMarker = new TMarker(project_x, project_y, 2);
        genMarker->SetMarkerSize(2);
        genMarker->Draw("s");

        center = TVector3(st->getExitX(), st->getExitY(), st->getExitZ());
        org_theta = center.Theta()*TMath::RadToDeg();
        org_phi   = center.Phi()*TMath::RadToDeg();
        org_theta = 90.0 - org_theta;

        GeoUtil::projectAitoff2xy(org_phi, org_theta, project_x, project_y);

        TMarker *edepMarker = new TMarker(project_x, project_y, 4);
        edepMarker->SetMarkerSize(2);
        edepMarker->Draw("s");

        simVtx += TVector3(st->getInitX(), st->getInitY(), st->getInitZ());
        simEdep += st->getEdep();
        simEnergy += sqrt(st->getInitMass()*st->getInitMass()+st->getInitPx()*st->getInitPx()+st->getInitPy()*st->getInitPy()+st->getInitPz()*st->getInitPz());
    }
    simVtx *= 1.0/nst;
    if ( m_verb > 1 ) {
        std::cout << "SimEvent vtx pos (" << simVtx.x() << ", " << simVtx.y() << ", " << simVtx.z() << ")" << std::endl;
        std::cout << "SimEvent simEdep " << simEdep << " energy " << simEnergy << std::endl;
    }

    gPad->Update();

    return true;
}

bool JVisTop::initEvePmt()
{
    if ( m_verb > 0 ) std::cout << __func__ << std::endl;

    if (!gEve) TEveManager::Create();
/*
    m_eveCdPmtTSet = new TEvePointSet();
    m_eveCdPmtTSet->SetOwnIds(kTRUE);

    for (unsigned int module = CdID::moduleMin(); module <= CdID::moduleMax(); module++) {
        unsigned int pmt = 0;
        Identifier id = CdID::id(module, pmt);
        TVector3 center = m_geom->getCdGeom()->getPmt(id)->getCenter();
        m_eveCdPmtTSet->SetNextPoint(center.X(), center.Y(), center.Z());
        m_eveCdPmtTSet->SetPointId(new TNamed(Form("%d", module), ""));
        if (module % 1000 == 0) cout << module << " " << center.X() << endl;
    }

    gEve->AddElement( m_eveCdPmtTSet );
    m_eveCdPmtTSet->SetMarkerStyle(4); // antialiased circle
    m_eveCdPmtTSet->SetMarkerColor(5);
    m_eveCdPmtTSet->SetMarkerSize(0.8);
*/
    gStyle->SetPalette(1, 0);
    m_evePaletteMode = 1;
    m_palZ = new TEveRGBAPalette(-20000, 20000);
    m_palT = new TEveRGBAPalette(-200, 0);   // this set the slider range in GUI
    m_palR = new TEveRGBAPalette(-10000, 0); // this set the slider range in GUI
    m_palC = new TEveRGBAPalette(-100, 100); // constant to 0
    m_palT_o = new TEveRGBAPaletteOverlay(m_palT, 0.65, 0.04, 0.33, 0.03);

    TEveVector dir, pos;
    double height, rad20inch; //theta, phi

    height = 1.0; //500.0;
    rad20inch = m_geom->get20inchPmtRadius(); // 20inch diameter

    // Add Cd Pmt
    m_eveCdPmtTSet = new TEveBoxSet("CD_PMT");
    m_eveCdPmtTSet->SetOwnIds(kTRUE);
    m_eveCdPmtTSet->SetPalette(m_palZ);
    m_eveCdPmtTSet->Reset(TEveBoxSet::kBT_Cone, kFALSE, 64);

    for (unsigned int module = CdID::moduleMin(); module <= (unsigned int)CdID::moduleMax(); module++) {
        unsigned int pmt = 0;
        Identifier id = CdID::id(module, pmt);
        //Identifier id = CdID::id(module, pmt);

        TVector3 center = m_geom->getCdGeom()->getPmt(id)->getCenter();
        TVector3 axisDir = m_geom->getCdGeom()->getPmt(id)->getAxisDir();

        pos.Set(center.X(), center.Y(), center.Z());
        dir.Set(axisDir.X(), axisDir.Y(), axisDir.Z());

        dir *= height;

        //if (center.X() > 0) {
        if (true) {
            double rad = rad20inch;
            if ( CdID::is3inch(id) ) rad *= 3.0/20;
            //cout << id << " " << " is20inch " <<  CdID::is20inch(id) << " is3inch "  << CdID::is3inch(id) << " rad " << rad << endl;
            m_eveCdPmtTSet->AddCone(pos, dir, rad);

            int id_print = static_cast<int>(id);
            // auto s = Form("PMT id%#x ", id_my);
            // std::cout << s << std::endl;
            m_eveCdPmtTSet->DigitId(new TNamed(Form("CopyNo=%d, id=%#010x \n(%3.1f, %3.1f, %3.1f)", module, id_print, center.X(), center.Y(), center.Z()), ""));
            m_eveCdPmtTSet->DigitValue((int)center.Z());
        }

        if ( m_verb > 2 ) {
            if ( module % 1000 == 0 ) cout << module << " " << center.X() << endl;
        }
    }

    m_eveCdPmtTSet->RefitPlex();
    if ( m_verb > 1 ) std::cout << "Cd Digit Size : " << m_eveCdPmtTSet->GetPlex()->Size() << std::endl;

    if (false) {
        for (unsigned int module = CdID::moduleMin(); module <= 4; module++) {
            TNamed* digitId = (TNamed*)m_eveCdPmtTSet->GetId( module );
            cout << digitId->GetName() << endl;
        }
    }

    m_eveCdPmtTSet->SetPickable(1);
    m_eveCdPmtTSet->SetAlwaysSecSelect(1);

    // Add Wp Pmt
    m_eveWpPmtTSet = new TEveBoxSet("WP_PMT");
    m_eveWpPmtTSet->SetOwnIds(kTRUE);
    m_eveWpPmtTSet->SetPalette(m_palC);
    m_eveWpPmtTSet->Reset(TEveBoxSet::kBT_Cone, kFALSE, 64);

    for (unsigned int module = WpID::moduleMin(); module <= (unsigned int)WpID::moduleMax(); module++) {
        unsigned int pmt = 0;
        Identifier id = WpID::id(module, pmt);
        TVector3 center = m_geom->getWpGeom()->getPmt(id)->getCenter();
        TVector3 axisDir = m_geom->getWpGeom()->getPmt(id)->getAxisDir();

        pos.Set(center.X(), center.Y(), center.Z());
        dir.Set(axisDir.X(), axisDir.Y(), axisDir.Z());
        dir *= height;

        if (true) {
            double rad = rad20inch;
            m_eveWpPmtTSet->AddCone(pos, dir, rad);
            m_eveWpPmtTSet->DigitId(new TNamed(Form("ID%d \n(%3.1f, %3.1f, %3.1f)", module, center.X(), center.Y(), center.Z()), ""));
            m_eveWpPmtTSet->DigitValue(-99);
        }

        if ( m_verb > 2 ) {
            if ( module % 1000 == 0 ) cout << module << " " << center.X() << endl;
        }
    }

    gEve->AddElement( m_eveCdPmtTSet );
    gEve->AddElement( m_eveWpPmtTSet );
    m_eveWpPmtTSet->SetRnrState(false);  // set to invisible by default

    gEve->GetDefaultViewer()->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraPerspXOY); // kCameraOrthoZOY
    gEve->Redraw3D(false, true);

    TGLViewer *v = gEve->GetDefaultGLViewer();
//    v->GetClipSet()->SetClipType( TGLClip::kClipBox );

    v->AddOverlayElement(m_palT_o);

    // Add axes at origin (kAxesOrigin)
    double referencePos[3];
    int axesType;
    bool axesDepthTest, referenceOn;
    v->GetGuideState(axesType, axesDepthTest, referenceOn, referencePos);
    v->SetGuideState(TGLUtil::kAxesOrigin, axesDepthTest, referenceOn, referencePos);
    v->RefreshPadEditor(v); 

    v->UpdateScene();  // this will rest current camera
//    v->ResetCurrentCamera();
/*    v->CurrentCamera().Zoom(+120, false, false);
    v->CurrentCamera().RotateRad(-30.0*TMath::DegToRad(), -225.0*TMath::DegToRad());
    v->CurrentCamera().Zoom(-10000, false, true);  //(factor, 'ctrl' pressed, 'shift' pressed)
    v->CurrentCamera().Dolly(-1000, false, true);
    v->CurrentCamera().Dolly(-1000, false, true);
*/

//    TEveViewer* viewer = gEve->GetDefaultViewer();
//    viewer->Redraw();

    TGWindow* eveMainWin = gEve->GetMainWindow();
    if ( m_verb > 1 ) std::cout << __func__ << " MainWin name: " << eveMainWin->GetName() << std::endl;
    eveMainWin->SetWindowName("Change Name");

    return true;
}

bool JVisTop::initEveRec()
{
    if ( m_verb > 0 ) std::cout << __func__ << std::endl;

    if (!gEve) TEveManager::Create();

    m_firstPmtId = CdID::id(0, 0);

    m_eveRecSet = new TEveBoxSet("REC_CONE");
    m_eveRecSet->SetOwnIds(kTRUE);

    m_eveRecSet->SetPickable(1);         // can select and show the outline of shape
    m_eveRecSet->SetAlwaysSecSelect(1);  // select as a whole or individually, pop out a window to show the info when cursor stops on it

    gEve->AddElement( m_eveRecSet );

    return true;
}

bool JVisTop::initEveSimOp()
{
    if ( m_verb > 0 ) std::cout << __func__ << std::endl;

    if (!gEve) TEveManager::Create();

    m_eveSimOpTrkList = new TEveTrackList("OP_PATH");
    m_eveSimOpTrkList->SetPickable(1);         // can select and show the outline of shape

    gEve->AddElement( m_eveSimOpTrkList );

    return true;

}

bool JVisTop::initEveGeom()
{
    if ( m_verb > 0 ) std::cout << __func__ << std::endl;

    if (!gEve) TEveManager::Create();

    TEveGeoShape* ls = new TEveGeoShape("LiquidScintillator");
    ls->SetShape(new TGeoSphere(0, 17700.0));
    ls->SetMainColor(kWhite);
    ls->SetMainTransparency(90);
    gEve->AddElement(ls); 
    ls->SetRnrState(false);  // set to invisible by default
    
    TEveGeoShape* eq = new TEveGeoShape("Equator");
    eq->SetShape(new TGeoTube(20000.0, 22000.0, 1.0));
    eq->SetMainColor(kYellow);
    eq->SetMainTransparency(80);
    gEve->AddElement(eq);

    TEveGeoShape* wp = new TEveGeoShape("WaterPool");
    wp->SetShape(new TGeoTube(0.0, 22000.0, 24000.0));
    wp->SetMainColor(kCyan);
    wp->SetMainTransparency(70);
    gEve->AddElement(wp);
    wp->SetRnrState(true);  // set to visible by default

    return true;
}

bool JVisTop::initEveTimer()
{
    // add overlay text
    TGLViewer* v = gEve->GetDefaultGLViewer();
    TDatime time;
    TGLAnnotation* ann = new TGLAnnotation(v, time.AsString(), 0.01, 0.98);
    ann->SetTextSize(0.04);// % of window diagonal

    // set timer to update text every second
    m_timer = new JVisTimer(ann);
    m_timer->SetTime(1000);
    m_timer->Reset();
    m_timer->TurnOn();

    return true;
}

bool JVisTop::drawEve()
{
    if ( m_verb > 1 ) {
        std::cout << __func__ << m_qtMode << std::endl;
    }

    if ( !m_evtLoad ) {
        std::cout << __func__ << " m_evtLoad " << m_evtLoad << std::endl;
        std::cout << "No events loaded, load an event first..." << std::endl;
        return false;
    }

    bool status = false;

    if ( m_evtMgr->hasCalib() ) { status = drawEvePmt();   if ( !status ) return status; }
    if ( m_evtMgr->hasRec() )   { status = drawEveRec();   if ( !status ) return status; }
    if ( m_evtMgr->hasSim() )   { status = drawEveTrk();   if ( !status ) return status; }
    if ( m_evtMgr->hasSimus() ) { status = drawEveSimOp(); if ( !status ) return status; }

    status = drawEveEvtInfo(); if ( !status ) return status; 

    gEve->Redraw3D(false, true);
    TEveViewer* viewer = gEve->GetDefaultViewer();
    viewer->Redraw();

    return true;
}

bool JVisTop::drawEvePmt()
{
    if ( m_verb > 1 ) {
        std::cout << __func__ << m_qtMode << std::endl;
    }

    //m_eveCdPmtTSet->DestroyElements(); 
    m_eveCdPmtTSet->Reset(TEveBoxSet::kBT_Cone, kFALSE, 64);
    if ( m_verb > 2 ) std::cout << "After destroy, Digit Size : " << m_eveCdPmtTSet->GetPlex()->Size() << std::endl;

    if ( !m_evtMgr->hasCalib() ) return false;

    auto ce = m_evtMgr->getCalibEvent();
    if ( 0 == ce ) {
        std::cout << "getCalibEvent error" << std::endl;
        return false;
    }

    m_aniRecRatio = 1.0;
    m_firstHitTimeMin = 9e9;
    m_firstHitTimeMax = 0.0;
    //TEveBoxSet::BCone_t* digit = 0;
    //TNamed*  digitId = 0;
    TEveVector dir, pos;
    double height, rad20inch; // theta, phi

    height = 1.0; //500.0;
    rad20inch = m_geom->get20inchPmtRadius(); // 20inch diameter

    if ( m_verb > 1 ) cout << __func__ << " reading calibPMTCol" << endl;
    int nCalibPMT = 0;
    const auto& calibPMTCol = ce->calibPMTCol();
    for (auto it = calibPMTCol.begin();
         it != calibPMTCol.end(); ++it) {
        if ( *it ) {
            auto cp = *it;

            Identifier id = Identifier(cp->pmtId());
            if ( !CdID::isCd( id ) ) {
                // not a CD PMT
                continue;
            }

            int module = CdID::module(id);
            //digit = (TEveBoxSet::BCone_t*)m_eveCdPmtTSet->GetDigit( module );
            //TEveVector pos = digit->fPos;
            //digitId = (TNamed*)m_eveCdPmtTSet->GetId( module ); // the info when cursor focused
            double nPE = cp->nPE();
            double firstT = cp->firstHitTime();

            TVector3 center = m_geom->getCdGeom()->getPmt(id)->getCenter();
            TVector3 axisDir = m_geom->getCdGeom()->getPmt(id)->getAxisDir();
            
            pos.Set(center.X(), center.Y(), center.Z());
            dir.Set(axisDir.X(), axisDir.Y(), axisDir.Z());
            dir *= height;
            //if (center.X() > 0) {
            if (true) {
                double rad = rad20inch;
                if ( CdID::is3inch(id) ) rad *= 3.0/20;
                //cout << id << " " << " is3inch " << CdID::is3inch(id) << " rad " << rad << endl;
                m_eveCdPmtTSet->AddCone(pos, dir, rad);
                int id_print = static_cast<int>(id);
                m_eveCdPmtTSet->DigitId(new TNamed(Form("CopyNo=%d, id=%#010x, \nPos (%3.1f, %3.1f, %3.1f)mm \nnPE=%3.1f, firstHitTime=%3.1fns", module, id_print, center.X(), center.Y(), center.Z(), nPE, firstT), ""));
                if (m_qtMode == 0) {
                    m_eveCdPmtTSet->DigitValue( int(nPE) );
                }
                else if (m_qtMode == 1) {
                    m_eveCdPmtTSet->DigitValue( int(-firstT) );
                }
                else {
                    std::cout << __func__ << " wrong m_qtMode " << m_qtMode << std::endl;
                    return false;
                }
            }

            if ( m_verb > 2 ) {
                if ( nCalibPMT < 10 ) {
                    cout << "Id " << CdID::module(id) << " pos (" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")"
                        << "  " << cp->nPE() << "  " << cp->firstHitTime()
                        << endl;
                }
            }

            if (firstT < m_firstHitTimeMin) {
                m_firstHitTimeMin = firstT;
                m_firstPmtId = id;
                m_firstPmtT = firstT;
            }
            if (firstT > m_firstHitTimeMax) {
                m_firstHitTimeMax = firstT;
            }

/*          if (firstT < tCut) {
                TMarker *tMarker = m_mapPmtTMarker[ Identifier(cp->pmtId()) ];
                int tColor = getTimeColor( firstT );
                tMarker->SetMarkerColor( tColor );
                tMarker->SetMarkerSize( 0.7 );
                tMarker->Draw("s");
            }
*/
        }
        nCalibPMT++;
    }

    if (m_qtMode == 0) {
        m_palT->SetMinMax( int(m_nPEMin), int(m_nPEMax)+1);
        if ( m_verb > 1 ) cout << "m_palT " << m_palT->GetMinVal() << ", " << m_palT->GetMaxVal() << endl;
    }
    else if (m_qtMode == 1) {
        m_palT->SetMinMax(-int(m_firstHitTimeMin)-99, -int(m_firstHitTimeMin)+1);
        if ( m_verb > 1 ) cout << "m_palT " << m_palT->GetMinVal() << ", " << m_palT->GetMaxVal() << endl;
    }
    else {
        std::cout << __func__ << " wrong m_qtMode " << m_qtMode << std::endl;
        return false;
    }
  
    m_eveCdPmtTSet->SetPalette(m_palT);
    m_palT->SetUnderflowAction(TEveRGBAPalette::kLA_Mark);
    m_palT->SetUnderColor(kViolet+2);

    if (m_evePaletteMode != 0) {
        m_palT_o->SetState(TGLOverlayElement::kActive);
    }
    else {
        m_palT_o->SetState(TGLOverlayElement::kInvisible);
    }

    return true;
}

bool JVisTop::animateEvePmtT(double tCut)
{
    //std::cout << __func__ << " tCut " << tCut << std::endl;
    if (!m_evtLoad) return false;

    TEveViewer* viewer = gEve->GetDefaultViewer();
    if ( !viewer ) {
        cout << "no DefaultViewer " << endl;
        return false;
    }

    if ( m_evtMgr->hasRec() ) {
        auto rh = m_evtMgr->getRecEvent();
        if ( 0 == rh ) {
            std::cout << __func__ << "getRecEvent error" << std::endl;
            return false;
        }
        // get event
        auto rec_vertex = rh->getVertex(0);
        TVector3 recVtx = TVector3(rec_vertex->x(), rec_vertex->y(), rec_vertex->z());
        //double recEnergy = rh->energy();

        // update Rec Cone
        if (m_eveRecConeMode == 0) {
           int nCones = m_eveRecSet->GetPlex()->Size();
           double ratio = tCut/fabs(m_palT->GetMaxVal());
           if ( m_verb > 3 ) cout << "nCones=" << nCones << " ratio=" << ratio << endl;
           if (ratio > 0.0 && ratio <= 1.0) {
                for (int i = 0; i < nCones; i++) {
                    TEveBoxSet::BCone_t* cone = (TEveBoxSet::BCone_t*)(m_eveRecSet->GetDigit(i));
                    cone->fDir *= (ratio/m_aniRecRatio);
                    cone->fR *= (ratio/m_aniRecRatio);
                }
                m_aniRecRatio = ratio;
            }
        }
        else if (m_eveRecConeMode == 2) {
            int palRMin = int(-tCut);
            if (palRMin > m_palR->GetMaxVal()) palRMin = m_palR->GetMaxVal();
            if (palRMin < m_palT->GetMaxVal()) palRMin = m_palT->GetMaxVal();
            m_palR->SetMinMax(palRMin, m_palR->GetMaxVal());
            m_palR->SetUnderflowAction(TEveRGBAPalette::kLA_Cut);
            //m_palR->SetUnderColor(kBlack);
        }
        else if (m_eveRecConeMode == 1) {
            auto ce = m_evtMgr->getCalibEvent();
            if ( 0 == ce ) {
                std::cout << __func__ << " getCalibEvent error" << std::endl;
                return false;
            }

            if ( m_verb > 2 ) cout << __func__ << " m_dMin=" << m_dMin << endl;   
            TVector3 center, axisDir;
            TEveVector dir;

            if ( m_verb > 2 ) cout << __func__ << " reading calibPMTCol" << endl;
            int nCalibPMT = 0;
            const auto& calibPMTCol = ce->calibPMTCol();
            for (auto it = calibPMTCol.begin();
                it != calibPMTCol.end(); ++it) {
                if ( *it ) {
                    auto cp = *it;
                    Identifier id = Identifier(cp->pmtId());
                    double firstT = cp->firstHitTime();
                    
                    center = m_geom->getCdGeom()->getPmt(id)->getCenter();
                    axisDir = center - recVtx;
                    double speed = axisDir.Mag()/firstT;   // flying speed is different for different PMTs, or flying path is longer than distance
                    double distFly = speed*tCut;
                    if (distFly < axisDir.Mag()) axisDir.SetMag(distFly); //  distFly <= distance(PMT-recVtx),

                    dir.Set(axisDir.X(), axisDir.Y(), axisDir.Z());
                    if ( m_verb > 3 ) { cout << "axisDir " << endl; axisDir.Print(); }
                    TEveBoxSet::BCone_t* cone = (TEveBoxSet::BCone_t*)(m_eveRecSet->GetDigit(nCalibPMT));
                    if ( m_verb > 3 ) cout << "dir (" << cone->fDir.fX << ", " << cone->fDir.fY << ", " <<cone->fDir.fZ << ") " << endl;
                    cone->fDir = dir; 
                }
                nCalibPMT++;
            }
        }
        else {
            cout << __func__ << " wrong m_eveRecConeMode " << m_eveRecConeMode << endl;
        }
    }

    if ( m_evtMgr->hasCalib() ) {
        // update Pmt T
        int palTMin = int(-tCut);
        if (palTMin > m_palT->GetMaxVal()) palTMin = m_palT->GetMaxVal();
        m_palT->SetMin(palTMin);
        //m_palT->SetUnderflowAction(TEveRGBAPalette::kLA_Cut);
        Color_t bkgColor = viewer->GetGLViewer()->RnrCtx().ColorSet().Background().GetColorIndex();
        m_palT->SetUnderColor(bkgColor);
        m_palT->SetUnderflowAction(TEveRGBAPalette::kLA_Cut);
    }

    viewer->Redraw();

    return true;
}

bool JVisTop::drawEveTrk()
{
    if ( m_verb > 1 ) std::cout << __func__ << std::endl;

    if (!gEve) TEveManager::Create();
/*
    if (!m_eveTrkList)
    {
        m_eveTrkList = new TEveTrackList("Tracks");
        m_eveTrkList->SetMainColor(6);
        m_eveTrkList->SetMarkerColor(kYellow);
        m_eveTrkList->SetMarkerStyle(4);
        m_eveTrkList->SetMarkerSize(0.5);

        gEve->AddElement( m_eveTrkList );
    }
    m_eveTrkList->DestroyElements();
    cout << "Add m_eveTrkList" << endl;

    TEveTrackPropagator* trkProp = m_eveTrkList->GetPropagator();
    trkProp->SetMagField(0.0, 0.0, 0.0);
    trkProp->SetMaxR(20000);
    trkProp->SetMaxZ(20000);

    JM::SimEvent* se = m_evtMgr->getSimEvent();
    if ( 0 == se ) {
        std::cout << "getSimEvent error" << std::endl;
    }
    
    const JM::SimParticleHistory *history = se->particleHistory();
    const std::vector<JM::SimTrack*>& stc = history->primaryTracks();
    std::cout << "se primary =" << stc.size() << std::endl;
    TVector3 simVtx(0,0,0);
    double simEdep = 0.0;
    double simEnergy = 0.0;
    std::vector<JM::SimTrack*>::const_iterator stit = stc.begin();
    while (stit!=stc.end()) {
        JM::SimTrack* st = *stit;
        std::cout << "se pdg " << st->pdgid() << " mass " << st->mass() << " edep " << st->edepEnergy() << std::endl;
        std::cout << "   gen  pos (" << st->genX() << ", " << st->genY() << ", " << st->genZ() << ") time " << st->genT() << std::endl;
        std::cout << "   edep pos (" << st->getExitX() << ", " << st->getExitY() << ", " << st->getExitZ() << ") " << std::endl;
        std::cout << "   momentum (" << st->px() << ", " << st->py() << ", " << st->pz() << ")" << std::endl;

        TEveMCTrack *mcTrk = new TEveMCTrack();
        mcTrk->SetProductionVertex(st->genX(), st->genY(), st->genZ(), st->genT());
        double e = sqrt(st->px()*st->px()+st->py()*st->py()+st->pz()*st->pz()+st->mass()*st->mass());
        mcTrk->SetMomentum(st->px(), st->py(), st->pz(), e);
        mcTrk->fDecayed = true;
        mcTrk->fVDecay.Set(st->getExitX(), st->getExitY(), st->getExitZ());

        TEveTrack* track = new TEveTrack(mcTrk, trkProp);
        track->SetName(Form("PDG %d", st->pdgid()));
        track->SetAttLineAttMarker(m_eveTrkList);
        track->MakeTrack();

        m_eveTrkList->AddElement(track);

        stit++;
    }
*/
/*
    if (!m_eveArrowList)
    {
        m_eveArrowList = new TList();
        m_eveArrowList->SetName("Tracks");
    }
*/
    if ( m_verb > 1 ) {
        cout << "children " << gEve->GetCurrentEvent()->NumChildren() << endl;
        cout << "m_eveArrowVec size " << m_eveArrowVec.size() << endl;
    }
    for (int i = m_eveArrowVec.size()-1; i >= 0; i--) {    
        TEveElement* element = m_eveArrowVec[i];
        m_eveArrowVec.pop_back();
        //gEve->PreDeleteElement(element);
        //gEve->RemoveElement(element, (TEveElement*)(gEve->GetCurrentEvent()));
        //gEve->GetCurrentEvent()->RemoveElement(element);
        element->Destroy();
        //gEve->GetCurrentEvent()->DestroyElements();
    }
    if ( m_verb > 1 ) {
        cout << "m_eveArrowVec size " << m_eveArrowVec.size() << endl;
    }

    TVector3 recVtx(0,0,0);
    double recEnergy = 0.0;
    JM::CdVertexRecEvt* rh = 0;
    if ( m_evtMgr->hasRec() ) {
        rh = m_evtMgr->getRecEvent();
        if ( 0 == rh ) {
            std::cout << __func__ << "getRecEvent error" << std::endl;
            return false;
        }
        // get event
        auto rec_vertex = rh->getVertex(0);
        recVtx = TVector3(rec_vertex->x(), rec_vertex->y(), rec_vertex->z());
        recEnergy = rec_vertex->energy();
    }

    double trkR = 5.0;
    double trkRRatio = 1.0;
    double trkL = 1.0;
    TVector3 genPos(0,0,0);
    TVector3 exitPos(0,0,0);

    JM::SimEvt* se = 0;
    if ( m_evtMgr->hasSim() ) {
        se = m_evtMgr->getSimEvent();
        if ( 0 == se ) {
            std::cout << "getSimEvent error" << std::endl;
        }

        const auto& stc = se->getTracksVec();
        const int nst = stc.size();
        if ( m_verb > 1 ) std::cout << "SimTrack size =" << nst << std::endl;
        TVector3 simVtx(0,0,0);
        double simEnergy = 0.0;
        for (int ist = 0; ist < nst; ist++ ) {
            auto st = stc[ist];
            if ( m_verb > 1 ) {
                std::cout << "se pdg " << st->getPDGID() << " mass " << st->getInitMass() << " edep " << st->getEdep() << std::endl
                    << "   gen  pos (" << st->getInitX() << ", " << st->getInitY() << ", " << st->getInitZ() << ") time " << st->getInitT() << std::endl
                    << "   exit pos (" << st->getExitX() << ", " << st->getExitY() << ", " << st->getExitZ() << ") time " << st->getExitT() << std::endl;
            }

            genPos.SetXYZ (st->getInitX(), st->getInitY(), st->getInitZ());
            exitPos.SetXYZ(st->getExitX(), st->getExitY(), st->getExitZ());       
            trkL = (exitPos-genPos).Mag();
            trkRRatio = trkR/trkL;
            simEnergy += sqrt(st->getInitMass()*st->getInitMass()+st->getInitPx()*st->getInitPx()+st->getInitPy()*st->getInitPy()+st->getInitPz()*st->getInitPz());

            TEveArrow* trk = new TEveArrow(
                st->getExitX()-st->getInitX(),
                st->getExitY()-st->getInitY(),
                st->getExitZ()-st->getInitZ(),
                st->getInitX(), st->getInitY(), st->getInitZ());
            trk->SetName("Sim Track");
            trk->SetTitle(Form(
                "MC Truth: Trk pdg=%d \nmass=%5.3fMeV/c^2 energy=%5.3fMeV\ngen pos=(%5.1f, %5.1f, %5.1f)mm\np=(%5.3f, %5.3f, %5.3f)MeV/c \nEdep=%5.3fMeV \nEdep pos=(%5.1f, %5.1f, %5.1f)mm \nEdep length=%3.1fmm",
                st->getPDGID(), st->getInitMass(), simEnergy,
                st->getInitX(), st->getInitY(), st->getInitZ(),
                st->getInitPx(), st->getInitPy(), st->getInitPz(),
                st->getEdep(), 
                st->getExitX(), st->getExitY(), st->getExitZ(),
                (exitPos-genPos).Mag()));
            trk->SetMainColor(kRed);
            trk->SetTubeR(trkRRatio);
            trk->SetConeR(2.0*trkRRatio);
            trk->SetConeL(trkL < 100 ? 0.3 : 30.0/trkL);
            trk->SetPickable(kTRUE);

            genPos.SetXYZ(st->getExitX(), st->getExitY(), st->getExitZ());

            if ( m_evtMgr->hasRec() ) {
                TEveArrow* srDif = new TEveArrow(
                    recVtx.X()-st->getExitX(), recVtx.Y()-st->getExitY(), recVtx.Z()-st->getExitZ(), 
                    st->getExitX(), st->getExitY(), st->getExitZ());
                srDif->SetName("Sim Rec Difference");
                srDif->SetTitle(Form(
                    "Rec Diff: Trk pdg=%d \nEdep True=%5.3fMeV Rec=%5.3fMeV Diff=%5.2f \nEdep pos=(%5.1f, %5.1f, %5.1f)mm \nrecVtx pos=(%5.1f, %5.1f, %5.1f)mm \nVtx Rec Diff=%3.1fmm",
                    st->getPDGID(), st->getEdep(), recEnergy, (recEnergy-st->getEdep())/st->getEdep()*100.0,
                    exitPos.X(), exitPos.Y(), exitPos.Z(),
                    recVtx.X(), recVtx.Y(), recVtx.Z(),
                    float((recVtx-exitPos).Mag())
                ));
                srDif->SetMainColor(kCyan);

                trkRRatio = trkR/(recVtx-exitPos).Mag();
                srDif->SetTubeR(trkRRatio);
                srDif->SetConeR(trkRRatio);
                srDif->SetConeL(0.0);
                srDif->SetPickable(kTRUE);

                gEve->AddElement(srDif);
                m_eveArrowVec.push_back(srDif);
            }

            if ( m_verb > 1 ) std::cout << __func__ << " add SimTrk to Eve " << std::endl;       
            gEve->AddElement(trk);
            m_eveArrowVec.push_back(trk);
        }
    }

    return true;
}

bool JVisTop::drawEveRec()
{
    if ( m_verb > 1 ) std::cout << __func__ << std::endl;

    if ( !m_evtMgr->hasRec() ) return false;

    if ( !gEve ) TEveManager::Create();

    if ( !m_evtLoad ) {
        std::cout << __func__ << " m_evtLoad " << m_evtLoad << std::endl;
        std::cout << "No event loaded, click Pre/Next or Charge/Time to display an event first..." << std::endl; 

        return false;
    }

    m_eveRecSet->Reset(TEveBoxSet::kBT_Cone, kFALSE, 64);

    auto rh = m_evtMgr->getRecEvent();
    if ( 0 == rh ) {
        std::cout << __func__ << "getRecEvent error" << std::endl;
        return false;
    }
    // get event
    auto rec_vertex = rh->getVertex(0);

    TVector3 recVtx(rec_vertex->x(), rec_vertex->y(), rec_vertex->z());

    TVector3 center = m_geom->getCdGeom()->getPmt(m_firstPmtId)->getCenter();
    TVector3 recEp(rec_vertex->px(), rec_vertex->py(), rec_vertex->pz());
    //cout << "center " << endl; center.Print();
    //cout << "recVtx " << endl; recVtx.Print();
    //cout << "recEp  " << endl; recEp.Print();
    TVector3 axisDir = center - recVtx;
    //axisDir = recEp.Unit();
    axisDir.SetMag(1.0);
    double peSum = rec_vertex->peSum();
    double energy = rec_vertex->energy();
    double eprec = rec_vertex->eprec();

    //TEveBoxSet::BCone_t* digit = 0;
    //TNamed*  digitId = 0;
    TEveVector dir, pos;
    double height, rad; // theta, phi

    height = 0.9*(center - recVtx).Mag(); //500.0;
    rad = 0.3*height;

    pos.Set(recVtx.X(), recVtx.Y(), recVtx.Z());
    dir.Set(axisDir.X(), axisDir.Y(), axisDir.Z());
    dir *= height;
    if ( m_verb > 1 ) {
        cout << "height " << height << " rad " << rad << endl;
        cout << "pos (" << pos.fX << ", " << pos.fY << ", " << pos.fZ << ")" << endl;
        cout << "dir (" << dir.fX << ", " << dir.fY << ", " << dir.fZ << ")" << endl;
    }

    if (m_eveRecConeMode == 0) {
        m_eveRecSet->AddCone(pos, dir, rad);
        drawEveRecInfo(peSum, recVtx, axisDir, energy, eprec);
        m_eveRecSet->UseSingleColor();
        m_eveRecSet->SetMainColor(kOrange-9); //0
        m_eveRecSet->SetMainTransparency(0);
    }
    else if (m_eveRecConeMode == 2) {
        int nSeg = 100;
        for (int i = 0; i < nSeg; i++) {
            TEveVector curDir = dir;
            curDir *= 1.0*(i+1)/nSeg;
            double curRad = 0.3*curDir.Mag()-i;
 
            m_eveRecSet->AddCone(pos, curDir, curRad);
            drawEveRecInfo(peSum, recVtx, axisDir, energy, eprec);
            double value = m_firstHitTimeMin*(i+1)/nSeg; // height*(i+1)/nSeg
            //cout << value << endl;
            m_eveRecSet->DigitValue( int(-value) );
        }
        if ( m_verb > 1 ) cout << "height " << height << endl;
        m_palR->SetMinMax( int(-height), 0 );
        m_palR->SetMinMax( int(-m_firstHitTimeMin), 0 );
        m_eveRecSet->SetPalette(m_palR);
        m_eveRecSet->RefitPlex();
    }
    else if (m_eveRecConeMode == 1) {
        auto ce = m_evtMgr->getCalibEvent();
        if ( 0 == ce ) {
            std::cout << __func__ << " getCalibEvent error" << std::endl;
            return false;
        }

        if ( m_verb > 1 ) cout << __func__ << " reading calibPMTCol" << endl;
        int nCalibPMT = 0;
        const auto& calibPMTCol = ce->calibPMTCol();
        for (auto it = calibPMTCol.begin();
            it != calibPMTCol.end(); ++it) {
            if ( *it ) {
                auto cp = *it;
                Identifier id = Identifier(cp->pmtId());
  
                center = m_geom->getCdGeom()->getPmt(id)->getCenter();
                axisDir = center - recVtx;
                if (axisDir.Mag() < m_dMin) m_dMin = axisDir.Mag();

                pos.Set(recVtx.X(), recVtx.Y(), recVtx.Z());
                dir.Set(axisDir.X(), axisDir.Y(), axisDir.Z());

                m_eveRecSet->AddCone(pos, dir, 10.0);

                double dist = axisDir.Mag();
                double firstT = cp->firstHitTime();
                double speed = dist/firstT;   // flying speed is different for different PMTs, or flying path is longer than distance

                drawEveRecInfo(peSum, recVtx, axisDir, energy, eprec, dist, firstT, speed);
            }
            nCalibPMT++;
        }

        m_eveRecSet->UseSingleColor();
        m_eveRecSet->SetMainColor(kOrange-9); //0
    }
    else {
        cout << __func__ << " wrong m_eveRecConeMode " << m_eveRecConeMode << endl;
    }

/*
    gEve->Redraw3D(false, true);
    TEveViewer* viewer = gEve->GetDefaultViewer();
    viewer->Redraw();
*/
    //drawTrkProj();

    //m_curDraw = PMTTPROJ;
    //gPad->Update();

    return true;
}

bool JVisTop::drawEveRecInfo(double peSum, TVector3 &recVtx, TVector3 &axisDir, double energy, double eprec, double dist, double t, double speed)
{
    if ( !m_evtMgr->hasRec() ) return false;

    if ( !m_eveRecSet ) {
        cout << __func__ << " m_eveRecSet not found " << endl;
    }

    TString info(Form("Rec PESum=%3.1f \nVtx (%3.1f, %3.1f, %3.1f)mm \nDir (%5.3f, %5.3f, %5.3f)\nenergy %5.3fMeV eprec %5.3fMeV",
                      peSum, recVtx.X(), recVtx.Y(), recVtx.Z(), axisDir.X(), axisDir.Y(), axisDir.Z(), energy, eprec));
    if ( dist > 0 ) {
        info += TString(Form("\n\nrecVtx->this PMT \ndist=%3.1fmm t=%3.1fns \nequivalent speed=%3.1fX10^6m/s", dist, t, speed));
    }

    m_eveRecSet->DigitId(new TNamed(info, TString("")));

    return true;
}

bool JVisTop::drawEveSimOp()
{
    if ( m_verb > 1 ) std::cout << __func__ << std::endl;

    if ( !m_evtMgr->hasSimus() ) {
        std::cout << __func__ << " no Simus to draw" << std::endl;
        return true;
    }

    if ( !gEve ) TEveManager::Create();

    JVisOpMgr *opMgr = m_evtMgr->getOpMgr();
    opMgr->setTrkList( m_eveSimOpTrkList );

    if ( 0 == m_eveSimOpTrkList ) {
        std::cout << __func__ << " m_eveSimOpTrkList does not exist" << std::endl;
        return false;
    }

    opMgr->clearTrks();
    opMgr->buildTrks();

    return true;
}

bool JVisTop::drawEveEvtInfo()
{
    if ( m_verb > 1 ) std::cout << __func__ << std::endl;

    if ( m_verb > 2 ) std::cout << m_evtMgr->getEvtInfoText() << std::endl; 
    m_timer->setText( m_evtMgr->getEvtInfoText() ); 

    return true;
}

TEveManager* JVisTop::getEve()
{
  return gEve;
}

TGWindow* JVisTop::getEveWindow()
{
  return gEve->GetMainWindow();
}

bool JVisTop::drawEveHomeView()
{
    if ( !gEve ) {
        std::cout << __func__ << " gEve not created" << std::endl;
        return false;
    }

    TGLViewer *v = gEve->GetDefaultGLViewer();
    if ( !v ) {
        std::cout << __func__ << " the default viewer not found" << std::endl;
        return false;
    }

    v->UpdateScene();

    return true;
}

bool JVisTop::drawEveMaxView()
{
    if ( !gEve ) {
        std::cout << __func__ << " gEve not created" << std::endl;
        return false;
    }

    TGLViewer *v = gEve->GetDefaultGLViewer();
    if ( !v ) {
        std::cout << __func__ << " the default viewer not found" << std::endl;
        return false;
    }

    v->ResetCurrentCamera();
    v->CurrentCamera().Zoom(+120, false, false);
    v->CurrentCamera().RotateRad(-30.0*TMath::DegToRad(), -225.0*TMath::DegToRad());
    v->CurrentCamera().Zoom(-10000, false, true);  //(factor, 'ctrl' pressed, 'shift' pressed)
    v->CurrentCamera().Dolly(40, false, true);
//    v->CurrentCamera().Dolly(-1000, false, true);
    v->DoDraw();

    return true;
}

bool JVisTop::drawEveInnerView()
{
    if ( !gEve ) {
        std::cout << __func__ << " gEve not created" << std::endl;
        return false;
    }

    TGLViewer *v = gEve->GetDefaultGLViewer();
    if ( !v ) {
        std::cout << __func__ << " the default viewer not found" << std::endl;
        return false;
    }

    v->ResetCurrentCamera();
    v->CurrentCamera().Zoom(+120, false, false);
    v->CurrentCamera().RotateRad(-30.0*TMath::DegToRad(), -225.0*TMath::DegToRad());
    v->CurrentCamera().Zoom(-10000, false, true);  //(factor, 'ctrl' pressed, 'shift' pressed)
    v->CurrentCamera().Dolly(43, false, true);
//    v->CurrentCamera().Dolly(-1000, false, true);
    v->DoDraw();

    return true;
}

void JVisTop::closeRootFile()
{
    std::cout<<"JVisTop::closeRootFile"<<std::endl;
    m_evtMgr->closeRootFile();
}

