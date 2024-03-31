#include <ParaDB/ParaPayload.hh>

namespace ParaDB {

    ParaPayload::ParaPayload(const std::string& object_type_,
                             const std::string& property_,
                             const std::string& fs_,
                             const std::string& path_,
                             const std::string& version_)
        : m_object_type(object_type_), m_property(property_),
          m_fs(fs_), m_path(path_), m_version(version_) {

    }

    const std::string& ParaPayload::object_type() const {
        return m_object_type;
    }

    const std::string& ParaPayload::property() const {
        return m_property;
    }

    const std::string& ParaPayload::fs() const {
        return m_fs;
    }

    const std::string& ParaPayload::path() const {
        return m_path;
    }

    const std::string& ParaPayload::version() const {
        return m_version;
    }

    const ParaPayload& ParaPayload::null_payload() {
        static ParaPayload _null{"null_object_type", 
                                 "null_property", 
                                 "null_fs",
                                 "null_path",
                                 "null_version"};
        return _null;
    }

}
