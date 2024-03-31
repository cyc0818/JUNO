#ifndef HECMUONTOOL_H
#define HECMUONTOOL_H
#include "HECAlg/IClassTool.h"
#include "SniperKernel/ToolBase.h"

class HECMuonTool: public ToolBase, public IClassTool{
    public:
        HECMuonTool(const std::string&);
        ~HECMuonTool();

        //inherite from ToolBase
        virtual bool initialize();
        virtual bool finalize();

        //inherite from IClassTool
        virtual bool isVetoed(JM::NavBuffer::Iterator);//True:the event is not vetoed    False:the event is vetoed
        virtual bool findCorrelation(JM::NavBuffer::Iterator, std::vector<JM::OecEvt*>&);//Find the events correlated to this event
        virtual bool calClassifyQuantity(const std::vector<JM::OecEvt*>&);//Input:the events correlated in time
        virtual bool userFunc(JM::EvtNavigator*, const EventPair&);

    private:
        JM::NavBuffer *m_buf;
        TTimeStamp m_latestMuon;

        //-------------parameters-------------
        int i_maxN;//The maximum number of correlated events
        int i_dT;//The maximum time which defines two events to be correlated in time, unit:ns
        double d_MuonWindow; // time difference between CDtrigger and WPtrigger/TTtrigger

};
#endif
