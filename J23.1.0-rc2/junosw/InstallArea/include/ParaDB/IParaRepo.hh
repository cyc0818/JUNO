#ifndef ParaDB_IParaRepo_hh
#define ParaDB_IParaRepo_hh

/*
 * Description:
 *
 * IParaRepo (repository) manages the metadata of parameter database. 
 * The implementation could be different according to the backends.
 *
 * Methods:
 *
 *   * findGlobalTags(gtname)
 *
 *       return a list of matched ParaGTag if found.
 *
 *   * findTags(gt)
 *
 *       return a list of tags by using the ParaGTag.
 *
 *   * findPayloads(tag)
 *
 *       return a list of payloads by using the ParaTag.
 * 
 * The ParaDBSvc will then manage all the payloads.
 *
 * Author: Tao Lin <lintao AT ihep.ac.cn>
 *
 */

#include <vector>
#include "ParaDB/ParaGTag.hh"
#include "ParaDB/ParaTag.hh"
#include "ParaDB/ParaPayload.hh"

namespace ParaDB {

    class IParaRepo {
    public:
        virtual const std::string& name() = 0;
        virtual const std::string& type() = 0;
        virtual const std::string& uri() = 0;

    public:
        virtual ~IParaRepo() = 0;

    public:
        virtual const std::vector<ParaGTag>&
        findGlobalTags(const std::string& para_gtag_name) = 0;

        virtual const std::vector<ParaTag>&
        findTags(const ParaGTag&) = 0;

        virtual const std::vector<ParaPayload>&
        findPayloads(const ParaTag&) = 0;
};

}

#endif
