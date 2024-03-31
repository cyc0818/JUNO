#include "VisClient/VisClient.h"
#include "VisClient/VisHelp.h"
#include "JVisLib/JVisTop.h"
#include "JVisLib/JVisEvtMgr.h"

// ROOT 
#include "TSystem.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TGLayout.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGTab.h"
#include "TGComboBox.h"
#include "TGTextBuffer.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TGSlider.h"
#include "TRootEmbeddedCanvas.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1.h"
#include "TH2.h"
#include "TList.h"
#include "TString.h"
#include "TEveManager.h"
#include "TEveBrowser.h"
#include "TEveViewer.h"
#include "TRootHelpDialog.h"
#include "TGeoManager.h"

// C/C++
#include <iostream>
#include <typeinfo>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

enum VisClientCommandId {
   HC1Id,
   HCheckEvtLoopId,
   HEntryEvtId,
   HPreEvtId,
   HNextEvtId,
   HAutoPlayId,
   HPmtQId,
   HPmtTId,
   HEntryTCutId, 
   HSliderTCutId,
   HEntryAnimTMinId,
   HEntryAnimTMaxId,
   HAnimId,
   HEntryAnimTStepId,
   HEveHomeViewId,
   HEveMaxViewId,
   HEveInnerViewId,
   HEveRecConeModeId,
   HEvePaletteModeId,
   HRanTH1testId,
   HBkgColorId,
   HHelpId
};

VisClient::VisClient(const TGWindow *p,
                     const char *title,
                     unsigned int width,
                     unsigned int height,
                     Option_t*,
                     int argc, 
                     char **argv)
    : TGMainFrame(p, width, height, kHorizontalFrame)
    ,  m_title(title)
    ,  m_simFile(TString(""))
    ,  m_calibFile(TString(""))
    ,  m_recFile(TString(""))
    ,  m_simusFile(TString(""))
    ,  m_geomFile(TString(""))
    ,  m_autoPlay(false)
    ,  m_PmtTTimerStatus(false)
    ,  m_tCut(0.0)
    ,  m_animationTMin(0.0)
    ,  m_animationTMax(150.0)
    ,  m_animationTStep(5.0)
    ,  m_evtLoop(false)
    ,  m_bkgColor(0)
    ,  m_width(width)
    ,  m_height(height)
    ,  m_curEvt(0)
    ,  m_verb(99)
    ,  playtimes(2)
    ,  m_dqm(0)
    ,  m_FileList(TString(""))
{
    setVerbosity(1);
    readArg(argc, argv);
    initialize();

    if (m_dqm)
    {
        while (true)
        {   
            std::cout<<"******start auto play******"<<std::endl;
            autoRunDisplay();
        }
    }

//     setRootFileName();
//     loadDate();
//     nextEvt();
}

VisClient::~VisClient()
{
    cleanup();
}

void VisClient::readArg(int argc, char** argv)
{
    if ( m_verb > 1 ) std::cout << __func__ << " argc " << argc << std::endl;
  
    for (int i = 1; i < argc; i++) {
        //std::cout << argv[i] << std::endl;
        anaArg( argv[i] );
    }
}

void VisClient::anaArg(char* arg)
{
    TString str( arg );
    //std::cout << arg << std::endl;

    if ( str.BeginsWith("--sim=") ) {
        m_simFile = str.Remove(0, 6);
    }
    else if ( str.BeginsWith("--calib=") ) {
        m_calibFile = str.Remove(0, 8);
    }
    else if ( str.BeginsWith("--rec=") ) {
        m_recFile = str.Remove(0, 6);
    }
    else if ( str.BeginsWith("--simus=") ) {
        m_simusFile = str.Remove(0, 8);
    }
    else if ( str.BeginsWith("--geom=") ) {
        m_geomFile = str.Remove(0, 7);
    }
    else if ( str.BeginsWith("--bg=") ) {
        m_bkgColor = str.Remove(0, 5).Atoi();
        if ( m_verb > 0 ) std::cout << str << " : Background Color set to " << m_bkgColor << std::endl;
    }
    else if ( str.BeginsWith("--width=") ) {
        m_width = str.Remove(0, 8).Atoi();
        if ( m_verb > 0 ) std::cout << str << " : Window width set to " << m_width << std::endl;
    }
    else if ( str.BeginsWith("--height=") ) {
        m_height = str.Remove(0, 9).Atoi();
        if ( m_verb > 0 ) std::cout << str << " : Window height set to " << m_height << std::endl;
    }
    else if ( str.BeginsWith("--dqm=") ) {
        m_dqm = str.Remove(0, 6).Atoi();
    }
    else if ( str.BeginsWith("--filelist=") ) {
        m_FileList = str.Remove(0, 11);
    }
    else {
        std::cout << "Do not understand arg " << arg << std::endl;
    }
}

void VisClient::run()
{
    m_canvas->cd();
    for (int i = 0; i < 1000; i++) {
        m_h1->Fill( gRandom->Rndm() );
    }
    m_h1->Draw();

    updateCanvas();
}

bool VisClient::initialize()
{
    if ( m_verb > 0 ) std::cout << "VisClient initialize..." << std::endl;

    initStyle();
    initJVis();

    initWidgets();
    initTimer();
    initConnections();

/*
    TGWindow* eveWindow = m_jvis->getEveWindow();
    eveWindow->ReparentWindow( m_toolBarFrame, 3, 3 );
    std::cout << "eveWindow name " << eveWindow->GetName() << std::endl;
    std::cout << "eveWindow Mapped " << eveWindow->IsMapped() << std::endl;
    //eveWindow->MapWindow();
    //m_layout = new TGLayoutHints( kLHintsExpandX | kLHintsExpandY, 3, 3, 3, 3);
    //m_mainFrame->AddFrame( (TGFrame*)m_jvis->getEveWindow() );
*/

    if (m_jvis) m_jvis->getPmtQProj()->Draw();
    //runEvt(0);

    return true;
}

bool VisClient::finalize()
{
    CloseWindow();

    return true;
}

bool VisClient::initStyle()
{
    gStyle->SetFrameFillColor( m_bkgColor );

    SetWindowName("");
    //SetIconName("");
    //SetIconPixmap( TString("./LogoSerena.png").Data() );
    SetClassHints("SERENA", "Software of Event-display with Root Eve for Neutrio Analysis");
    SetWMPosition(10, 10); // position of the window
   
    return true;
}

bool VisClient::initWidgets()
{
    m_widgets = new TList();
    createMainFrame();
    createToolBar();
    setTitle();
    updateWidgets();

    return true;
}

void VisClient::loadDate()
{   
    std::cout<<"*****loadDate******"<<std::endl;
    m_jvis->setEvtFile( m_simFile, m_calibFile, m_recFile, m_simusFile );
    m_jvis->openRootFile();
    totalEvent = m_jvis->getTotalEvent();
}

bool VisClient::setRootFileName()
{   
    std::cout<<"****setRootFileName*****"<<std::endl;
    bool status = false;
    listFile.open(listFileName,ios::in);
    getline(listFile,list);
    std::stringstream ss(list);
    while (ss>>content){
        rootFileName.push_back(content);
    }
    m_calibFile = rootFileName[0];
    m_recFile = rootFileName[1];
    status = true;
    listFile.close();
    rootFileName.empty();
    return status;    
}

bool VisClient::createMainFrame()
{
    // --- build the main frame ---
    m_mainFrame = new TGCompositeFrame(this, this->GetWidth()-200, this->GetHeight()-10, kVerticalFrame);
    m_layout = new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0);
    m_widgets->Add(m_layout);
    AddFrame(m_mainFrame, m_layout );

    // --- build the main tab ---
    m_mainTab = new TGTab(m_mainFrame, 800, 600);
    //m_mainTab->Connect("Selected(Int_t)", "TestDialog", this, "DoTab(Int_t)");

    TGLayoutHints* layoutNoExpand = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2);
    TGLayoutHints* layoutExpandX  = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 200, 2, 2, 2);
    TGLayoutHints* layoutExpandXY = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2);

    // --- build tab1 ---
    TGCompositeFrame* tab1 = m_mainTab->AddTab("3D Eve Display");
    TGCompositeFrame* eveFrame = new TGCompositeFrame(tab1, 400, 400, kVerticalFrame);

    //TGTextButton* fBtn = new TGTextButton(tab1Frame, "&Button 1", 61);
    //tab1Frame->AddFrame(fBtn, layoutExpandX);
    createEve(eveFrame);
    tab1->AddFrame(eveFrame, layoutExpandXY);

    // --- build tab2 ---
    TGCompositeFrame* tab2 = m_mainTab->AddTab("2D Projection");
    TGCompositeFrame* tab2Frame = new TGCompositeFrame(tab2, 60, 20, kVerticalFrame);

    tab2Frame->AddFrame(new TGTextButton(tab2Frame, "&Test button", 0), layoutNoExpand);
    createCanvas(tab2Frame);
    tab2->AddFrame(tab2Frame, layoutExpandXY);

    // --- build tab3 ---
    TGCompositeFrame* tab3 = m_mainTab->AddTab("Histograms");
    TGCompositeFrame* tab3Frame = new TGCompositeFrame(tab3, 60, 20, kVerticalFrame);

    TGTextButton* fBtn1 = new TGTextButton(tab3Frame, "&Button 1", 61);
    tab3Frame->AddFrame(fBtn1, layoutExpandX);
    TGTextButton* fBtn2 = new TGTextButton(tab3Frame, "B&utton 2", 62);
    tab3Frame->AddFrame(fBtn2, layoutExpandX);
    TGComboBox* fCombo = new TGComboBox(tab3Frame, 88);
    tab3Frame->AddFrame(fCombo, layoutNoExpand);
    tab3->AddFrame(tab3Frame, layoutNoExpand);

    // --- draw the main frame ---
    m_layout = new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5);
    m_widgets->Add(m_layout);
    m_mainFrame->AddFrame(m_mainTab, m_layout);
    m_mainFrame->MapSubwindows();
    m_mainFrame->Resize();
    m_mainFrame->MapWindow();

    MapWindow();
    MapSubwindows(); 

    return true;
}

bool VisClient::createCanvas(TGCompositeFrame* tabFrame)
{
    m_eCanvas = new TRootEmbeddedCanvas(0, tabFrame, m_mainFrame->GetWidth()-10, m_mainFrame->GetHeight()-10);
    m_layout = new TGLayoutHints( kLHintsExpandX | kLHintsExpandY, 3, 3, 3, 3);
    //m_layout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5);
    m_widgets->Add(m_layout);

    tabFrame->AddFrame(m_eCanvas, m_layout);

    m_canvas = m_eCanvas->GetCanvas();
//    int wid = m_eCanvas->GetCanvasWindowId();
//    m_canvas = new TCanvas("jvis",m_eCanvas->GetWidth(), m_eCanvas->GetHeight()-10, wid);
//    m_eCanvas->AdoptCanvas(m_canvas);

    m_canvas->cd();
    setCanvasMargin();

    m_h1 = new TH1F("h1", "h1", 100, 0, 1);

    return true; 
}

bool VisClient::createEve(TGCompositeFrame* tabFrame)
{
    if (!gEve) TEveManager::Create();

    TEveBrowser* eveBrowser = gEve->GetBrowser();
    eveBrowser->UnmapWindow();

    TList* testList = eveBrowser->GetList();
    int size = testList->GetSize();
    for (int i = 0; i < size; i++) {
        TGFrameElement* fE = (TGFrameElement*)(testList->At(i));
        if ( m_verb > 2 ) cout << i << "  " << fE->GetName() << " " << fE->GetTitle() << endl;
        TGCompositeFrame* frame = (TGCompositeFrame*)(fE->fFrame); 
        TGLayoutHints *layout = (TGLayoutHints*)(fE->fLayout);

        // add fVf only, statusBar not added
        if (i == 0) { 
            frame->ReparentWindow( tabFrame );
            tabFrame->AddFrame( (TGCompositeFrame*)frame, layout);
        }

        // get pointer to the bottom tab and resize it to tiny
        if (i == 0) {  // fVf
          //TGCompositeFrame* fTopMenuFrame = (TGCompositeFrame*)(((TGFrameElement*)((TGCompositeFrame*)(fE->fFrame))->GetList()->At(0))->fFrame);
          //fTopMenuFrame->Resize(1, 1);

          TGCompositeFrame* fHf = (TGCompositeFrame*)(((TGFrameElement*)((TGCompositeFrame*)(fE->fFrame))->GetList()->At(2))->fFrame);
          if ( !fHf ) cout << __func__ << " fHf not found" << endl;
          TGCompositeFrame* fV2 = (TGCompositeFrame*)((TGFrameElement*)(fHf->GetList()->At(2)))->fFrame;
          if ( !fV2 ) cout << __func__ << " fV2 not found" << endl;
          TGCompositeFrame* fH2 = (TGCompositeFrame*)((TGFrameElement*)(fV2->GetList()->At(2)))->fFrame;  // bottomTab
          if ( !fH2 ) cout << __func__ << " fH2 not found" << endl;
          fV2->HideFrame( fH2 );
          fH2->Resize(1, 1);
        }
    }
    // The following functions does not work here
    //eveBrowser->GetTabBottom()->Resize(300, 300);
    //eveBrowser->HideBottomTab();
    //tabFrame->HideFrame( eveBrowser->GetTabBottom() );

    return true;
}

bool VisClient::createToolBar()
{
    m_toolBarFrame = new TGCompositeFrame(this, 0, 0, kVerticalFrame);

    m_btnCheck1 = new TGCheckButton(m_toolBarFrame, "&Check Test", HC1Id);
    m_btnCheck1->SetState(kButtonUp);
    m_btnCheck1->SetToolTipText("Pointer Check1");
 
    m_layout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2);
    m_toolBarFrame->AddFrame(m_btnCheck1, m_layout);

    // ----- "Event Contrl" group --------------------------------------------
    m_groupEvt = new TGGroupFrame(m_toolBarFrame, "Event Control ");
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft | kLHintsExpandX, 0, 0, 2, 2);
    m_toolBarFrame->AddFrame(m_groupEvt, m_layout);

    // label
    m_labelEvt = new TGLabel(m_groupEvt, "#");
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 0, 0, 0, 4);
    m_groupEvt->AddFrame(m_labelEvt, m_layout);

    // input of event number
    m_numEvt = new TGNumberEntry(m_groupEvt, 0, 7, HEntryEvtId,  // 7 digits 
        (TGNumberFormat::EStyle) 0,      //0="Int"
        (TGNumberFormat::EAttribute) 1); //1="&Non negative"
    m_numEvt->Resize(80, 20);
    m_numEvt->SetNumber(0);
    m_groupEvt->AddFrame(m_numEvt,    m_layout);

    // pre, next
    m_framePreNext  = new TGCompositeFrame(m_groupEvt, 0, 0, kHorizontalFrame);
    m_groupEvt->AddFrame(m_framePreNext,    m_layout);
    m_btnPreEvt     = new TGTextButton(m_framePreNext, "Pre ", HPreEvtId);
    m_btnNextEvt    = new TGTextButton(m_framePreNext, "Next", HNextEvtId);
    m_layout = new TGLayoutHints(kLHintsTop | kLHintsCenterX, 5, 5, 5, 5);
    m_framePreNext->AddFrame(m_btnPreEvt,     m_layout);
    m_framePreNext->AddFrame(m_btnNextEvt,    m_layout);

    // auto play
    m_btnAutoPlay = new TGTextButton(m_groupEvt, "Auto Play", HAutoPlayId);
    m_btnAutoPlay->SetState( kButtonUp );
    m_btnAutoPlay->SetToolTipText("Pointer to Auto Play next events");
    m_groupEvt->AddFrame(m_btnAutoPlay, m_layout);

    //
    // ----- "Draw " group ---------------------------------------------------
    m_btnPmtQ   = new TGTextButton(m_toolBarFrame, " Charge ", HPmtQId);
    m_btnPmtT   = new TGTextButton(m_toolBarFrame, "   Time   ", HPmtTId);
    m_toolBarFrame->AddFrame(m_btnPmtQ, m_layout);
    m_toolBarFrame->AddFrame(m_btnPmtT, m_layout);

    // ----- "Display Options" group -----------------------------------------
    m_groupDisp = new TGGroupFrame(m_toolBarFrame, "Display Options");
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft | kLHintsExpandX, 0, 0, 2, 2);
    m_toolBarFrame->AddFrame(m_groupDisp, m_layout);

    m_btnEveRecConeMode = new TGTextButton(m_groupDisp, " RecConeMode", HEveRecConeModeId);
    m_groupDisp->AddFrame(m_btnEveRecConeMode,  m_layout);

    m_btnEvePaletteMode = new TGTextButton(m_groupDisp, " Palette", HEvePaletteModeId);
    m_groupDisp->AddFrame(m_btnEvePaletteMode,  m_layout);

    // ----- "Anim" group ----------------------------------------------------
    m_groupAnim = new TGGroupFrame(m_toolBarFrame, "Animation");
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft | kLHintsExpandX, 0, 0, 2, 2);
    m_toolBarFrame->AddFrame(m_groupAnim, m_layout);

    // label
    //m_labelAnim = new TGLabel(m_groupAnim, "Animation");
    //m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 0, 0, 0, 4);
    //m_groupAnim->AddFrame(m_labelAnim, m_layout);

    // tCut
    m_frameTCut = new TGCompositeFrame(m_groupAnim, 0, 0, kHorizontalFrame);
    m_groupAnim->AddFrame(m_frameTCut, m_layout);

    // label tCut
    m_labelTCut = new TGLabel(m_frameTCut, "TCut= ");
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 0, 0, 0, 0);
    m_frameTCut->AddFrame(m_labelTCut, m_layout);

    // numEntry for m_tCut
    m_numTCut = new TGNumberEntry(m_frameTCut, 0, 4, HEntryTCutId,  // 4 digits 
        (TGNumberFormat::EStyle) 0,      //0="Int"
        (TGNumberFormat::EAttribute) 0); //0="Any number"
    m_numTCut->Resize(50, 20);
    m_numTCut->SetNumber( int(m_tCut) );
    m_frameTCut->AddFrame(m_numTCut, m_layout);

    // label tCutns
    m_labelTCutns = new TGLabel(m_frameTCut, " ns");
    m_frameTCut->AddFrame(m_labelTCutns, m_layout);

    // slider for m_tCut
    m_sliderTCut = new TGHSlider(m_groupAnim, 120, kSlider1 | kScaleDownRight, HSliderTCutId);  // kSlider2 | kScaleBoth
    m_sliderTCut->SetRange(int(m_animationTMin), int(m_animationTMax));
    m_sliderTCut->SetPosition( int(m_tCut) );
    m_groupAnim->AddFrame(m_sliderTCut, m_layout);

    // frame tRange
    m_frameTRange = new TGCompositeFrame(m_groupAnim, 0, 0, kHorizontalFrame);
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 0, 0, 4, 4);
    m_groupAnim->AddFrame(m_frameTRange, m_layout);

    // label tMin
    m_labelTMin = new TGLabel(m_frameTRange, " ");
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 0, 0, 2, 2);
    m_frameTRange->AddFrame(m_labelTMin, m_layout);

    // numEntry for m_animationTMin
    m_numAnimTMin = new TGNumberEntryField(m_frameTRange, HEntryAnimTMinId, 1, // 1 decimal
        (TGNumberFormat::EStyle) 1,      //1="Real"
        (TGNumberFormat::EAttribute) 0); //0="Any number"
    m_numAnimTMin->Resize(40, 20);
    m_numAnimTMin->SetNumber( m_animationTMin );
    m_frameTRange->AddFrame(m_numAnimTMin, m_layout);

    // label tMax
    m_labelTMax = new TGLabel(m_frameTRange, " <T< ");
    m_frameTRange->AddFrame(m_labelTMax, m_layout);

    // numEntry for m_animationTMax
    m_numAnimTMax = new TGNumberEntryField(m_frameTRange, HEntryAnimTMaxId, 1,
        (TGNumberFormat::EStyle) 1,      //1="Real"
        (TGNumberFormat::EAttribute) 0); //0="Any number"
    m_numAnimTMax->Resize(40, 20);
    m_numAnimTMax->SetNumber( m_animationTMax );
    m_frameTRange->AddFrame(m_numAnimTMax, m_layout);

    // frame play
    m_frameAnimPlay = new TGCompositeFrame(m_groupAnim, 0, 0, kHorizontalFrame);
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 0, 0, 4, 4);
    m_groupAnim->AddFrame(m_frameAnimPlay, m_layout);

    // Animaton button
    m_btnAnim   = new TGTextButton(m_frameAnimPlay, "Start", HAnimId);
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsCenterX, 4, 0, 0, 0);
    m_frameAnimPlay->AddFrame(m_btnAnim, m_layout);

    // frame TStep
    m_frameTStep = new TGCompositeFrame(m_groupAnim, 0, 0, kHorizontalFrame);
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 0, 0, 4, 4);
    m_groupAnim->AddFrame(m_frameTStep, m_layout);

    // label TStep
    m_labelTStep = new TGLabel(m_frameTStep, "TStep= ");
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 0, 0, 0, 0);
    m_frameTStep->AddFrame(m_labelTStep, m_layout);

    // numEntry for m_animationTStep
    m_numAnimTStep = new TGNumberEntryField(m_frameTStep, HEntryAnimTStepId, 1,
        (TGNumberFormat::EStyle) 1,      //1="Real"
        (TGNumberFormat::EAttribute) 0); //1="Any number"
    m_numAnimTStep->SetNumber( m_animationTStep );
    m_numAnimTStep->Resize(40, 20);
    m_frameTStep->AddFrame(m_numAnimTStep, m_layout);

    // label TStepns 
    m_labelTStepns = new TGLabel(m_frameTStep, " ns");
    m_frameTStep->AddFrame(m_labelTStepns, m_layout);

    // Event Loop
    m_btnCheckEvtLoop = new TGCheckButton(m_groupAnim, "&Event Loop", HCheckEvtLoopId);
    m_btnCheckEvtLoop->SetState( kButtonUp );
    m_btnCheckEvtLoop->SetToolTipText("Pointer to Loop Event in animation");
    m_groupAnim->AddFrame(m_btnCheckEvtLoop, m_layout);

    // ----- "View" group -----
    m_groupView = new TGGroupFrame(m_toolBarFrame, "Eve View");
    m_layout = new TGLayoutHints(kLHintsCenterY | kLHintsLeft | kLHintsExpandX, 0, 0, 2, 2);
    m_toolBarFrame->AddFrame(m_groupView, m_layout);

    // EveView button
    m_btnEveHomeView  = new TGTextButton(m_groupView, " Home ", HEveHomeViewId);
    m_btnEveMaxView   = new TGTextButton(m_groupView, "  Max ", HEveMaxViewId);
    m_btnEveInnerView = new TGTextButton(m_groupView, " Inner", HEveInnerViewId);
    m_groupView->AddFrame(m_btnEveHomeView,  m_layout);
    m_groupView->AddFrame(m_btnEveMaxView,  m_layout);
    m_groupView->AddFrame(m_btnEveInnerView,  m_layout);
    
    // buttons
    m_btnRanTH1test = new TGTextButton(m_toolBarFrame, " Random TH1 test", HRanTH1testId);
    m_btnBkgColor   = new TGTextButton(m_toolBarFrame, "Backgrond Color",  HBkgColorId);
    m_btnHelp = new TGTextButton(m_toolBarFrame, " Help ", HHelpId);

    m_layout = new TGLayoutHints(kLHintsTop | kLHintsCenterX, 2, 2, 2, 2);
    m_toolBarFrame->AddFrame(m_btnRanTH1test,  m_layout);
    m_toolBarFrame->AddFrame(m_btnBkgColor,    m_layout);
    m_toolBarFrame->AddFrame(m_btnHelp,        m_layout);

    m_layout = new TGLayoutHints( kLHintsTop | kLHintsLeft, 5, 5, 5, 5);
    m_widgets->Add(m_layout);
    m_toolBarFrame->Resize(180, 50);
    AddFrame(m_toolBarFrame, m_layout);

    return true;
}

void VisClient::initTimer()
{
    m_PmtTTimer = new TTimer( int(100/1.0) );  // 30ms/frame
}

void VisClient::initConnections()
{
    m_btnCheck1->Connect("Pressed()", "VisClient", this, "handleButtons()");

    m_numEvt->GetNumberEntry()->Connect("ReturnPressed()", "VisClient", this, "handleButtons()");

    m_btnPreEvt->Connect("Clicked()", "VisClient", this, "handleButtons()");
    //m_btnPreEvt->Connect("Pressed()", "VisClient", this, "handleButtons()");
    m_btnNextEvt->Connect("Clicked()", "VisClient", this, "handleButtons()");
    m_btnAutoPlay->Connect("Clicked()", "VisClient", this, "handleButtons()");
    
    m_btnPmtQ->Connect("Clicked()", "VisClient", this, "handleButtons()");
    m_btnPmtT->Connect("Clicked()", "VisClient", this, "handleButtons()");

    m_btnEveRecConeMode->Connect("Clicked()", "VisClient", this, "handleButtons()");
    m_btnEvePaletteMode->Connect("Clicked()", "VisClient", this, "handleButtons()");

    m_numTCut->GetNumberEntry()->Connect("ReturnPressed()", "VisClient", this, "handleButtons()");
    m_sliderTCut->Connect("PositionChanged(Int_t)", "VisClient", this, "handleSliders(Int_t)");
    m_numAnimTMin->Connect("ReturnPressed()", "VisClient", this, "handleButtons()");
    m_numAnimTMax->Connect("ReturnPressed()", "VisClient", this, "handleButtons()");
    m_btnAnim->Connect("Clicked()", "VisClient", this, "handleButtons()");
    m_numAnimTStep->Connect("ReturnPressed()", "VisClient", this, "handleButtons()");
    m_btnCheckEvtLoop->Connect("Released()", "VisClient", this, "handleButtons()");

    m_btnEveHomeView->Connect("Clicked()", "VisClient", this, "handleButtons()");
    m_btnEveMaxView->Connect("Clicked()", "VisClient", this, "handleButtons()");
    m_btnEveInnerView->Connect("Clicked()", "VisClient", this, "handleButtons()");

    m_btnRanTH1test->Connect("Clicked()", "VisClient", this, "handleButtons()");
    m_btnBkgColor->Connect("Clicked()", "VisClient", this, "handleButtons()");
    m_btnHelp->Connect("Clicked()", "VisClient", this, "handleButtons()");

//    Connect("CloseWindow()", "VisClient", this, "CloseWindow()");

    Connect(m_PmtTTimer, "Timeout()", "VisClient", this, "animatePmtTCommand()");
}

void VisClient::setTitle()
{
   // Toplevel widget layout
    TString title(m_title);
    title.Append("@");
    title.Append(gSystem->HostName());
    SetWindowName(title);
    //SetIconName(title);
    //SetIconPixmap( (fPath + TString("/icons/Logo.gif")).Data() );
    SetClassHints(title, "Event Display");

    SetWMPosition(10, 10); // position of the window
    SetMWMHints(kMWMDecorAll, kMWMFuncAll, kMWMInputModeless);
    //Resize(GetDefaultSize());
    //Resize(width, height);
    Resize(m_width, m_height);
}

void VisClient::setBkgColor()
{
    m_bkgColor++;
    if (m_bkgColor > 1) m_bkgColor = 0;
    if ( m_verb > 1 ) cout << "Background color " << m_bkgColor << endl;

    updateBkgColor();
    updateCanvas();
}

void VisClient::updateBkgColor()
{
   TFrame* frame = gPad->GetFrame();
    if (!frame) {

    }
    else {
        frame->SetFillColor( m_bkgColor );
        //std::cout << "frame color " << frame->GetFillColor() << std::endl;
    }

    gPad->Update();
}

void VisClient::help()
{
    TRootHelpDialog * hd = new TRootHelpDialog(m_toolBarFrame, "Help on Event Display", 660, 400);
    hd->AddText( VisHelp::m_authorText );
    hd->AddText( VisHelp::m_helpText1 );
    hd->AddText( VisHelp::m_helpText2 );
    hd->Popup();
}

void VisClient::setCanvasMargin(double left, double right, double bottom, double top)
{
    if ( m_canvas ) {
        m_canvas->SetLeftMargin( left );
        m_canvas->SetRightMargin( right );
        m_canvas->SetBottomMargin( bottom );
        m_canvas->SetTopMargin( top ); 
        if ( m_verb > 1 ) {
            cout << "LeftMargin: "    << m_canvas->GetLeftMargin()
                 << " RightMargin: "  << m_canvas->GetRightMargin()
                 << " BottomMargin: " << m_canvas->GetBottomMargin()
                 << " TopMargin: "    << m_canvas->GetTopMargin() 
                 << endl;
        }    
    }
    else {
        cout << typeid(this).name() << "::" << __FUNCTION__ << endl;     
    }
}

void VisClient::updateCanvas()
{
    m_canvas->Modified();
    m_canvas->Update();
}

void VisClient::updateWidgets()
{
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
}

bool VisClient::initJVis()
{
    if ( m_verb > 0 ) std::cout << "InitJVis..." << std::endl;

    m_jvis = new JVisTop();
    m_jvis->setVerbosity(2);
    if ( m_simFile.Length()   == 0 ) m_simFile   = TString("sample_detsim.root");
    if ( m_calibFile.Length() == 0 ) m_calibFile = TString("sample_calib.root");
    if ( m_recFile.Length()   == 0 ) m_recFile   = TString("sample_rec.root");
    if ( m_simusFile.Length() == 0 ) m_simusFile = TString("sample_detsim_user_op.root");
    if ( m_geomFile.Length()  == 0 ) {
        // if no geom file specified, load the root in Geometry
        if (getenv("JUNO_GEOMETRY_PATH")) {
            m_geomFile = TString(getenv("JUNO_GEOMETRY_PATH"));
            m_geomFile += "/share/JunoGeom.root";
        } else {
            std::cout << __FUNCTION__ << " can't load default geometry from GEOMETRYROOT. " 
                      << std::endl;
            return false;
        }
    }
    // switch the defaut units in TGeoManager
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,22,8)
    // set the default Units to cm
    TGeoManager::LockDefaultUnits(kFALSE);
    TGeoManager::SetDefaultUnits(TGeoManager::EDefaultUnits::kRootUnits);
    TGeoManager::LockDefaultUnits(kTRUE);
#endif


    m_jvis->setEvtFile( m_simFile, m_calibFile, m_recFile, m_simusFile );
    m_jvis->setGeomFile( m_geomFile );
    m_jvis->initialize();

    return true;
}

bool VisClient::runEvt(long iEvt)
{
    bool status = false;

    status = m_jvis->runEvt(iEvt);
    m_curEvt = iEvt;
    updateNumEntryEvt();

    return status;
}

bool VisClient::curEvt()
{
    bool status = runEvt( m_curEvt );

    return status;
}

bool VisClient::preEvt()
{
    m_curEvt--;
    bool status = runEvt( m_curEvt );
    if ( !status ) m_curEvt++;
    updateNumEntryEvt();

    return status; 
}

bool VisClient::nextEvt()
{
    m_curEvt++;
    bool status = runEvt( m_curEvt );
    if ( !status ) m_curEvt--;
    updateNumEntryEvt();

    return status;
}

void VisClient::updateNumEntryEvt()
{
    m_numEvt->SetNumber( m_curEvt );
}

void VisClient::setAutoPlay()
{
    if ( m_verb > 1 ) std::cout << __func__ << " m_autoPlay " << m_autoPlay << std::endl;
    if (m_autoPlay) return;

    m_autoPlay = true;

    bool status = true;
    while (status) {
        status = nextEvt();
    }
    m_autoPlay = false;
}

void VisClient::autoRunDisplay()
{
    bool status = setRootFileName();
    if (status) loadDate();
    for (int i = 0; i < playtimes; i++)
    {
        m_curEvt = (int)(totalEvent*(double)rand()/RAND_MAX);
        std::cout<<"******draw event : "<<m_curEvt<<" ******"<<std::endl;
        status = runEvt( m_curEvt );
        // updateNumEntryEvt();
        sleep(20);
        std::cout<<"*****end of wait , draw next event*****"<<std::endl;
    }
    std::cout<<"*****end of play , close file*****"<<std::endl;
    m_jvis->closeRootFile();
}


void VisClient::changeEveRecConeMode()
{
    m_jvis->changeEveRecConeMode();
}

void VisClient::changeEvePaletteMode()
{
    m_jvis->changeEvePaletteMode();
}

void VisClient::drawQ()
{
    m_jvis->drawPmtQ();
    updateCanvas();
}

void VisClient::drawT(double tCut)
{
    m_jvis->drawPmtT(tCut);
    updateCanvas();
}

void VisClient::setAnimTMin(double tMin)
{
    if ( m_verb > 1 ) std::cout << __func__ << " m_animationTMin " << m_animationTMin << std::endl;
    m_animationTMin = tMin;
    m_numAnimTMin->SetNumber( m_animationTMin );
    m_sliderTCut->SetRange(int(m_animationTMin), int(m_animationTMax));
    m_sliderTCut->SetPosition( int(m_tCut) );
}

void VisClient::setAnimTMax(double tMax)
{
    if ( m_verb > 1 ) std::cout << __func__ << " m_animationTMax " << m_animationTMax << std::endl;
    m_animationTMax = tMax;
    m_numAnimTMax->SetNumber( m_animationTMax );
    m_sliderTCut->SetRange(int(m_animationTMin), int(m_animationTMax));
    m_sliderTCut->SetPosition( int(m_tCut) );    
}

void VisClient::setAnimTStep(double tStep)
{
    if ( m_verb > 0 ) std::cout << __func__ << " m_animationTStep " << m_animationTStep << std::endl;
    m_animationTStep = tStep;
    m_numAnimTStep->SetNumber( m_animationTStep );
}


void VisClient::animatePmtT()
{
    //m_tCut = m_jvis->getFirstHitTimeMin();
    if ( m_verb > 3 ) std::cout << __func__ << " m_tCut " << m_tCut << std::endl;

    if (m_PmtTTimerStatus) {
        m_PmtTTimer->TurnOff();
        m_btnAnim->SetDown(false);
// Need to set EvePmtT color underflow back to "Mark"
    }
    else {
        m_PmtTTimer->TurnOn();
        m_btnAnim->SetDown(true);
    }
    m_PmtTTimerStatus = !m_PmtTTimerStatus;
}

void VisClient::animatePmtTCommand()
{
    if ( m_verb > 3 ) std::cout << "VisClient::animatePmtProjCommand" << std::endl;

    bool outRange = false;
    if (m_tCut > m_animationTMax) { 
        m_tCut = m_animationTMin; 
        outRange = true;
    }

    if (m_tCut < m_animationTMin) {
        m_tCut = m_animationTMax; 
        outRange = true;
    }

    if (outRange && m_evtLoop) {
        bool status = nextEvt();
        if ( !status ) runEvt(0);
    }

    updateTCut();
    m_tCut += m_animationTStep;
}

void VisClient::updateTCut()
{
    if ( m_verb > 3 ) std::cout << __func__ << " updateTCut" << std::endl;

    m_numTCut->SetNumber( int(m_tCut) );
    m_sliderTCut->SetPosition( int(m_tCut) );
    m_jvis->animateEvePmtT(m_tCut);

    if ( m_verb > 3 ) std::cout << "m_tCut " << m_tCut << std::endl;
}

void VisClient::setEvtLoop()
{
    m_evtLoop = m_btnCheckEvtLoop->IsOn();
}

void VisClient::drawEveHomeView()
{
    if ( m_verb > 1 ) std::cout << __func__ << std::endl;
    m_jvis->drawEveHomeView();
}

void VisClient::drawEveMaxView()
{
    if ( m_verb > 1 ) std::cout << __func__ << std::endl;
    m_jvis->drawEveMaxView();
}

void VisClient::drawEveInnerView()
{
    if ( m_verb > 1 ) std::cout << __func__ << " drawEveMaxView" << std::endl;
    m_jvis->drawEveInnerView();
}

bool VisClient::cleanup()
{
    m_widgets->Delete();
    delete m_widgets;

    delete m_btnCheck1;
    delete m_btnCheckEvtLoop;
    delete m_btnPreEvt;
    delete m_btnNextEvt;
    delete m_btnPmtQ;
    delete m_btnPmtT;
    delete m_btnRanTH1test;

    delete m_canvas;
    delete m_h1;
  
    return true;
}

void VisClient::CloseWindow()
{
    TGMainFrame::CloseWindow();
    gApplication->Terminate(0);
}

void VisClient::handleButtons()
{
   TGButton *btn = (TGButton *) gTQSender;
   int id = btn->WidgetId(); 
   
   switch (id) {
      case HC1Id:
         std::cout << "Check 1" << std::endl;
         break;
      case HEntryEvtId:
         std::cout << "Previous Event " << std::endl;
         runEvt( m_numEvt->GetIntNumber() );
         break;
      case HPreEvtId:
         std::cout << "Previous Event " << std::endl;
         preEvt();
         break;
      case HNextEvtId:
         std::cout << "Next Event " << std::endl;
         nextEvt();
         break;
      case HAutoPlayId:
         std::cout << "Auto Play" << std::endl;
         setAutoPlay();
         break;
      case HPmtQId:
         std::cout << "drawQ" << std::endl;
         drawQ();
         break;
      case HPmtTId:
         std::cout << "drawT" << std::endl;
         drawT();
         break;
      case HEveRecConeModeId:
         std::cout << "changeEveRecConeMode" << std::endl;
         changeEveRecConeMode();
         break;
      case HEvePaletteModeId:
         std::cout << "changeEvePaletteMode" << std::endl;
         changeEvePaletteMode();
         break;
      case HEntryTCutId:
         std::cout << "Entry TCut " << std::endl;
         m_tCut = m_numTCut->GetIntNumber();
         updateTCut();
         break;
      case HEntryAnimTMinId:
         std::cout << "Entry AnimTMin " << std::endl;
         m_animationTMin = double(m_numAnimTMin->GetNumber());
         setAnimTMin( m_animationTMin );
         break;
      case HEntryAnimTMaxId:
         std::cout << "Entry AnimTMax " << std::endl;
         m_animationTMax = double(m_numAnimTMax->GetNumber());
         setAnimTMax( m_animationTMax );
         break;
      case HAnimId:
         std::cout << "animatePmtT" << std::endl;
         animatePmtT();
         break;
      case HEntryAnimTStepId:
         std::cout << "Entry AnimTStep " << std::endl;
         m_animationTStep = m_numAnimTStep->GetNumber();
         setAnimTStep( m_animationTStep );
         break;
      case HCheckEvtLoopId:
         std::cout << "Check Evt Loop" << std::endl;
         setEvtLoop();
         break;
      case HEveHomeViewId:
         std::cout << "EveHomeView" << std::endl;
         drawEveHomeView();
         break;
      case HEveMaxViewId:
         std::cout << "EveMaxView" << std::endl;
         drawEveMaxView();
         break;
      case HEveInnerViewId:
         std::cout << "EveInnerView" << std::endl;
         drawEveInnerView();
         break;
      case HRanTH1testId:
         std::cout << "Random TH1 test" << std::endl;
	 run();
	 break;
      case HBkgColorId:
         std::cout << "Set Background color" << std::endl;
         setBkgColor();
         break;
      case HHelpId:
         std::cout << "About Help" << std::endl;
         help();
         break;
      default:
         break;
   }
}

void VisClient::handleSliders(int slider)
{
    TGButton *btn = (TGButton *) gTQSender;
    int id = btn->WidgetId();

    switch (id) {
      case HSliderTCutId:
         //std::cout << "TCut Slider" << std::endl;
         //m_sliderTCut->SetPosition(slider);
         m_tCut = slider;
         updateTCut();
         break;
      default:
         break;
    }
}

