#ifndef SELECT_EVENT_DATA_H
#define SELECT_EVENT_DATA_H 1

#include "SniperKernel/AlgBase.h"
#include <string>
#include "EvtNavigator/NavBuffer.h"

namespace JM {
    class EvtNavigator;
    class SimHeader;
    class SimEvt;
}

class TTree;
class TFile;
class DummyHeader;
class DummyEvent;

class SelectEventData : public AlgBase {

    public:
        SelectEventData(const std::string& name);
        ~SelectEventData();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        float m_ratio;
        int m_seed;
        int m_limit;
        int m_iEvt;
        int m_mode;
        JM::NavBuffer* m_buf;
        JM::SimHeader* m_header;
        JM::SimEvt* m_event;
        JM::EvtNavigator* m_nav;

        std::string m_fileName;
        TTree* m_headerTree;
        TTree* m_eventTree;
        TTree* m_navTree;
        TFile* m_file;
        DummyHeader* m_plainheader;
        DummyEvent* m_plainevent;
};

#endif
