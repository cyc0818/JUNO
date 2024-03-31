#ifndef ParaDB_ParaGTag_hh
#define ParaDB_ParaGTag_hh

/*
 * Description:
 *   ParaGTag represents the model in table ParaGTag
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 */

#include <string>

namespace ParaDB {

    class ParaGTag {
    public:

        ParaGTag(const std::string& name_);

        const std::string& name() const;

    private:
        
        std::string m_name;

    };

}

#endif
