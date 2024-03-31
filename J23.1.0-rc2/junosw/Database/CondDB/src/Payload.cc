#include "CondDB/Payload.h"

namespace CondDB {

    Payload::Payload(const std::string& hash_,
                     const std::string& object_type_,
                     const std::string& path_,
                     const std::string& data_,
                     const std::string& streamer_)
        : m_hash(hash_), m_object_type(object_type_)
        , m_path(path_)
        , m_data(data_)
        , m_streamer(streamer_)
    {

    }

    const std::string& Payload::hash() const
    {
        return m_hash;
    }

    const std::string& Payload::object_type() const
    {
        return m_object_type;
    }

    const std::string& Payload::path() const
    {
        return m_path;
    }

    const std::string& Payload::data() const
    {
        return m_data;
    }

    const std::string& Payload::streamer() const 
    {
        return m_streamer;
    }


} // namespace CondDB


