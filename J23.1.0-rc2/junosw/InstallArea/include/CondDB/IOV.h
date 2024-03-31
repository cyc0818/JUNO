#ifndef CondDB_IOV_h
#define CondDB_IOV_h

#include <string>
// #include <cstdint>
#include <inttypes.h>

namespace CondDB {

class IRepo;

class IOV {
public:

    IOV(const int64_t& since_,
        const std::string& payload_hash_,
        const std::string& object_type_);

    const int64_t& since() const;
    const std::string& payload_hash() const;
    const std::string& object_type() const;

    IRepo* repo(); // getter
    IRepo* repo() const;
    bool repo(IRepo*); // setter

    bool operator==(const IOV& rhs) {
        return this == &rhs;
    }


private:

    int64_t m_since;
    std::string m_payload_hash;
    std::string m_object_type;

    // Ref to IRepo;
    IRepo* m_repo;
};

}

#endif
