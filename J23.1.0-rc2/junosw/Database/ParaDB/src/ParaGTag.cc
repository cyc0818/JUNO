#include <ParaDB/ParaGTag.hh>

namespace ParaDB {

ParaGTag::ParaGTag(const std::string& name_)
    : m_name(name_) {

}

const std::string& ParaGTag::name() const {
    return m_name;
}

}
