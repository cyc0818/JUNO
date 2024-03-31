#ifndef ANALY_NAV_BUFFER_H
#define ANALY_NAV_BUFFER_H

#include "EvtNavigator/NavBuffer.h"
#include <memory>
#include "IADSvc/IADEvtBox.h"
#include "RootIOSvc/NavInputStream.h"

class IInputStream;

class AnalyNavBuffer : public JM::NavBuffer
{   
public:
    AnalyNavBuffer(double lowBound, double highBound);
    
    virtual ~AnalyNavBuffer();

    bool loadWindow(const IADEvtBox& iadEvt);

private:
    bool isOverlap(const IADEvtBox& iadEvt);

    void trimDated();
    void fillFresh();
    void fillNext();
    double timeShift(JM::EvtNavigator* nav);

    const double m_floor;
    const double m_ceiling;

    JM::EvtNavigator*  m_beyond;
    std::string m_esdFile;
    uint32_t m_fileNum;
    NavInputStream* m_iStream;

    uint32_t m_curEntry;
};

#endif