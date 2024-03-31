#ifndef JUNO_VIS_CLIENT_H
#define JUNO_VIS_CLIENT_H

//
//  Juno Visualization Client
//
//  Author: Zhengyun You  2013-12-24
//  Author: Yumei Zhang   2013-12-30
//

#include "TROOT.h"
#include "TGFrame.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class JVisTop;

class TGLayoutHints;
class TGTab;
class TRootEmbeddedCanvas;
class TGCheckButton;
class TGTextButton;
class TGLabel;
class TGGroupFrame;
class TGNumberEntry;
class TGNumberEntryField;
class TGHSlider;
class TCanvas;
class TList;
class TH1F;
class TTimer;

class VisClient : public TGMainFrame
{
    public :

        VisClient(const TGWindow *p,
                  const char *title,
                  unsigned int width,
                  unsigned int height,
                  Option_t *option = "",
                  int argc = 0,
                  char **argv = 0);
        virtual ~VisClient();

        // Argument
        void readArg(int argc, char** argv);
        void anaArg(char* arg);        

        // Flow
        virtual bool initialize();
        virtual void loadDate();
        virtual bool finalize();
        virtual void run();

        // Graphi User Interface
        bool initStyle();
        bool initWidgets();
        void initTimer();
        void initConnections();
        bool createMainFrame();
        bool createCanvas(TGCompositeFrame* tabFrame);
        bool createEve(TGCompositeFrame* tabFrame);
        bool createToolBar();
        void setTitle();

        void setBkgColor();
        void updateBkgColor();

        void updateWidgets();
        void updateCanvas();
        void setCanvasMargin(double left   = 0.05,
                             double right  = 0.02,
                             double bottom = 0.04,
                             double top    = 0.02);

        // Button Operations
        void handleButtons();
        void handleSliders(int slider);
        void drawQ();
        void drawT(double tCut = 9e9);
        void updateTCut();

        void changeEveRecConeMode();
        void changeEvePaletteMode();
       
        void animatePmtT();
        void animatePmtTCommand();
        void setAnimTMin(double tMin);
        void setAnimTMax(double tMax);
        void setAnimTStep(double tStep);
        void setEvtLoop();

        void drawEveHomeView();
        void drawEveMaxView();
        void drawEveInnerView();

        void help();

        // JVisTop
        bool initJVis();

        // Event
        bool runEvt(long iEvt);
        bool curEvt();
        bool preEvt();
        bool nextEvt();
        void updateNumEntryEvt();
        void setAutoPlay();

        //Auto display for DQM
        void autoRunDisplay();
        bool setRootFileName();

        // Close
        bool cleanup();
        void CloseWindow();

        void setVerbosity(int verb) { m_verb = verb; }
        int  getVerbosity() { return m_verb; }

    private :

        long evt;
        const char* m_title;

        //Auto display for DQM
        int m_dqm;
        int totalEvent;
        int playtimes;
        TString m_FileList;
        std::ifstream listFile;
        std::string content;
        std::string listFileName;
        std::string list;
        std::vector<std::string> rootFileName;

        TString m_simFile; 
        TString m_calibFile;
        TString m_recFile;
        TString m_simusFile;
        TString m_geomFile;

        // Widgets and layout
        TList*         m_widgets;
        TGLayoutHints* m_layout;

        // Main frame
        TGCompositeFrame*    m_mainFrame;
        TGTab*               m_mainTab;
 
        // Read drawings
        TRootEmbeddedCanvas* m_eCanvas;
        //TEveManager*         m_eve;      
 
        // Tool bar
        TGCompositeFrame* m_toolBarFrame;

        // Event Contrl group
        TGGroupFrame*     m_groupEvt;
        TGLabel*          m_labelEvt;
        TGCompositeFrame* m_framePreNext;
        TGCheckButton*    m_btnCheck1;
        TGNumberEntry*    m_numEvt;
        TGTextButton*     m_btnPreEvt;
        TGTextButton*     m_btnNextEvt;
        TGTextButton*     m_btnAutoPlay;

        // Draw group
        TGTextButton*     m_btnPmtQ;
        TGTextButton*     m_btnPmtT;

        // Display Option group;
        TGGroupFrame*     m_groupDisp;
        TGTextButton*     m_btnEveRecConeMode;
        TGTextButton*     m_btnEvePaletteMode;

        // Animation group
        TGGroupFrame*     m_groupAnim;
        TGLabel*          m_labelAnim;
        TGCompositeFrame* m_frameTCut;
        TGLabel*          m_labelTCut;
        TGNumberEntry*    m_numTCut;
        TGLabel*          m_labelTCutns;
        TGHSlider*        m_sliderTCut;
        TGCompositeFrame* m_frameTRange;
        TGLabel*          m_labelTMin;
        TGNumberEntryField* m_numAnimTMin;
        TGLabel*          m_labelTMax;
        TGNumberEntryField* m_numAnimTMax;
        TGCompositeFrame* m_frameAnimPlay;
        TGTextButton*     m_btnAnim;
        TGCompositeFrame* m_frameTStep;
        TGLabel*          m_labelTStep;
        TGNumberEntryField* m_numAnimTStep;
        TGLabel*          m_labelTStepns;
        TGCheckButton*    m_btnCheckEvtLoop;

        // View group
        TGGroupFrame*     m_groupView;
        TGTextButton*     m_btnEveHomeView;
        TGTextButton*     m_btnEveMaxView;
        TGTextButton*     m_btnEveInnerView;
	TGTextButton*     m_btnRanTH1test;
        TGTextButton*     m_btnBkgColor;
        TGTextButton*     m_btnHelp;

        // Canvas in main frame
        TCanvas* m_canvas;
        TH1F* m_h1;

        bool    m_autoPlay;

        TTimer* m_PmtTTimer;
        bool    m_PmtTTimerStatus;
        double  m_tCut;
        double  m_animationTMin;
        double  m_animationTMax;
        double  m_animationTStep;
        bool    m_evtLoop;

        int m_bkgColor;
        int m_width;
        int m_height;

        // JVisTop
        JVisTop* m_jvis;
        long m_curEvt;

        int m_verb;
        

    ClassDef(VisClient, 0)  // Vis Client
};

#endif // JUNO_VIS_CLIENT_H
