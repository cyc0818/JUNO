
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/Incident.h"

#include "EvtNavigator/NavBuffer.h"
#include "BufferMemMgr/IDataMemMgr.h"

#include "Event/SimHeader.h"
#include "Event/SimEvent.h"


class TestingAlg: public AlgBase {

public:
    TestingAlg(const std::string& name) : AlgBase(name) {
        m_counter = 0;
    }

    bool initialize() {

        Task* toptask = getRoot();

        task0 = dynamic_cast<Task*>(toptask->find("task0"));
        task1 = dynamic_cast<Task*>(toptask->find("task1"));

        if (!task0) {
            LogError << "Failed to load task0." << std::endl;
            return false;
        }
        if (!task1) {
            LogError << "Failed to load task1." << std::endl;
            return false;
        }


        return true;
    }

    bool execute() {
        Task* task = 0;
        std::string taskname;
        if (m_counter%2==0) {
            task = task0;
            taskname = "task0";
        } else {
            task = task1;
            taskname = "task1";
        }

        JM::EvtNavigator* nav = new JM::EvtNavigator();

        SniperPtr<IDataMemMgr> mMgr(task, "BufferMemMgr");
        mMgr->adopt(nav, "/Event");
        JM::SimHeader* sim_header = new JM::SimHeader;
        JM::SimEvent* sim_event = new JM::SimEvent(m_counter);
        sim_header->setEvent(sim_event);
        nav->addHeader("/Event/Sim"+taskname, sim_header);

        Incident::fire(*getRoot(), taskname);

        ++m_counter;
        return true;
    }

    bool finalize() {
        return true;
    }


private:

    int m_counter;

    Task* task0;
    Task* task1;

};

DECLARE_ALGORITHM(TestingAlg);
