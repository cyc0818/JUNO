#include <ParaDB/ParaTag.hh>

namespace ParaDB {

ParaTag::ParaTag(const std::string& name_)
    : m_name(name_) {

}

const std::string& ParaTag::name() const {
    return m_name;
}

}
