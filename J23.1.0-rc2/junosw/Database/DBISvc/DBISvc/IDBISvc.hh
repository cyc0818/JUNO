#ifndef IDBISvc_hh
#define IDBISvc_hh

// This service maintains database connections, which are used by the clients.
// Therefore the clients don't need to take care of the database connection any more. 
//
// In order to configure connection for the clients, the clients need to invoke the 
// method `getConnection` with a customized client name. 
//
// The service will find a connection for the corresponding client name according to
// the configuration. 
//
// The configuration will be maintained in YAML format file, with following rules:
//
// connections:
//   connection_name_1:
//     key: value
//   connection_name_2:
//     key: value
// clients:
//   mcparamsvc:
//     - connection_name_1
//     - connection_name_2
//
// The corresponding python object will be:
// {
//   'connections': {
//     'connection_name_1': {'key': 'value'}
//     'connection_name_2': {'key': 'value'}
//   },
//   'clients': {
//     'mcparamsvc': ['connection_name_1', 'connection_name_2']
//   }
// }
//
// Author:
//   Tao Lin <lintao AT ihep.ac.cn>, 12 Nov 2022

#include <dbi/dbi.h>
#include <memory>

class IDBISvc {

public:
    virtual std::shared_ptr<dbi::DBAPI> getConnection(const std::string& client) = 0;

    // query schema
    virtual std::string getSchemaProperty(const std::string& client,
                                          const std::string& property) const = 0;
};

#endif
