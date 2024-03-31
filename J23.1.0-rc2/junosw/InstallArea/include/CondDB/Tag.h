#ifndef CondDB_Tag_h
#define CondDB_Tag_h

#include <string>

namespace CondDB {

class Tag {
public:

    Tag(const std::string& name_, 
        const std::string& type_);

    const std::string& name() const;
    const std::string& type() const;

    bool operator==(const Tag& rhs) {
        return this == &rhs;
    }

private:

    std::string m_name;
    std::string m_type;
};

}

#endif
