#include "CondDB/IOV.h"

namespace CondDB {

    IOV::IOV(const int64_t& since_, 
             const std::string& payload_hash_,
             const std::string& object_type_)
        : m_since(since_), m_payload_hash(payload_hash_)
        , m_object_type(object_type_)
        , m_repo(NULL)
    {

    }

    const int64_t& IOV::since() const
    {
        return m_since;
    }

    const std::string& IOV::payload_hash() const
    {
        return m_payload_hash;
    }

    const std::string& IOV::object_type() const
    {
        return m_object_type;
    }

    IRepo* IOV::repo() {
        return m_repo;
    }

    IRepo* IOV::repo() const {
        return m_repo;
    }

    bool IOV::repo(IRepo* repo_) {
        m_repo = repo_;
        return true;
    }

} // namespace CondDB


