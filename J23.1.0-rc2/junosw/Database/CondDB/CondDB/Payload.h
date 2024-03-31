#ifndef CondDB_Payload_h
#define CondDB_Payload_h

#include <string>
// #include <cstdint>
#include <inttypes.h>

namespace CondDB {

class Payload {
public:

    Payload(const std::string& hash_, 
            const std::string& object_type_,
            const std::string& path_,
            const std::string& data_,
            const std::string& streamer_);

    const std::string& hash() const;
    const std::string& object_type() const;
    const std::string& path() const;
    const std::string& data() const;
    const std::string& streamer() const;

    bool operator==(const Payload& rhs) {
        return this == &rhs;
    }

private:

    std::string m_hash;
    std::string m_object_type;
    std::string m_path;
    std::string m_data;
    std::string m_streamer;
};

}

#endif
