#include "CondDBSvc.h"

#include "LocalFSRepo.h"
#include "DBRepo.h"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SharedElemFactory.h"

DECLARE_SERVICE(CondDBSvc);
DECLARE_CAN_BE_SHARED(CondDBSvc);

CondDBSvc::CondDBSvc(const std::string& name)
    : SvcBase(name) {
    declProp("RepoTypes", m_map_repo_types);
    declProp("RepoURIs", m_map_repo_uris);
    declProp("RepoPriorities", m_map_repo_priorities);

    declProp("GlobalTag", m_globaltag_name);
}

CondDBSvc::~CondDBSvc() {

}

const std::vector<std::string>&
CondDBSvc::getRepoNames() {
    return m_vec_repo_names;
}

CondDB::IRepo*
CondDBSvc::getRepo(const std::string& name) {
    MapRepos::iterator it = m_map_repo.find(name);

    if (it != m_map_repo.end()) {
        return it->second.get();
    }

    return NULL;
}

bool
CondDBSvc::setCurrent(const int64_t& cur) {
    // notify all payloads and condition objects
    LogInfo << "Update the current: " << cur << std::endl;
    // TODO: simplify the code

    for (MapCondObjs::iterator it_condobj = m_map_condobjs.begin();
         it_condobj != m_map_condobjs.end(); ++it_condobj) {

        const std::string& condobj_path = it_condobj->first; // object type
        const std::string condobj_type = condobj_path.substr(
                                                             0,
                                                             condobj_path.find("/")
                                                             );

        CondDB::IConditionObject* condobj_ptr = it_condobj->second;

        LogInfo << "-> condobj_type: " << condobj_type
                << " [" << condobj_path << "]" << std::endl;

        const CondDB::IOV* ptr_iov = find_iov_current_prio(condobj_type, cur);
        if (!ptr_iov) {
            LogWarn << "Can't find iov for " << condobj_type << " " << cur << std::endl;
            continue;
        }
        const CondDB::IOV& iov = *ptr_iov;

        // find the first IOV
        // before loading payload, we need to check whether the iov is the same one or not
        if (m_map_lastsinces.find(condobj_path) != m_map_lastsinces.end()) {
            // same iov, we don't need to continue
            if (iov.since() == m_map_lastsinces[condobj_path]) {

                LogInfo << "skip load payload. "
                        << " cur: " << cur
                        << " current iov since: " << iov.since()
                        << " last iov since: " << m_map_lastsinces[condobj_path]
                        << std::endl;
                continue;
            }
        }
        // if not same, just update the last since.
        m_map_lastsinces[condobj_path] = iov.since();
        // get the Payload
        const std::vector<CondDB::Payload>& vec_payload = iov.repo()->findPayloads(iov);
        if (vec_payload.size() == 0) {
            LogWarn << "Can't find payload with hash "
                    << iov.payload_hash()
                    << std::endl;
            continue;
        }

        const CondDB::Payload& payload = vec_payload[0];

        // convert
        // - get streamer:
        const std::string streamer = payload.streamer();
        CondDB::ICnvFromPayload* cnv = condobj_ptr->getCnv(streamer);
        if (!cnv) {
            return false;
        }
        cnv->cnv(payload);

    }

    LogInfo << "Finish to update the current: " << cur << std::endl;

    return true;
}

bool
CondDBSvc::declCondObj(const std::string& cond_obj_path,
                       CondDB::IConditionObject& cond_obj) {
    if (m_map_condobjs.find(cond_obj_path) != m_map_condobjs.end()) {
        LogError << "The cond_obj_path '"
                 << cond_obj_path
                 << "' already exists." << std::endl;
        return false;
    }
    
    m_map_condobjs[cond_obj_path] = &cond_obj;

    return true;
}

bool
CondDBSvc::initialize() {

    // check global tag
    if (m_globaltag_name.empty()) {
        LogError << "Empty global tag name." << std::endl;
        return false;
    }


    for (MapRepoTypes::iterator it = m_map_repo_types.begin();
         it != m_map_repo_types.end(); ++it) {

        const std::string& nam = it->first;

        m_vec_repo_names.push_back(nam);

        // initialize the Repo
        const std::string& typ = it->second;
        const std::string& uri = m_map_repo_uris[nam];

        if (uri.empty()) {
            LogError << "URI is empty!" << std::endl;
            return false;
        }

        IRepoPtr repo;


        if (typ.empty()) {
            LogError << "Type is empty!" << std::endl;
            return false;

        } else if (typ == "LocalFSRepo") {
            boost::shared_ptr<CondDB::LocalFSRepo> repo_(new CondDB::LocalFSRepo(typ, uri));

            repo = repo_;

        } else if (typ == "DBI") {
            SniperPtr<IDBISvc> dbisvc(getParent(), "DBISvc");
            if (dbisvc.invalid()) {
                LogError << "Failed to find DBISvc. " << std::endl;
                return false;
            }

            std::string client = "conddb";

            auto db_conn = dbisvc->getConnection(client);
            if (!db_conn) {
                LogError << "Failed to setup database connection for conddb" << std::endl;
                return false;
            }

            boost::shared_ptr<CondDB::DBRepo> repo_(new CondDB::DBRepo(typ, db_conn));
            // passing the properties from dbisvc to repo
            repo_->confDatabaseName(dbisvc->getSchemaProperty(client, "database"));
            repo_->confGlobalTagTable(dbisvc->getSchemaProperty(client, "GlobalTagTable"));
            repo_->confGlobalTag2TagsTable(dbisvc->getSchemaProperty(client, "GlobalTag2TagsTable"));
            repo_->confTag2IovsTable(dbisvc->getSchemaProperty(client, "Tag2IovsTable"));
            repo_->confPayloadTable(dbisvc->getSchemaProperty(client, "PayloadTable"));

            repo = repo_;

            LogInfo << "Setup DBI based CondDBSvc. " << std::endl;

        } else {
            LogError << "Unsupported type " << typ << std::endl;
            return false;
        }

        m_map_repo[nam] = repo;
    }
    
    return true;
}

bool
CondDBSvc::finalize() {
    return true;
}

const CondDB::IOV*
CondDBSvc::find_iov_current_prio(const std::string& condobj_type,
                                 const int64_t& cur) {
    LogInfo << " condobj_type/cur: "
            << condobj_type
            << "/"
            << cur
            << std::endl;
    const CondDB::IOV* iov = NULL;

    const std::vector<std::string>& reponames = getRepoNames();

    // note, low prio at front, high prio at back
    for (std::vector<std::string>::const_iterator it = reponames.begin(); 
         it != reponames.end(); ++it) {
        CondDB::IRepo* repo = getRepo(*it);
        if (!repo) {
            LogError << "Can't find Repo: " << *it << std::endl;
            return iov;
        }

        LogInfo << "finding iov with repo "
                << repo->name() << " / "
                << repo->type() << " / "
                << repo->uri() 
                << std::endl;

        iov = find_iov_current_repo(repo, condobj_type, cur);

        LogInfo << "Get IOV: " << iov->since() << std::endl;
    }

    return iov;
}

const CondDB::IOV*
CondDBSvc::find_iov_current_repo(CondDB::IRepo* repo,
                                 const std::string& type,
                                 const int64_t& cur) {
    LogInfo << " condobj_type/cur: "
            << type
            << "/"
            << cur
            << std::endl;


    const CondDB::IOV* iov = NULL;
    bool is_in_range = false;

    LogInfo << "finding iov with global tag: " << m_globaltag_name << std::endl;

    const std::vector<CondDB::GlobalTag>& vec_gt = repo->findGlobalTags(m_globaltag_name);
    if (vec_gt.size()!=1) {
        LogError << "Unexpected size of global tags: " << vec_gt.size() << std::endl;
        return iov;
    }

    LogInfo << "size of global tags: " << vec_gt.size() << std::endl;

    // now, according to the global tag, get all related tags.
    const CondDB::GlobalTag& gt = vec_gt[0];

    LogInfo << "The first global tag: " << gt.name() << std::endl;

    const std::vector<CondDB::Tag>& vec_tag = repo->findTags(gt);

    LogInfo << "size of tags: " << vec_tag.size() << std::endl;

    // cache the first and last tag
    const CondDB::Tag* tag_first_ptr = NULL;
    const CondDB::Tag* tag_last_ptr = NULL;

    for (std::vector<CondDB::Tag>::const_iterator it = vec_tag.begin();
         it != vec_tag.end(); ++it) {
        const CondDB::Tag& tag = *it;

        LogInfo << "Tag: " << tag.name() << "/" << tag.type() << std::endl;

        // in the test, we only select the specific type.
        if (tag.type() != type) {
            continue;
        }

        if (!tag_first_ptr) {
            tag_first_ptr = &tag;
        }
        tag_last_ptr = &tag;

        // list iov in the tag
        const std::vector<CondDB::IOV>& vec_iov = repo->findIOVs(tag);

        int idx = -1;
        int siz = vec_iov.size();

        for (int i = 1; i < siz; ++i) {
            const CondDB::IOV& iov1 = vec_iov[i-1];
            const CondDB::IOV& iov2 = vec_iov[i];

            if (iov1.since() <= cur &&
                cur < iov2.since()) {
                idx = i-1;
                break;
            }
        }

        if (idx != -1) {
            iov = &vec_iov[idx];
            is_in_range = true;
        }
    }
    // note: the IOV of different tags can be overlapped.
    // The new one will override the old one.
    //  v1:  --------------
    //  v2:            --------------
    //        ^          ^       ^
    //        |          |       |
    //      use v1       |     use v2
    //                 use v2

    // check first and last exists
    if (tag_first_ptr==NULL or tag_last_ptr==NULL) {
        LogWarn << "Can't find tag for " << type << " in repo " << repo->name()  << std::endl;
    }

    // Then, the out range part should be check:
    if (tag_first_ptr and tag_last_ptr and !is_in_range) {
        // the first
        const CondDB::Tag& tag_first = *tag_first_ptr;
        const std::vector<CondDB::IOV>& iovs_first = repo->findIOVs(tag_first);
        const CondDB::IOV& iov_first = iovs_first[0];
        // the last
        int sz = vec_tag.size();
        const CondDB::Tag& tag_last = *tag_last_ptr;
        const std::vector<CondDB::IOV>& iovs_last = repo->findIOVs(tag_last);
        sz = iovs_last.size();
        const CondDB::IOV& iov_last = iovs_last[sz-1];

        if (cur < iov_first.since()) {
            iov = &iov_first;
        } else if (iov_last.since() <= cur) {
            iov = &iov_last;
        }

    }


    return iov;
}
