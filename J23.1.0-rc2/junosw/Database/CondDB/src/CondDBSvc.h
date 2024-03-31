#ifndef CondDBSvc_h
#define CondDBSvc_h

#include <CondDB/ICondDBSvc.h>
#include <CondDB/IConditionObject.h>
#include <CondDB/IRepo.h>
#include <SniperKernel/SvcBase.h>
#include <boost/shared_ptr.hpp>
#include <map>

class CondDBSvc: public CondDB::ICondDBSvc, public SvcBase
{
public:
    CondDBSvc(const std::string& name);
    ~CondDBSvc();

public:

    virtual const std::vector<std::string>& getRepoNames();
    virtual CondDB::IRepo* getRepo(const std::string& name);

public:
    virtual bool setCurrent(const int64_t& cur);

public:
    virtual bool declCondObj(const std::string& cond_obj_path,
                             CondDB::IConditionObject& cond_obj);

public:
    virtual bool initialize();
    virtual bool finalize();

private:

    // loop the repo from low prio to high prio, then find the best one
    const CondDB::IOV* find_iov_current_prio(const std::string& type,
                                             const int64_t&);

    const CondDB::IOV* find_iov_current_repo(CondDB::IRepo* repo,
                                             const std::string& type,
                                             const int64_t& cur);

private:
    typedef boost::shared_ptr<CondDB::IRepo> IRepoPtr;
    typedef std::map<std::string, IRepoPtr> MapRepos;
    MapRepos m_map_repo;

    typedef std::vector<std::string> VecRepoNames;
    VecRepoNames m_vec_repo_names;

    // key: label, value: repo types
    typedef std::map<std::string, std::string> MapRepoTypes;
    MapRepoTypes m_map_repo_types;

    // key: label, value: repo uri
    // URI format:
    // * file://  -> Local file system Repo
    // * mysql:// -> MySQL backend Repo
    typedef std::map<std::string, std::string> MapRepoURIs;
    MapRepoURIs m_map_repo_uris;

    // proxy list
    // such as: ["http://squid-01.ihep.ac.cn:3128"]
    std::vector<std::string> m_vec_repo_proxies;

    // key: label, value: repo priority
    // For an example, priority of DBRepo is 10, priority of FSRepo is 20,
    // so the iov from FSRepo will override iov from DBRepo.
    // It is also possible that user setups two FSRepos with different 
    // priorities.
    typedef std::map<std::string, std::string> MapRepoPriorities;
    MapRepoPriorities m_map_repo_priorities;

private:

    // key: condition object type
    typedef std::map<std::string, CondDB::IConditionObject*> MapCondObjs;
    MapCondObjs m_map_condobjs;
    typedef std::map<std::string, int64_t> MapLastSince;
    MapLastSince m_map_lastsinces;

private:

    // global tag name is fixed for each job.
    std::string m_globaltag_name;

}; // class CondDBSvc


#endif
