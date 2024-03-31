#ifndef ICondDBSvc_h
#define ICondDBSvc_h

/*
 * ICondDBSvc is the user interface used by end users.
 * 
 * All the conditions objects should be retrieved from this interface.
 * To simplify the user interface, user need to notify the CondDBSvc 
 * the current event timestamp.
 *
 * It also manages the metadata of conditions objects, such as:
 *   * Payload (not the real data)
 *   * IOV (internal of validity)
 *   * Tag (a set of IOV)
 *   * Global Tags (a set of Tags)
 *
 * In order to support different persistencies or data stores, 
 * we use Repo to represent one type of data store.
 * The implementation of CondDBSvc can contain multiple Repos.
 * The Repo can be:
 *   * LocalFSRepo: based on local file system
 *   * MySQLRepo: based on MySQL client
 *   * RestRepo: based on RESTful client
 *
 * Each type of conditions object is mapping to one or more Tags.
 * 
 * The design of CondDB
 *
 *  +----------------+ access +------------+
 *  |Users/Developers| _____\ | Conditions |  <-- convert -.  Each type of payloads 
 *  |   Algorithm    |      / |  objects   |               |    will be converted
 *  +----------------+        +------------+          +=========+
 *                                   ^                | Payload | <-----
 *                                   |                +=========+      /
 *                           +---------------+                        /
 *                           |  C o n d D B  |          +=====+ ------
 *                           | S e r v i c e |          | IoV | <---
 *                           +---------------+          +=====+     \
 *                                   | 1.                           /
 *                                   |                  +=====+ ----
 *                                   v n.               | Tag | 
 *                            +-------------+           +=====+ <-----    
 *                            |   R e p o   |                         \
 *                            +-------------+       +============+    /
 *                                   \          --> | Global Tag | ---
 *                                    ~--------/    +============+
 *                            
 */

#include <map>
#include <vector>
#include <string>
#include <CondDB/IConditionObject.h>

namespace CondDB {

class IRepo;

class ICondDBSvc {

public:

    virtual ~ICondDBSvc() = 0;

public:
    // Following are Repos
    virtual const std::vector<std::string>& getRepoNames() = 0;
    virtual IRepo* getRepo(const std::string& name) = 0;

public:
    // Following are metadata 
    //   The metadata will be merged according to the priorities
    //   of these repos.

    // Return all IOVs of specific object type.
    
    // set current time/run and update the registered 
    // payloads and condition objects.
    virtual bool setCurrent(const int64_t& cur) = 0;

public:
    // Following are conditions objects
    // To define several instances, the cond_obj_path can be:
    //    Type/name
    // For an example:
    //    CD.PMT.Prop/mypmt
    virtual bool declCondObj(const std::string& cond_obj_path,
                             IConditionObject& cond_obj) = 0;
                     
};

}

#endif
