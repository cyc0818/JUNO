#ifndef CondDB_LocalFSRepo_h
#define CondDB_LocalFSRepo_h

#include <CondDB/IRepo.h>
#include <map>
#include <boost/property_tree/ptree.hpp>

namespace CondDB {

    class LocalFSRepo: public IRepo {
    public:
        // Repo metadata:
        const std::string& name();
        const std::string& type();
        const std::string& uri();
    public:
        LocalFSRepo(const std::string& repo_name, const std::string& repo_uri);
        ~LocalFSRepo();
    public:
        // findGlobalTags:
        // return: a list of global tags
        // param:
        //     gt_name: GT name or names
        const std::vector<GlobalTag>& 
            findGlobalTags(const std::string& gt_name);
        
        // findTags:
        // return: a list of tags
        // param:
        //     gt: GlobalTag
        const std::vector<Tag>&
            findTags(const GlobalTag& gt);

        // findIOVs:
        // return: a list of iovs
        // param:
        //     tag: Tag
        const std::vector<IOV>&
            findIOVs(const Tag& tag);

        // findPayloads:
        // return: a list of payloads
        // param:
        //     iov: IOV
        const std::vector<Payload>&
            findPayloads(const IOV& iov);

    private:
        // load global tags
        bool load_global_tags(const std::string& gt_name,
                              const std::string& path_str);

        // load tags (the list of tags is stored in GT)
        bool load_tags(const std::string& gt_name);

        // load iovs (the list of iovs is stored in IOV)
        bool load_iovs(const Tag& tag);

        // return the loaded entries
        int load_iovs_entries(const Tag& tag, 
                              const std::string& entries_path);
        // helper: load an iov 
        bool load_iov_entry(const Tag& tag, boost::property_tree::ptree& pt_single);

        // load payload
        bool load_payload(const IOV& iov);

    private:
        std::string m_repo_name; // full path
        std::string m_repo_type; // full path
        std::string m_repo_uri; // full path

    private:
        // In the LocalFSRepo, all the results are cached.
        typedef std::vector<GlobalTag> GlobalTagVec;
        typedef std::map<std::string, GlobalTagVec> CacheGlobalTagVec;
        CacheGlobalTagVec m_gt_map; // key: gt

        typedef std::vector<Tag> TagVec;
        typedef std::map<std::string, TagVec> CacheTagVec;
        CacheTagVec m_tag_map; // key: gt

        typedef std::vector<IOV> IOVVec;
        typedef std::map<std::string, IOVVec> CacheIOVVec;
        CacheIOVVec m_iov_map; // key: tag

        typedef std::vector<Payload> PayloadVec;
        typedef std::map<std::string, PayloadVec> CachePayloadVec;
        CachePayloadVec m_payload_map; // key: hash

    }; // class LocalFSRepo

} // namespace CondDB


#endif
