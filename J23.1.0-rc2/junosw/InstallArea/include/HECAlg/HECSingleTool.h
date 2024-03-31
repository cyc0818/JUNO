#ifndef HECSINGLETOOL_H
#define HECSINGLETOOL_H
#include "HECAlg/IClassTool.h"
#include "SniperKernel/ToolBase.h"

class HECSingleTool: public ToolBase, public IClassTool{
    public:
        HECSingleTool(const std::string&);
        ~HECSingleTool();

        //inherite from ToolBase
        virtual bool initialize();
        virtual bool finalize();

        //inherite from IClassTool
        virtual bool isVetoed(JM::NavBuffer::Iterator);
        virtual bool findCorrelation(JM::NavBuffer::Iterator, std::vector<JM::OecEvt*>&);//Find the events correlated to this event
        virtual bool calClassifyQuantity(const std::vector<JM::OecEvt*>&);//Input:the events correlated in time
        virtual bool userFunc(JM::EvtNavigator*, const EventPair&);

    private:
        //Help functions
        bool isCDMuon(int tag){
            return ((tag | ~i_muonTag)==0xFFFFFFFF);
        }

        JM::NavBuffer *m_buf;
        TTimeStamp m_latestMuon;
        int i_muonTag;
        int i_singleTag_ME;
        int i_singleTag_LE;
        int i_singleTag_HE;

        //-------------parameters-------------
        int i_preMuonVetoTime;
        int i_postMuonVetoTime;
};
#endif
