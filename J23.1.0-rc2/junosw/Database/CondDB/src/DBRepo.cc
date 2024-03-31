#include "DBRepo.h"
#include <iostream>
#include <sstream>

namespace CondDB {

    const std::string& DBRepo::name() {
        return m_repo_name;
    }
    const std::string& DBRepo::type() {
        return m_repo_type;
    }
    const std::string& DBRepo::uri() {
        return m_repo_uri;
    }

    DBRepo::DBRepo(const std::string& repo_name, std::shared_ptr<dbi::DBAPI> db)
        : m_repo_name(repo_name), 
          m_repo_type("DBIRepo"),
          m_db_name("OfflineDB"), // override by confDatabaseName
          m_db_conn(db) {

    }

    DBRepo::~DBRepo() {


    }

    bool DBRepo::confDatabaseName(const std::string& dbname) {
        m_db_name = dbname;
        return true;
    }

    bool DBRepo::confGlobalTagTable(const std::string& sql_template) {
        globaltag_table.sql_statement_template = sql_template;
        return true;
    }

    bool DBRepo::confGlobalTag2TagsTable(const std::string& sql_template) {
        globaltag2tags_table.sql_statement_template = sql_template;
        return true;
    }

    bool DBRepo::confTag2IovsTable(const std::string& sql_template) {
        tag2iovs_table.sql_statement_template = sql_template;
        return true;
    }

    bool DBRepo::confPayloadTable(const std::string& sql_template) {
        payload_table.sql_statement_template = sql_template;
        return true;
    }

    const std::vector<GlobalTag>&
    DBRepo::findGlobalTags(const std::string& gt_name) {
        CacheGlobalTagVec::iterator it = m_gt_map.find(gt_name);
        if (it == m_gt_map.end()) {

            load_gt(gt_name);

        }

        return m_gt_map[gt_name];

    }

    const std::vector<Tag>&
    DBRepo::findTags(const GlobalTag& gt) {
        const std::string& gt_name = gt.name();

        CacheTagVec::iterator it = m_tag_map.find(gt_name);
        if (it == m_tag_map.end()) {
            load_tags(gt_name);
        }

        return m_tag_map[gt_name];


    }

    const std::vector<IOV>&
    DBRepo::findIOVs(const Tag& tag) {
        const std::string& tag_name = tag.name();

        CacheIOVVec::iterator it = m_iov_map.find(tag_name);
        if (it == m_iov_map.end()) {
            load_iovs(tag);
        }

        return m_iov_map[tag_name];

    }

    const std::vector<Payload>&
    DBRepo::findPayloads(const IOV& iov) {
        const std::string& payload_hash = iov.payload_hash();

        CachePayloadVec::iterator it = m_payload_map.find(payload_hash);
        
        if (it == m_payload_map.end()) {
            load_payload(iov);
        }

        return m_payload_map[payload_hash];

    }


    void
    DBRepo::load_gt(const std::string& gt_name) {
        // std::string tbl_name = "global_tag";
        // std::string sql = "select name from ";
        // sql += m_db_name; sql += "."; sql += tbl_name;
        // sql += " where name='"; sql += gt_name; sql += "'";

        auto sql = globaltag_table.tosql(m_db_name, gt_name);

        if (m_db_conn) { // dbi
            auto results = m_db_conn->query<std::string // CondGTag
                                            >(sql);
            for (auto result: results) {
                auto [condgtag] = result;
            }

            // Build Global Tag object
            GlobalTag gt(gt_name);
            m_gt_map[gt_name].push_back(gt);

        }

    }

    void
    DBRepo::load_tags(const std::string& gt_name) {
        // std::string gtm_tbl_name = "global_tag_map";
        // std::string tag_tbl_name = "tag";
        // std::string sql = "select tag.name, tag.object_type";
        // sql += " from ";
        // sql += m_db_name; sql += "."; sql += gtm_tbl_name;
        // //sql += " gtm";
        // sql += " inner join ";
        // sql += m_db_name; sql += "."; sql += tag_tbl_name;
        // sql += " on global_tag_map.tag_name = tag.name";
        // sql += " where global_tag_map.global_tag_name='"; sql += gt_name; sql += "'";

        auto sql = globaltag2tags_table.tosql(m_db_name, gt_name);

        std::cout << sql << std::endl;

        if (m_db_conn) {
            auto results = m_db_conn->query<std::string, // tag name
                                            std::string  // tag type
                                            >(sql);
            for (auto result: results) {
                auto [name, type] = result;
                Tag tag(name, type);

                m_tag_map[gt_name].push_back(tag);
            }
        }
    }

    void
    DBRepo::load_iovs(const Tag& tag) {

        const std::string& tag_name = tag.name();
        const std::string& tag_type = tag.type();

        // std::string tim_tbl_name = "tag_iov_map";
        // std::string iov_tbl_name = "iov";

        // std::string sql = "select iov.since, iov.payload_hash, iov.object_type";
        // sql += " from ";
        // sql += m_db_name; sql += "."; sql += tim_tbl_name;
        // //sql += " tim";
        // sql += " inner join ";
        // sql += m_db_name; sql += "."; sql += iov_tbl_name;
        // sql += " on tag_iov_map.iov_id = iov.id";
        // sql += " where tag_iov_map.tag_name='";
        // sql += tag_name + "'";

        auto sql = tag2iovs_table.tosql(m_db_name, tag_name);

        std::cout << sql << std::endl;

        if (m_db_conn) {
            auto results = m_db_conn->query<int64_t,     // since
                                            std::string, // hash
                                            std::string  // type
                                            >(sql);

            for (auto result: results) {
                auto [iov_since, iov_hash, iov_type] = result;

                IOV iov(iov_since, iov_hash, iov_type);
                iov.repo(this); // associate it with repo

                m_iov_map[tag_name].push_back(iov);

            }

        }
    }

    void
    DBRepo::load_payload(const IOV& iov) {
        const std::string& hash_ = iov.payload_hash();

        // std::string sql = "select hash, object_type, path, data, '' as streamer_info";
        // sql += " from ";
        // sql += m_db_name; sql += "."; sql += payload_tbl;
        // sql += " where hash='"; sql += hash_; sql += "'";
        auto sql = payload_table.tosql(m_db_name, hash_);
        std::cout << sql << std::endl;

        if (m_db_conn) {
            auto results = m_db_conn->query<std::string, // hash
                                            std::string, // type
                                            std::string, // path
                                            std::string, // data
                                            std::string  // streamer info
                                            >(sql);
            for (auto result: results) {
                auto [one_hash, one_type, one_path, one_data, one_streamer] = result;
                Payload payload(hash_, 
                                one_type,
                                one_path,
                                one_data,
                                one_streamer);
        
                m_payload_map[hash_].push_back(payload);
            }

        }
    }

}; // namespace CondDB
