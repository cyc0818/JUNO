#include "LocalFSRepo.h"

#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

namespace CondDB {

    const std::string& LocalFSRepo::name() {
        return m_repo_name;
    }
    const std::string& LocalFSRepo::type() {
        return m_repo_type;
    }
    const std::string& LocalFSRepo::uri() {
        return m_repo_uri;
    }


    LocalFSRepo::LocalFSRepo(const std::string& repo_name, const std::string& repo_uri)
        : m_repo_name(repo_name),
          m_repo_type("LocalFSRepo"),
          m_repo_uri(repo_uri) {

    }

    LocalFSRepo::~LocalFSRepo() {

    }

    const std::vector<GlobalTag>&
    LocalFSRepo::findGlobalTags(const std::string& gt_name) {

        CacheGlobalTagVec::iterator it = m_gt_map.find(gt_name);

        // if not found in cache, try to load it from FS
        if (it == m_gt_map.end()) {
            const std::string path_str = 
                m_repo_uri + "/globaltags/" + gt_name;
            namespace fs = boost::filesystem;
            fs::path p(path_str);

            if (fs::exists(p)) {
                load_global_tags(gt_name, path_str);
            }
        }


        return m_gt_map[gt_name];
    }


    const std::vector<Tag>&
    LocalFSRepo::findTags(const GlobalTag& gt) {

        const std::string& gt_name = gt.name();

        CacheTagVec::iterator it = m_tag_map.find(gt_name);
        if (it == m_tag_map.end()) {
            load_tags(gt_name);
        }

        return m_tag_map[gt_name];
    }

    const std::vector<IOV>&
    LocalFSRepo::findIOVs(const Tag& tag) {

        const std::string& tag_name = tag.name();

        CacheIOVVec::iterator it = m_iov_map.find(tag_name);
        if (it == m_iov_map.end()) {
            load_iovs(tag);
        }

        return m_iov_map[tag_name];
    }

    const std::vector<Payload>&
    LocalFSRepo::findPayloads(const IOV& iov) {
        const std::string& payload_hash = iov.payload_hash();

        CachePayloadVec::iterator it = m_payload_map.find(payload_hash);
        
        if (it == m_payload_map.end()) {
            load_payload(iov);
        }

        return m_payload_map[payload_hash];
    }


    bool
    LocalFSRepo::load_global_tags(const std::string& gt_name,
                                  const std::string& path_str) {

        // parse the metadata of GlobalTag.
        // A JSON file:
        // {
        //   "name": "GT_NAME",
        //   "tags": ["tag1", "tag2"],
        // }
        boost::property_tree::ptree pt;

        std::ifstream ifs(path_str.c_str());

        boost::property_tree::read_json(ifs, pt);

        // check consistency
        if (gt_name != pt.get<std::string>("name")) {
            std::cerr << "Consistency check failed: expected " << gt_name
                      << " but got: " << pt.get<std::string>("name") << std::endl;
            return false;
        }

        // Build Global Tag object
        GlobalTag gt(gt_name);

        // TODO:
        // Read the tags in gt file:
        //   /globaltags/gt_name
        //
        // file format (containing multiple lines of tags):
        //   /tags/[Type]/tag_name
        // 
        // then cache the tags.

        m_gt_map[gt_name].push_back(gt);

        return true;
    }

    bool
    LocalFSRepo::load_tags(const std::string& gt_name) {
        const std::string path_str = 
            m_repo_uri + "/globaltags/" + gt_name;
        namespace fs = boost::filesystem;
        fs::path p(path_str);

        if (not fs::exists(p)) {
            return false;
        }

        boost::property_tree::ptree pt;

        std::ifstream ifs(path_str.c_str());

        boost::property_tree::read_json(ifs, pt);

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("tags")) {
            std::string temp_line = v.second.data();

            // Each line should contain:
            //   /tags/[Type]/[tag_name]

            std::stringstream ss;
            ss << temp_line;

            std::string raw_tag_path;
            ss >> raw_tag_path;

            if (raw_tag_path.size() == 0) {
                continue;
            }

            // check exists or not
            namespace fs = boost::filesystem;
            const std::string tag_path_str =
                m_repo_uri + raw_tag_path;
            fs::path tag_p(tag_path_str);
            if (not fs::exists(tag_p)) {
                continue;
            }

            // if exists, split it into three parts
            boost::char_separator<char> sep("/");
            typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

            tokenizer tok(raw_tag_path, sep);

            std::vector<std::string> tmp_tokens;

            for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {

                std::string s = *it;

                tmp_tokens.push_back(s);
            }

            if (tmp_tokens.size() != 3) {
                // warning here
                continue;
            }

            const std::string& str_prefix = tmp_tokens[0];
            const std::string& str_type = tmp_tokens[1];
            const std::string& str_tag = tmp_tokens[2];

            // only the prefix is tags, then create tag.
            if (str_prefix != "tags") {
                continue;
            }


            // parse the metadata of Tag.
            // A JSON file:
            // {
            //   "name": "TAG_NAME",
            //   "type": "TAG_TYPE",
            //   "iovs": ["iov1", "iov2"],
            // }
            boost::property_tree::ptree tag_pt;

            std::ifstream tag_ifs(tag_path_str.c_str());
            
            boost::property_tree::read_json(tag_ifs, tag_pt);
            // check consistency
            if (str_tag != tag_pt.get<std::string>("name")) {
                std::cerr << "Consistency check failed: expected " << str_tag
                          << " but got: " << tag_pt.get<std::string>("name") << std::endl;
                return false;
            }
            if (str_type != tag_pt.get<std::string>("type")) {
                std::cerr << "Consistency check failed: expected " << str_type
                          << " but got: " << tag_pt.get<std::string>("type") << std::endl;
                return false;
            }



            Tag tag(str_tag, str_type);

            m_tag_map[gt_name].push_back(tag);
        }

        return true;
    }

    bool
    LocalFSRepo::load_iovs(const Tag& tag) {

        const std::string& tag_name = tag.name();
        const std::string& tag_type = tag.type();

        const std::string path_str = 
            m_repo_uri + "/tags/" + tag_type + "/" + tag_name;

        namespace fs = boost::filesystem;
        fs::path p(path_str);

        if (not fs::exists(p)) {
            return false;
        }

        // Format of Tag file:
        //   /iovs/[Type]/[iov_name]
        //
        //   {"since": 123, "payload_hash": "..."}
        //
        // or packed:
        //   /iovs/[Type]/[iov_name].pack
        //
        //   {"iovs": [
        //     {"since": 123, "payload_hash": "..."},
        //     {"since": 456, "payload_hash": "..."}
        //    ], "entries": 2
        //   }

        // Reading the Tag file first, then according to the
        // reference, loading IOVs.
        boost::property_tree::ptree pt;

        std::ifstream ifs(path_str.c_str());

        boost::property_tree::read_json(ifs, pt);

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("iovs")) {
            std::string temp_line = v.second.data();

            // Each line should contain:
            //   /iovs/[Type]/[iov_or_iovs]

            std::stringstream ss;
            ss << temp_line;

            std::string raw_iovs_path;
            ss >> raw_iovs_path;

            if (raw_iovs_path.size() == 0) {
                continue;
            }

            // check exists or not
            namespace fs = boost::filesystem;
            const std::string iovs_path_str =
                m_repo_uri + raw_iovs_path;
            fs::path iovs_p(iovs_path_str);
            if (not fs::exists(iovs_p)) {
                continue;
            }

            // check starts from /iovs
            boost::char_separator<char> sep("/");
            typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

            tokenizer tok(raw_iovs_path, sep);

            std::vector<std::string> tmp_tokens;

            for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {

                std::string s = *it;

                tmp_tokens.push_back(s);
            }

            if (tmp_tokens.size() != 3) {
                // warning here
                continue;
            }

            const std::string& str_prefix = tmp_tokens[0];
            const std::string& str_type = tmp_tokens[1];
            const std::string& str_iovs = tmp_tokens[2];

            if (str_prefix != "iovs") {
                continue;
            }

            if (str_type != tag_type) {
                std::cout << "WARNING: " 
                          << "mismatch types between Tag and IOV: "
                          << tag_type << " "
                          << str_type
                          << std::endl;
                continue;
            }

            // load the real iov
            int entries = load_iovs_entries(tag, iovs_path_str);
        }


        return true;
    }

    int
    LocalFSRepo::load_iovs_entries(const Tag& tag,
                                   const std::string& entries_path) {

        int cnt = 0;

        const std::string& tag_name = tag.name();
        const std::string& tag_type = tag.type();


        // Format of Tag file:
        //   /iovs/[Type]/[iov_name]
        //
        //   {"since": 123, "payload_hash": "..."}
        //
        // or packed:
        //   /iovs/[Type]/[iov_name].pack
        //
        //   {"iovs": [
        //     {"since": 123, "payload_hash": "..."},
        //     {"since": 456, "payload_hash": "..."}
        //    ], "entries": 2
        //   }


        std::ifstream ifs(entries_path.c_str());

        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ifs, pt);

        boost::optional<boost::property_tree::ptree&> iovs = pt.get_child_optional("iovs");

        if (!iovs) { // single IOV

            boost::property_tree::ptree& pt_single = pt;

            if (load_iov_entry(tag, pt_single)) {
                ++cnt;
            }                

        } else { // packed IOVs

            BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("iovs")) {
            
                // Each line:
                //   [since] [hash]
                //
                // or
                //   [since] [user specified files]
                //
                // Note 1: [hash] refers to /payloads/[Type]/[hash]
                //
                // To balance the number of payloads in one directories,
                // The first two character are used as directory name. 
                //
                // Note 2: [user specified files]
                // 
                // The file format can be specified by developers.

                boost::property_tree::ptree& pt_single = v.second;
                if (load_iov_entry(tag, pt_single)) {
                    ++cnt;
                }
            }

        }

        return cnt;
    }

    bool LocalFSRepo::load_iov_entry(const Tag& tag, boost::property_tree::ptree& pt_single) {

        std::string raw_since = pt_single.get<std::string>("since");
        std::string raw_hash = pt_single.get<std::string>("payload_hash");
        boost::optional<std::string> raw_path = pt_single.get_optional<std::string>("payload_path");


        if (raw_since.size() == 0) {
            std::cout << "WARNING: "
                      << "since is empty."
                      << std::endl;

            return false;
        }

        if (raw_hash.size() == 0) {
            std::cout << "WARNING: "
                      << "since is " << raw_since
                      << ", but hash or path is empty"
                      << std::endl;
            return false;
        }

        int64_t since;
        std::stringstream ss_since;
        ss_since << raw_since;
        ss_since >> since;

        const std::string& tag_name = tag.name();
        const std::string& tag_type = tag.type();

        IOV iov(since, raw_hash, tag_type);
        iov.repo(this); // associate it with repo

        m_iov_map[tag_name].push_back(iov);


        return true;
    }


    bool LocalFSRepo::load_payload(const IOV& iov) {
        // according to the hash, load the payload data
        // Payload Path:
        //   /payloads/[Type]/__/___________________________
        //                    |<---------- hash ---------->|
        // or
        // User specified Path (TODO).

        const std::string& hash_ = iov.payload_hash();

        const std::string path_str =
            m_repo_uri
            + "/payloads/"
            + iov.object_type() + "/"
            + hash_.substr(0,2) + "/"
            + hash_.substr(2);

        namespace fs = boost::filesystem;
        fs::path p(path_str);

        if (not fs::exists(p)) {
            std::cerr << "Can't access file: " << path_str << std::endl;
            return false;
        }

        std::cout << "ACCESS PAYLOAD: " << path_str << std::endl;

        // Payload JSON format:
        // {
        //   "hash": "___________",
        //   "object_type": "____",
        //   "path": "___________",
        //   "data": "__BASE_64__",
        //   "streamer": "_______"
        // }
        //
        // Note:
        // * path: using payload path if data field is empty.
        // * data: base64 encoding data.
        // * streamer: indicates the conversation of objects.


        boost::property_tree::ptree pt;

        std::ifstream ifs(path_str.c_str());

        boost::property_tree::read_json(ifs, pt);

        // check consistency
        if (hash_ != pt.get<std::string>("hash")) {
            std::cerr << "Consistency check failed: expected " << hash_
                      << " but got: " << pt.get<std::string>("hash") << std::endl;
            return false;
        }

        if (iov.object_type() != pt.get<std::string>("object_type")) {
            std::cerr << "Consistency check failed: expected " << iov.object_type()
                      << " but got: " << pt.get<std::string>("object_type") << std::endl;
            return false;
        }

        const std::string path_ = pt.get<std::string>("path");
        const std::string data_ = pt.get<std::string>("data");
        const std::string streamer_ = pt.get<std::string>("streamer");


        Payload payload(hash_, 
                        iov.object_type(),
                        path_,
                        data_,
                        streamer_);
        
        m_payload_map[hash_].push_back(payload);
        
        return true;
    }


} // namespace CondDB
