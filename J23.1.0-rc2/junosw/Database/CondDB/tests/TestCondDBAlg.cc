#include <SniperKernel/AlgBase.h>
#include <SniperKernel/AlgFactory.h>

#include <CondDB/ICondDBSvc.h>
#include <CondDB/IRepo.h>
#include <CondDB/ICnvFromPayload.h>

// CD.LS.MatProp
#include "tests/Testing_CD_LS_MatProp.h"

struct TestCondDBAlg: public AlgBase {

    TestCondDBAlg(const std::string& name)
        : AlgBase(name), m_conddb_svc(NULL) {

    }

    ~TestCondDBAlg() {

    }

    bool initialize() {

        SniperPtr<CondDB::ICondDBSvc> conddb(getParent(), "CondDBSvc");
        if (conddb.invalid()) {
            LogError << "Failed to get CondDBSvc!" << std::endl;
            return false;
        }

        m_conddb_svc = conddb.data();

        // associate condition objects with conddb service
        m_conddb_svc->declCondObj("pmt.cc", m_cdLS_matprop);

        m_cur = 20202312;

        return true;
    }

    bool execute() {
        m_cur += 20;

        // retrieve conditions objects according to current event timestamp.

        m_conddb_svc->setCurrent(m_cur);

        std::cout << "path for this event is" << m_cdLS_matprop.Data << " " << std::endl;
        //m_cdLS_matprop.dump();

        return true;
    }

    bool finalize() {
        return true;
    }


private:
    CondDB::ICondDBSvc* m_conddb_svc;

    CondDB::Testing::CD_LS_MatProp m_cdLS_matprop;

    int64_t m_cur;

};

DECLARE_ALGORITHM(TestCondDBAlg);

