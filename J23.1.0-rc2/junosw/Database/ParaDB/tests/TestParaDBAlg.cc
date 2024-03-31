#include <SniperKernel/AlgBase.h>
#include <SniperKernel/AlgFactory.h>

#include <ParaDB/IParaDBSvc.hh>
#include <memory>

struct TestParaDBAlg: public AlgBase {

    TestParaDBAlg(const std::string& name)
        : AlgBase(name), m_paradb_svc(nullptr) {

    }

    ~TestParaDBAlg() {

    }

    bool initialize() {

        SniperPtr<ParaDB::IParaDBSvc> paradb(getParent(), "ParaDBSvc");
        if (paradb.invalid()) {
            LogError << "Failed to get ParaDBSvc. " << std::endl;
            return false;
        }

        m_paradb_svc = paradb.data();

        return true;
    } 

    bool execute() {
        const std::string prop_name = "Material.Acrylic.ABSLENGTH";

        if (not m_paradb_svc->hasParameter(prop_name)) {
            LogError << "Cannot find parameter " << prop_name << std::endl;
            return false;
        }

        auto prop = m_paradb_svc->getParameter(prop_name);
        LogInfo << "Get parameter: " << prop_name << " : " << prop.path() << std::endl;

        return true;
    } 

    bool finalize() {
        return true;
    } 


    ParaDB::IParaDBSvc* m_paradb_svc;

};

DECLARE_ALGORITHM(TestParaDBAlg);
