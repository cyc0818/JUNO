#include <SniperKernel/AlgBase.h>
#include <SniperKernel/AlgFactory.h>

#include <CondDB/ICondDBSvc.h>
#include <CondDB/IRepo.h>
#include <CondDB/ICnvFromPayload.h>

#include <CondObj/CommonCondObj.h>

struct TestingCondDBAlg: public AlgBase {

    TestingCondDBAlg(const std::string& name)
        : AlgBase(name), m_conddb_svc(NULL) {

    }

    ~TestingCondDBAlg() {

    }

    bool initialize() {

        SniperPtr<CondDB::ICondDBSvc> conddb(getParent(), "CondDBSvc");
        if (conddb.invalid()) {
            LogError << "Failed to get CondDBSvc!" << std::endl;
            return false;
        }

        m_conddb_svc = conddb.data();

        // associate condition objects with conddb service
        m_conddb_svc->declCondObj("Calib.Deconv.Para", m_condobj_deconv_para);
        m_conddb_svc->declCondObj("Calib.Deconv.Filter", m_condobj_deconv_filter);

        m_cur = 2020;

        return true;
    }

    bool execute() {
        m_cur += 20;

        // retrieve conditions objects according to current event timestamp.

        m_conddb_svc->setCurrent(m_cur);

        std::cout << "path for this event is "
                  << " Calib.Deconv.Para: " << m_condobj_deconv_para.path() << " "
                  << " Calib.Deconv.Filter: " << m_condobj_deconv_filter.path() << " "
                  << std::endl;
        //m_cdLS_matprop.dump();

        return true;
    }

    bool finalize() {
        return true;
    }


private:
    CondDB::ICondDBSvc* m_conddb_svc;

    // Define the Condition Objects here and associate it with CondDB during initialization
    CondObj::Common::CommonCondObj m_condobj_deconv_para;
    CondObj::Common::CommonCondObj m_condobj_deconv_filter;

    int64_t m_cur;

};

DECLARE_ALGORITHM(TestingCondDBAlg);

