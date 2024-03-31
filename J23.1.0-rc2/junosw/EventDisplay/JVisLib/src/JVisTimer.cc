#include "JVisLib/JVisTimer.h"
#include <iostream>

JVisTimer::JVisTimer()
{

}

JVisTimer::~JVisTimer()
{

}

Bool_t JVisTimer::Notify()
{
    // stop timer
    TurnOff();

    // so some action here
    TDatime d;
    TString text = d.AsString();
    text += "\n";
    text += m_evtInfoText;
    //std::cout << text << std::endl;
   
    m_label->SetText(text);
    gEve->GetDefaultGLViewer()->RequestDraw();

    // start timer
    SetTime(1000);
    Reset();
    TurnOn();
    return true;
}

void JVisTimer::setText(TString t)
{
    m_evtInfoText = t;
}

