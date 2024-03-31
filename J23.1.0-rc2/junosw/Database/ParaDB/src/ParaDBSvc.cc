#include "ParaDBSvc.hh"

// REPO
#include "DBISvc/IDBISvc.hh"
#include "DBParaRepo.hh"

// SNiPER
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SharedElemFactory.h"

DECLARE_SERVICE(ParaDBSvc);
DECLARE_CAN_BE_SHARED(ParaDBSvc);

ParaDBSvc::ParaDBSvc(const std::string& name)
    : SvcBase(name) {
    declProp("RepoNames", m_vec_repo_names);
    declProp("RepoTypes", m_vec_repo_types);

    declProp("ParaGTag", m_gtag_name);
}

ParaDBSvc::~ParaDBSvc() {

}

bool
ParaDBSvc::initialize() {
    if (m_vec_repo_names.size() == 0) {
        LogError << "No repo name specified. " << std::endl;
        return false;
    }

    if (m_vec_repo_names.size() != m_vec_repo_types.size()) {
        LogError << "the size of repo name and size of type is mismatch. " << std::endl;
        return false;
    }

    for (size_t i = 0; i < m_vec_repo_names.size(); ++i) {
        auto& n = m_vec_repo_names[i];
        auto& t = m_vec_repo_types[i];

        std::shared_ptr<ParaDB::IParaRepo> repo;

        if (t == "DBI") {
            SniperPtr<IDBISvc> dbisvc(getParent(), "DBISvc");
            if (dbisvc.invalid()) {
                LogError << "Failed to find DBISvc. " << std::endl;
                return false;
            }

            std::string client = "paradb";

            auto db_conn = dbisvc->getConnection(client);
            if (!db_conn) {
                LogError << "Failed to setup database connection for paradb" << std::endl;
                return false;
            }

            std::shared_ptr<ParaDB::DBParaRepo> repo_(new ParaDB::DBParaRepo(n, db_conn));
            repo_->confDatabaseName(dbisvc->getSchemaProperty(client, "database"));
            repo_->confParaGTagTable(dbisvc->getSchemaProperty(client, "ParaGTagTable"));
            repo_->confParaGTag2TagTable(dbisvc->getSchemaProperty(client, "ParaGTag2TagTable"));
            repo_->confParaTag2PayloadTable(dbisvc->getSchemaProperty(client, "ParaTag2PayloadTable"));

            repo = repo_;

            LogInfo << "Setup DBI based ParaDBSvc. " << std::endl;

        } else {
            LogError << "Unsupported repo type " << t << std::endl;
            return false;
        }

        m_paradb_repos[n] = repo;

    }

    // after initialization of paradb repos, try to load the data. 
    for (auto repo_name: m_vec_repo_names) {
        auto repo = m_paradb_repos[repo_name];

        // get the corresponding gt
        auto gtags = repo->findGlobalTags(m_gtag_name);
        if (not gtags.size()) {
            LogWarn << "Can't find global tag " << m_gtag_name << " in repo " << repo_name << std::endl;
            continue;
        }

        // use the first one
        auto& gtag = gtags[0];

        auto& tags = repo->findTags(gtag);
        if (not tags.size()) {
            LogWarn << "Can't find tags for global tag " 
                    << m_gtag_name << " in repo " << repo_name << std::endl;
            continue;
        }

        for (auto& tag: tags) {
            auto& payloads = repo->findPayloads(tag);
            if (not payloads.size()) {
                LogWarn << "Can't find any payloads for tag " << tag.name()
                        << " (global tag: " << m_gtag_name << ") "
                        << " in repo " << repo_name << std::endl;
            }
            // now, merge the data into the cache

            for (auto& p: payloads) {
                auto it = m_paradb_payloads.find(p.property());
                // if not exist, register it. otherwise, skip. 
                if (it == m_paradb_payloads.end()) {
                    m_paradb_payloads[p.property()] = &p;
                } else {
                    LogWarn << "- Found duplicated payload with same name: "
                            << " loading: " << p.property() << " " << p.version()
                            << "; existing: " << it->first << " " << it->second->version()
                            << std::endl;
                }
            }
        }
    }

    LogInfo << "=========================================" << std::endl;
    LogInfo << "Dumping all the parameters in ParaDBSvc: " << std::endl;
    LogInfo << "=========================================" << std::endl;

    for (auto [k,p]: m_paradb_payloads) {
        LogInfo << "- " << k << ": " << p->path() << std::endl;
    }

    LogInfo << "=========================================" << std::endl;
    LogInfo << " Total number of parameters: " << m_paradb_payloads.size() << std::endl;
    LogInfo << "=========================================" << std::endl;

    return true;
}

bool
ParaDBSvc::finalize() {
    return true;
}

const std::vector<std::string>&
ParaDBSvc::getRepoNames() {
    return m_vec_repo_names;
}

std::shared_ptr<ParaDB::IParaRepo>
ParaDBSvc::getRepo(const std::string& name) {
    auto it = m_paradb_repos.find(name);
    if (it != m_paradb_repos.end()) {
        return it->second;
    }
    return nullptr;
}

bool
ParaDBSvc::hasParameter(const std::string& name) {

    auto it = m_paradb_payloads.find(name);
    if (it != m_paradb_payloads.end()) {
        return true;
    }

    return false;
}

const ParaDB::ParaPayload&
ParaDBSvc::getParameter(const std::string& name) {
    auto it = m_paradb_payloads.find(name);
    if (it != m_paradb_payloads.end()) {
        return *it->second;
    }

    return ParaDB::ParaPayload::null_payload();
}
