#ifndef GlobalTag_h
#define GlobalTag_h

#include <string>

namespace CondDB {

class GlobalTag {
public:

    GlobalTag(const std::string& name_);

    const std::string& name() const;

    bool operator==(const GlobalTag& rhs) {
        return this == &rhs;
    }

private:

    std::string m_name;
};

}

#endif
