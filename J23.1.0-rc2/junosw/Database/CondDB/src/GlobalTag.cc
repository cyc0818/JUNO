#include "CondDB/GlobalTag.h"

namespace CondDB {

    GlobalTag::GlobalTag(const std::string& name_)
        : m_name(name_)
    {

    }

    const std::string& GlobalTag::name() const
    {
        return m_name;
    }


} // namespace CondDB


