#ifndef MAKE_SAMPLE_H
#define MAKE_SAMPLE_H 1

#include "SniperKernel/AlgBase.h"
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TRandom.h"
#include "EvtNavigator/EvtNavigator.h"
#include "Event/DummyHeader.h"
#include "Event/DummyEvent.h"
#include "EvtNavigator/NavBuffer.h"


class MakeSample : public AlgBase {

    public:
        MakeSample(const std::string& name);
        ~MakeSample();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        int m_iEvt;
        int m_curEvt;
        int m_mode;
        int m_hitNum;
        int m_eventPerFile;

        // For dummy tree
        TRandom m_ran;
        std::string m_fileName;
        TTree* m_headerTree;
        TTree* m_eventTree;
        TFile* m_file;
        DummyHeader* m_header;
        DummyEvent* m_event;
        JM::EvtNavigator* m_nav;
        std::string m_outputFile;

        // For real tree
        JM::NavBuffer* m_buf;
};

#endif
