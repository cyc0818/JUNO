#ifndef CondDB_IRepo_h
#define CondDB_IRepo_h

/*
 * IRepo (repository) manages the metadata of conditions database.
 * The implementation can be different according to the types of backends.
 *
 * Important methods:
 *
 *   * findGlobalTags(gt_name): 
 *
 *         return a list of matched GlobalTags if found. 
 *
 *   * findTags(gt):
 *
 *         return a list of Tags if found.
 *
 *   * findIOVs(tag):
 *
 *         return a list of IOVs if found.
 * 
 *   * findPayloads(iov):
 *
 *         return a list of Payloads if found. 
 *         According to the design, one iov associates with one payload.
 *
 * For convenience, user can also specify the name (string) for all methods.
 * Also, user can specify the time range.
 *
 */

#include <vector>
#include "CondDB/GlobalTag.h"
#include "CondDB/Tag.h"
#include "CondDB/IOV.h"
#include "CondDB/Payload.h"

namespace CondDB {

    class IRepo {

    public:
        // Repo metadata:
        virtual const std::string& name() = 0;
        virtual const std::string& type() = 0;
        virtual const std::string& uri() = 0;
    public:
        virtual ~IRepo() = 0;

    public:
        // findGlobalTags:
        // return: a list of global tags
        // param:
        //     gt_name: GT name or names
        virtual const std::vector<GlobalTag>& 
            findGlobalTags(const std::string& gt_name) = 0;
        
        // findTags:
        // return: a list of tags
        // param:
        //     gt: GlobalTag
        virtual const std::vector<Tag>&
            findTags(const GlobalTag& gt) = 0;

        // findIOVs:
        // return: a list of iovs
        // param:
        //     tag: Tag
        virtual const std::vector<IOV>&
            findIOVs(const Tag& tag) = 0;

        // findPayloads:
        // return: a list of payloads
        // param:
        //     iov: IOV
        virtual const std::vector<Payload>&
            findPayloads(const IOV& iov) = 0;
    }; // class IRepo


} // namespace CondDB

#endif
