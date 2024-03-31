#include "DBParaRepo.hh"
#include <iostream>

namespace ParaDB {

DBParaRepo::DBParaRepo(const std::string& repo_name, std::shared_ptr<dbi::DBAPI> db)
    : m_repo_name(repo_name), m_repo_type("DBParaRepo"), m_repo_uri("dbi://paradb"),
      m_db_conn(db), m_db_name("OfflineDB") {

}

DBParaRepo::~DBParaRepo() {

}

bool DBParaRepo::confDatabaseName(const std::string& dbname) {
    m_db_name = dbname;
    return true;
}

bool DBParaRepo::confParaGTagTable(const std::string& sql_template) {
    paragtag_table.sql_statement_template = sql_template;
    return true;
}

bool DBParaRepo::confParaGTag2TagTable(const std::string& sql_template) {
    paragtag2tag_table.sql_statement_template = sql_template;
    return true;
}

bool DBParaRepo::confParaTag2PayloadTable(const std::string& sql_template) {
    paratag2payload_table.sql_statement_template = sql_template;
    return true;
}


const std::string& DBParaRepo::name() {
    return m_repo_name;
}

const std::string& DBParaRepo::type() {
    return m_repo_type;
}

const std::string& DBParaRepo::uri() {
    return m_repo_uri;
}

const std::vector<ParaGTag>&
DBParaRepo::findGlobalTags(const std::string& para_gtag_name) {

    // try to find the gtags in cache
    auto it = m_cache_gtags.find(para_gtag_name);
    if (it != m_cache_gtags.end()) {
        return it->second;
    }

    // not in cache
    auto sql = paragtag_table.tosql(m_db_name, para_gtag_name);
    std::cout << "Executing sql: " << sql << std::endl;
    if (m_db_conn) {
        auto rs = m_db_conn->query<std::string>(sql);

        for (auto r: rs) {
            auto [paragtag] = r;

            ParaGTag gt{paragtag};
            m_cache_gtags[para_gtag_name].push_back(gt);

        }
        return m_cache_gtags[para_gtag_name];
    }

    static std::vector<ParaGTag> null_result;
    return null_result;
}

const std::vector<ParaTag>&
DBParaRepo::findTags(const ParaGTag& gtag) {
    auto& para_gtag_name = gtag.name();

    // try to find the gtags in cache
    auto it = m_cache_tags.find(para_gtag_name);
    if (it != m_cache_tags.end()) {
        return it->second;
    }

    // not in cache
    auto sql = paragtag2tag_table.tosql(m_db_name, para_gtag_name);
    std::cout << "Executing sql: " << sql << std::endl;
    if (m_db_conn) {
        auto rs = m_db_conn->query<std::string>(sql);

        for (auto r: rs) {
            auto [tagname] = r;

            ParaTag tag{tagname};
            m_cache_tags[para_gtag_name].push_back(tag);

        }
        return m_cache_tags[para_gtag_name];
    }

    static std::vector<ParaTag> null_result;
    return null_result;
    
}

const std::vector<ParaPayload>&
DBParaRepo::findPayloads(const ParaTag& tag) {
    auto& tag_name = tag.name();

    // try to find the gtags in cache
    auto it = m_cache_payloads.find(tag_name);
    if (it != m_cache_payloads.end()) {
        return it->second;
    }

    // not in cache

    auto sql = paratag2payload_table.tosql(m_db_name, tag_name);
    std::cout << "Executing sql: " << sql << std::endl;
    if (m_db_conn) {
        auto rs = m_db_conn->query<std::string,        // object_type
                                   std::string,        // property
                                   std::string,        // fs
                                   std::string,        // path
                                   std::string>(sql);  // version

        for (auto r: rs) {
            auto [ot,pr,fs,p,v] = r;

            ParaPayload payload{ot,pr,fs,p,v};
            m_cache_payloads[tag_name].push_back(payload);

        }
        return m_cache_payloads[tag_name];
    }

    static std::vector<ParaPayload> null_result;
    return null_result;

}


} // namespace ParaDB
