#ifndef JUNO_VIS_TIMER_H
#define JUNO_VIS_TIMER_H

//
//  Juno Visualization Timer with Event Info Text Display
//
//  Author: Zhengyun You  2017-01-28
//

#include "TEveManager.h"
#include "TGLViewer.h"
#include "TGLAnnotation.h"
#include "TDatime.h"
#include "TTimer.h"
#include "TString.h"

class JVisTimer : public TTimer
{
    public:
        JVisTimer(TGLAnnotation* x) : TTimer(1000), m_label(x)
        { }

        JVisTimer();
        ~JVisTimer();

        virtual Bool_t Notify();

        void setText(TString t);

    private:
        TGLAnnotation* m_label;
        TString m_evtInfoText;

    //ClassDef(JVisTimer, 0);  // ClassDef requires LinkDef
};

#endif // JUNO_VIS_TIMER_H
