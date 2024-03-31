#include <SniperKernel/AlgBase.h>
#include <SniperKernel/AlgFactory.h>

#include <DBISvc/IDBISvc.hh>

struct DummyDBISvcTestAlg: public AlgBase {

    DummyDBISvcTestAlg(const std::string& name)
        : AlgBase(name) {

    }

    ~DummyDBISvcTestAlg() {

    }

    bool initialize() {
        SniperPtr<IDBISvc> dbisvc(getParent(), "DBISvc");

        m_conn_frontier = dbisvc->getConnection("conddb");
        if (!m_conn_frontier) {
            LogError  << "Failed to setup database connection. " << std::endl;
            return false;
        }

        return true;
    }

    bool execute() {

        for (auto result: m_conn_frontier->query<std::string>(stmt_select_date)) {
            auto [current_timestamp] = result;

            LogInfo << "Result: " << current_timestamp << std::endl;
        }

        return true;
    }

    bool finalize() {
        
        return true;
    }

    const std::string stmt_select_date = "select DATE_FORMAT(now(),'%Y-%m-%d_%H:%i')";


    std::shared_ptr<dbi::DBAPI> m_conn_frontier;
};

DECLARE_ALGORITHM(DummyDBISvcTestAlg);
