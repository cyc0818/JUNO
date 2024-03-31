#ifndef ParaDB_DBParaRepo_hh
#define ParaDB_DBParaRepo_hh

/*
 * Description:
 *
 *   An implementation of ParaRepo using DBI
 *
 * Author: Tao Lin <lintao AT ihep.ac.cn>
 *
 */

#include <memory>
#include <map>
#include <ParaDB/IParaRepo.hh>
#include <dbi/dbi.h>
#include <boost/format.hpp>

namespace ParaDB {

    class DBParaRepo: public IParaRepo {

    public:
        DBParaRepo(const std::string& repo_name, std::shared_ptr<dbi::DBAPI> db);
        ~DBParaRepo();

    public:
        // configure interfaces
        bool confDatabaseName(const std::string& dbname);
        bool confParaGTagTable(const std::string& sql_template);
        bool confParaGTag2TagTable(const std::string& sql_template);
        bool confParaTag2PayloadTable(const std::string& sql_template);

    public:
        const std::string& name() override;
        const std::string& type() override;
        const std::string& uri() override;

        const std::vector<ParaGTag>&
        findGlobalTags(const std::string& para_gtag_name) override;

        const std::vector<ParaTag>&
        findTags(const ParaGTag&) override;

        const std::vector<ParaPayload>&
        findPayloads(const ParaTag&) override;

    private:
        std::string m_repo_name;
        std::string m_repo_type;
        std::string m_repo_uri;

        // dbi
        std::shared_ptr<dbi::DBAPI> m_db_conn;

    private:
        // key: gtag, value: gtag objects
        std::map<std::string, std::vector<ParaGTag>> m_cache_gtags;

        // key: gtag, value: tag objects
        std::map<std::string, std::vector<ParaTag>> m_cache_tags;

        // key: tag, value: payload objects
        std::map<std::string, std::vector<ParaPayload>> m_cache_payloads;

    private:
        // followings are the tables related to ParaDB

        // table: 

        std::string m_db_name;

        struct ParaGTagTable {

            const std::string table_name = "ParaGTag";

            std::string tosql(const std::string& db_name, const std::string& gt_name) {
                if (sql_statement_template.size()) {
                    return (boost::format(sql_statement_template) 
                            % db_name
                            % table_name
                            % gt_name).str();
                }


                std::string sql = "select name from ";
                sql += db_name + "." + table_name + " where name='" + gt_name + "'";
                return sql;
            }

            std::string sql_statement_template;

        } paragtag_table;

        struct ParaGTag2TagTable {
            const std::string gtm_tbl_name = "ParaGTag2Tag";
            const std::string tag_tbl_name = "ParaTag";

            std::string tosql(const std::string& db_name, const std::string& gt_name) {
                if (sql_statement_template.size()) {
                    return (boost::format(sql_statement_template) 
                            % db_name
                            % gtm_tbl_name
                            % tag_tbl_name
                            % gt_name).str();
                }


                std::string sql = "select tag.name from ";
                sql += db_name + "." + gtm_tbl_name + " gtm ";
                sql += " inner join " + db_name + "." + tag_tbl_name + " tag ";
                sql += " on gtm.tag_name = tag.name";
                sql += " where gtm.para_gtag_name='" + gt_name + "'";

                return sql;
            }

            std::string sql_statement_template;

        } paragtag2tag_table;

        struct ParaTag2PayloadTable {
            const std::string ptp_tbl_name = "ParaTag2Payload";
            const std::string payload_tbl_name = "ParaPayload";

            std::string tosql(const std::string& db_name, const std::string& tag_name) {
                if (sql_statement_template.size()) {
                    return (boost::format(sql_statement_template) 
                            % db_name
                            % ptp_tbl_name
                            % payload_tbl_name
                            % tag_name).str();
                }

                std::string sql = "select payload.object_type, payload.property, ";
                sql += "payload.file_system, payload.path, payload.version ";
                sql += " from " + db_name + "." + ptp_tbl_name + " ptp ";
                sql += " inner join " + db_name + "." + payload_tbl_name + " payload ";
                sql += " on ptp.payload_id = payload.id";
                sql += " where ptp.tag_name='" + tag_name + "'";

                return sql;
            }
            std::string sql_statement_template;

        } paratag2payload_table;

    };

}

#endif
