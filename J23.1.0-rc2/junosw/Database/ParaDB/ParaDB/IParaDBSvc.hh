#ifndef IParaDBSvc_hh
#define IParaDBSvc_hh

/*
 * Description:
 *
 * IParaDBSvc is the user interface to access parameter database. 
 *
 * The data model of ParaDB is similar to CondDB, but there is no IOV in ParaDB.
 * It consists of following:
 *   * ParaPayload: the metadata includes the path to the real parameters
 *   * ParaTag: the tag of a collection of payloads. 
 *   * ParaGTag: global tag of a collection of tags. 
 *
 * The software architecture is also similar to the CondDB.
 * 
 *  +-------+       +-----------+              +==========+
 *  | Users | ----> | ParaDBSvc | <----------- | Payload  | <--
 *  +-------+       +-----------+              +==========+    \
 *                     1. |                                    /
 *                        |                    +==========+ ---
 *                     n. v                    | ParaTag  |
 *                  +-----------+              +==========+ <--
 *                  | ParaRepo  |                              \
 *                  +-----------+              +==========+    /
 *                         \             /-->  | ParaGTag | ---
 *                          -------------      +==========+
 * 
 * Users could access a parameter by a key, such as 'Material.Acrylic.ABSLENGTH'.
 * Then the payload will be returned by ParaDBSvc. 
 *
 * Author: Tao Lin <lintao AT ihep.ac.cn>
 *
 */

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <ParaDB/IParaRepo.hh>

namespace ParaDB {

    class IParaDBSvc {

    public:
        virtual ~IParaDBSvc() = 0;

    public:
        virtual const std::vector<std::string>& getRepoNames() = 0;
        virtual std::shared_ptr<IParaRepo> getRepo(const std::string& name) = 0;

    public:
        // User need to check the parameter before use it. 
        virtual bool hasParameter(const std::string& name) = 0;
        virtual const ParaPayload& getParameter(const std::string& name) = 0;

    };

}

#endif
