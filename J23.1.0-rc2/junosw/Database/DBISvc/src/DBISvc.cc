#include "DBISvc.hh"

#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SharedElemFactory.h"
#include "SniperKernel/SniperLog.h"

// include different backends
#include "dbi/FrontierDB.h"
#include "dbi/MysqlDB.h"
#include "dbi/SqliteDB.h"

DECLARE_SERVICE(DBISvc);
DECLARE_CAN_BE_SHARED(DBISvc);
// todo: currently, this service is not thread-safe. 
// -- Tao Lin <lintao AT ihep.ac.cn>


DBISvc::DBISvc(const std::string& name)
    : SvcBase(name)
{
    declProp("Connections", m_conf_connections);
    declProp("Clients", m_conf_clients);
    declProp("Schemas", m_conf_schemas);
}

DBISvc::~DBISvc() {

}

bool DBISvc::initialize() {

    // check conf of connections
    for (const auto& [conn_name, conn_props]: m_conf_connections) {
        // does 'backend' exist?
        auto it_backend = conn_props.find("backend");
        if (it_backend == conn_props.end()) {
            LogError << "Can not find 'backend' property for connection: "
                     << conn_name
                     << std::endl;
            return false;
        }
    }

    // check conf of clients
    for (const auto& [client_name, conn_names]: m_conf_clients) {
        if (conn_names.size() == 0) {
            LogError << "Can not find any required connection names for client: "
                     << client_name
                     << std::endl;
            return false;
        }

        for (auto conn_name: conn_names) {
            auto it = m_conf_connections.find(conn_name);
            if (it == m_conf_connections.end()) {
                LogError << "Find invalid connection name '"
                         << conn_name << "' "
                         << " which is not registered in connections. "
                         << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool DBISvc::finalize() {
    return true;
}

std::shared_ptr<dbi::DBAPI> DBISvc::getConnection(const std::string& client) {

    // first, check whether client in conf_clients
    auto it_client = m_conf_clients.find(client);
    if (it_client == m_conf_clients.end()) {
        LogError << "Can not find registered client '" << client << "' " << std::endl;
        return nullptr;
    }

    // find the associated connections
    auto& connections = it_client->second;
    if (connections.size() == 0) {
        LogError << "Can not find any required connection names for client: "
                 << client
                 << std::endl;
        return nullptr;
    }

    // get the first one
    auto& conn_name = connections[0];

    // now, try to find in the dbapis
    auto it_dbi = m_dbapis.find(conn_name);
    if (it_dbi != m_dbapis.end()) {
        return it_dbi->second;
    }

    // if not exists, create a new instance and register it
    auto dbi_instance = setup_connection(conn_name);
    if (dbi_instance) {
        m_dbapis[conn_name] = dbi_instance;
    }

    return dbi_instance;
}

std::shared_ptr<dbi::DBAPI>
DBISvc::setup_connection(const std::string& conn_name) {
    // check conn_name valid
    auto it_conf_conn = m_conf_connections.find(conn_name);
    if (it_conf_conn == m_conf_connections.end()) {
        LogError << "Can not find registered connection '" << conn_name << "' " << std::endl;
        return nullptr;
    }

    auto conf_connection = it_conf_conn->second;

    // get the backend first
    auto it_backend = conf_connection.find("backend");
    if (it_backend == conf_connection.end()) {
        LogError << "Can not find valid backend for connection '" << conn_name << "' " << std::endl;
        return nullptr;
    }

    auto backend = it_backend->second;

    LogInfo << "Try to initialize the DB connection with backend " << backend << std::endl;

    std::shared_ptr<dbi::DBAPI> db_conn{nullptr};

    if (backend == "frontier") {
        db_conn = setup_connection_frontier(conn_name);
    } else if (backend == "sqlite") {
        db_conn = setup_connection_sqlite(conn_name);
    } else if (backend == "mysql") {
        db_conn = setup_connection_mysql(conn_name);
    } else {
        LogError << "The DB backend " << backend << " does not support yet. " << std::endl;
    }

    return db_conn;
}

std::shared_ptr<dbi::DBAPI>
DBISvc::setup_connection_frontier(const std::string& conn_name) {
    // retrieve the conf
    auto it_conf_conn = m_conf_connections.find(conn_name);
    if (it_conf_conn == m_conf_connections.end()) {
        LogError << "Can not find registered connection '" << conn_name << "' " << std::endl;
        return nullptr;
    }

    auto conf_connection = it_conf_conn->second;

    // check server
    auto it_server = conf_connection.find("server");
    if (it_server == conf_connection.end()) {
        LogError << "Can not find server name for connection '" << conn_name << "' " << std::endl;
        return nullptr;
    }

    auto& server_name = it_server->second;

    // check proxy
    std::string proxy_name;
    auto it_proxy = conf_connection.find("proxy");
    if (it_proxy != conf_connection.end()) {
        proxy_name = it_proxy->second;
    }

    // create the real dbi

    dbi::FrontierDB frontierdb{server_name, proxy_name};
    std::shared_ptr<dbi::DBAPI> db_conn(new dbi::DBAPI(frontierdb));
    db_conn->connect();

    return db_conn;
}

std::shared_ptr<dbi::DBAPI>
DBISvc::setup_connection_mysql(const std::string& conn_name) {
    #ifndef BUILD_ONLINE
    // retrieve the conf
    auto it_conf_conn = m_conf_connections.find(conn_name);
    if (it_conf_conn == m_conf_connections.end()) {
        LogError << "Can not find registered connection '" << conn_name << "' " << std::endl;
        return nullptr;
    }

    auto conf_connection = it_conf_conn->second;

    // two cases
    // - server/username/password
    // - mysql cnf / section

    // check server
    auto it_server = conf_connection.find("server");
    if (it_server != conf_connection.end()) {
        auto& server_name = it_server->second;

        // user name
        std::string username;
        auto it_username = conf_connection.find("username");
        if (it_username != conf_connection.end()) {
            username = it_username->second;
        }

        // user password
        std::string password;
        auto it_pass = conf_connection.find("password");
        if (it_pass != conf_connection.end()) {
            password = it_pass->second;
        }


        // create the real dbi

        dbi::MysqlDB db{server_name, username, password};
        std::shared_ptr<dbi::DBAPI> db_conn(new dbi::DBAPI(db));
        db_conn->connect();
    
        return db_conn;
    }


    #endif
    return nullptr;
}

std::shared_ptr<dbi::DBAPI>
DBISvc::setup_connection_sqlite(const std::string& conn_name) {
    // retrieve the conf
    auto it_conf_conn = m_conf_connections.find(conn_name);
    if (it_conf_conn == m_conf_connections.end()) {
        LogError << "Can not find registered connection '" << conn_name << "' " << std::endl;
        return nullptr;
    }

    auto conf_connection = it_conf_conn->second;

    // option:
    // - path
    // - schema_name
    std::string schema_name;
    auto it_schema_name = conf_connection.find("schema_name");
    if (it_schema_name != conf_connection.end()) {
        schema_name = it_schema_name->second;
    }
    
    // check server
    auto it_path = conf_connection.find("path");
    if (it_path != conf_connection.end()) {
        auto& path = it_path->second;

        // create the real dbi
        dbi::SqliteDB db{path, schema_name};
        std::shared_ptr<dbi::DBAPI> db_conn(new dbi::DBAPI(db));
        db_conn->connect();

        return db_conn;
    }



    return nullptr;
}



std::string 
DBISvc::getSchemaProperty(const std::string& client,
                          const std::string& property) const {
    auto it_client = m_conf_schemas.find(client);
    if (it_client == m_conf_schemas.end()) {
        std::cerr << "Can't find schema for client " << client << std::endl;
        return "";
    }

    const auto& properties = it_client->second;

    auto it_property = properties.find(property);
    if (it_property == properties.end()) {
        std::cerr << "Can't find property " << property
                  << " in schema for client " << client << std::endl;
        return "";
    }

    return it_property->second;
}
