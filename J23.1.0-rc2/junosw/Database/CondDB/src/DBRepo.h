#ifndef CondDB_DBRepo_h
#define CondDB_DBRepo_h

#include <CondDB/IRepo.h>
#include <map>
#include <list>
#include <dbi/dbi.h>
#include <DBISvc/IDBISvc.hh>
#include <boost/format.hpp>

namespace CondDB {

    class DBRepo: public IRepo {
    public:
        // Repo metadata:
        const std::string& name();
        const std::string& type();
        const std::string& uri();
    public:
        // Use DBAPI as backend
        DBRepo(const std::string& repo_name, std::shared_ptr<dbi::DBAPI> db);
        ~DBRepo();

    public:
        // configure interfaces
        bool confDatabaseName(const std::string& dbname);
        bool confGlobalTagTable(const std::string& sql_template);
        bool confGlobalTag2TagsTable(const std::string& sql_template);
        bool confTag2IovsTable(const std::string& sql_template);
        bool confPayloadTable(const std::string& sql_template);

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
        void load_gt(const std::string& gt_name);
        void load_tags(const std::string& gt_name);
        void load_iovs(const Tag& tag);
        void load_payload(const IOV& iov);

    private:
        std::string m_repo_name;
        std::string m_repo_type;
        std::string m_repo_uri;
        // based on dbi
        std::shared_ptr<dbi::DBAPI> m_db_conn;

    private:
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

    private:
        std::string m_db_name; // database

        // followings are the names of tables
        struct GlobalTagTable {
            const std::string table_name = "GlobalTag";

            std::string tosql(const std::string& db_name, const std::string& gt_name) {
                if (sql_statement_template.size()) {
                    return (boost::format(sql_statement_template) 
                            % db_name
                            % table_name
                            % gt_name).str();
                }

                std::string sql = "select cond_gtag_name from ";
                sql += db_name + "." + table_name + " where cond_gtag_name='" + gt_name +"'"; 
                return sql;
            } 
            std::string sql_statement_template;
        } globaltag_table;

        struct GlobalTag2TagsTable {
            const std::string gtm_tbl_name = "CondGTag2Tag";
            const std::string tag_tbl_name = "CondTag";

            std::string tosql(const std::string& db_name, const std::string& gt_name) {
                if (sql_statement_template.size()) {
                    return (boost::format(sql_statement_template) 
                            % db_name
                            % gtm_tbl_name
                            % tag_tbl_name
                            % gt_name).str();
                }

                std::string sql;
                sql += "select tag.name, tag.object_type";
                sql += " from ";
                sql += db_name + "." + gtm_tbl_name + " gtm";
                sql += " inner join ";
                sql += db_name + "." + tag_tbl_name + " tag";
                sql += " on gtm.tag_name = tag.name";
                sql += " where gtm.cond_gtag_name='" + gt_name + "'";

                return sql;
            }

            std::string sql_statement_template;
        } globaltag2tags_table;

        struct Tag2IovsTable {
            const std::string tim_tbl_name = "CondTag2IOV";
            const std::string iov_tbl_name = "CondIOV";

            std::string tosql(const std::string& db_name, const std::string& tag_name) {
                if (sql_statement_template.size()) {
                    return (boost::format(sql_statement_template) 
                            % db_name
                            % tim_tbl_name
                            % iov_tbl_name
                            % tag_name).str();
                }

                std::string sql;
                sql += "select iov.since, iov.payload_hash, iov.object_type";
                sql += " from ";
                sql += db_name + "." + tim_tbl_name + " tim";
                sql += " inner join ";
                sql += db_name + "." + iov_tbl_name + " iov";
                sql += " on tim.iov_id = iov.id";
                sql += " where tim.tag_name='" + tag_name + "'";

                return sql;
            }
            std::string sql_statement_template;
        } tag2iovs_table;

        struct PayloadTable {
            const std::string payload_tbl = "CondPayload";

            std::string tosql(const std::string& db_name, const std::string& hash_) {
                if (sql_statement_template.size()) {
                    return (boost::format(sql_statement_template) 
                            % db_name
                            % payload_tbl
                            % hash_).str();
                }

                std::string sql;
                sql += "select hash, object_type, path, data, streamer_info";
                sql += " from ";
                sql += db_name + "." + payload_tbl;
                sql += " where hash='" + hash_ + "'";
                return sql;
            }

            std::string sql_statement_template;

        } payload_table;

    }; // class DBRepo

}; // namespace CondDB

#endif

