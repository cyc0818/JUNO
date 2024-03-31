#include "CondDB/Tag.h"

namespace CondDB {

    Tag::Tag(const std::string& name_,
             const std::string& type_)
        : m_name(name_), m_type(type_)
    {

    }

    const std::string& Tag::name() const
    {
        return m_name;
    }

    const std::string& Tag::type() const
    {
        return m_type;
    }


} // namespace CondDB


