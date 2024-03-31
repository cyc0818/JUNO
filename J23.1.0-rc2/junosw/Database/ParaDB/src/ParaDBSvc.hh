#ifndef ParaDBSvc_hh
#define ParaDBSvc_hh

/*
 * See the details in IParaDBSvc.hh
 * 
 * Tao Lin <lintao AT ihep.ac.cn>
 *
 */

#include <ParaDB/IParaDBSvc.hh>
#include <SniperKernel/SvcBase.h>

class ParaDBSvc: public ParaDB::IParaDBSvc, public SvcBase {
public:

    ParaDBSvc(const std::string& name);
    ~ParaDBSvc();

    bool initialize() override;
    bool finalize() override;

public:
    const std::vector<std::string>& getRepoNames() override;
    std::shared_ptr<ParaDB::IParaRepo> getRepo(const std::string& name) override;

public:
    // User need to check the parameter before use it. 
    bool hasParameter(const std::string& name) override;
    const ParaDB::ParaPayload& getParameter(const std::string& name) override;

private:

    // the order is important, the first one is the top priority
    std::vector<std::string> m_vec_repo_names;
    std::vector<std::string> m_vec_repo_types;

    std::string m_gtag_name; // the para gtag name is set via python script


    std::map<std::string, std::shared_ptr<ParaDB::IParaRepo>> m_paradb_repos;

    // initialized at beginning. it is read only in later stage. 
    // to avoid copying, use the pointer. 
    // that means the repo can't release these payloads. 
    std::map<std::string, const ParaDB::ParaPayload*> m_paradb_payloads;
};

#endif
