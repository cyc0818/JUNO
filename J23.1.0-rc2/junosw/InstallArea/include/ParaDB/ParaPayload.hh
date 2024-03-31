#ifndef ParaDB_ParaPayload_hh
#define ParaDB_ParaPayload_hh

/*
 * Description:
 *   ParaPayload represents the model in table ParaPayload
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 */

#include <string>

namespace ParaDB {

    class ParaPayload {

    public:

        ParaPayload(const std::string& object_type_,
                    const std::string& property_,
                    const std::string& fs_,
                    const std::string& path_,
                    const std::string& version_);

        const std::string& object_type() const;
        const std::string& property() const;
        const std::string& fs() const;

        const std::string& path() const;
        const std::string& version() const;

    public:
        // return a null object
        static const ParaPayload& null_payload();

    private:

        std::string m_object_type;
        std::string m_property;
        std::string m_fs;
        std::string m_path;
        std::string m_version;

    };

}

#endif
