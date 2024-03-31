#ifndef DBISvc_hh
#define DBISvc_hh

#include "DBISvc/IDBISvc.hh"
#include "SniperKernel/SvcBase.h"
#include <map>

class DBISvc: public IDBISvc, public SvcBase {
public:
    DBISvc(const std::string& name);
    ~DBISvc();

    bool initialize() override;
    bool finalize() override;

    std::shared_ptr<dbi::DBAPI> getConnection(const std::string& client) override;

    std::string getSchemaProperty(const std::string& client,
                                  const std::string& property) const override;
private:
    // connections
    //   key: connection name, std::string
    //   value: connection properties, std::map<std::string, std::string>
    //     - backend: required

    std::map<std::string, std::map<std::string, std::string>> m_conf_connections;

    // clients
    //   key: client name, std::string
    //   value: connection names, std::vector<std::string>
    //     note: only the first element will be used

    std::map<std::string, std::vector<std::string>> m_conf_clients;

    // schemas
    //   key: client name, std::string
    //   value: client properties, std::map<std::string, std::string>
    //     - database: required

    std::map<std::string, std::map<std::string, std::string>> m_conf_schemas;


private:
    // all the DBAPI instances are maintained here
    // only invoked by users, then create the connection. 
    std::map<std::string, std::shared_ptr<dbi::DBAPI>> m_dbapis;

    // helpers to create different backend DBAPI
    // note: the client name is used to find the conn_name
    std::shared_ptr<dbi::DBAPI> setup_connection(const std::string& conn_name);
    std::shared_ptr<dbi::DBAPI> setup_connection_frontier(const std::string& conn_name);
    std::shared_ptr<dbi::DBAPI> setup_connection_mysql(const std::string& conn_name);
    std::shared_ptr<dbi::DBAPI> setup_connection_sqlite(const std::string& conn_name);
};

#endif
