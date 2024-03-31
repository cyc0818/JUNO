#ifndef ParaDB_ParaTag_hh
#define ParaDB_ParaTag_hh

/*
 * Description:
 *   ParaTag represents the model in table ParaTag
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 */

#include <string>

namespace ParaDB {

    class ParaTag {
    public:

        ParaTag(const std::string& name_);

        const std::string& name() const;

    private:
        
        std::string m_name;

    };

}

#endif
