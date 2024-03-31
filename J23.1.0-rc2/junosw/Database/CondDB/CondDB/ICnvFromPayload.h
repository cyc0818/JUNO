#ifndef CondDB_ICnvFromPayload_h
#define CondDB_ICnvFromPayload_h

#include <CondDB/Payload.h>

namespace CondDB {

struct ICnvFromPayload {

    virtual bool cnv(const Payload& payload) = 0;

}; // struct ICnvFromPayload

}; // namespace CondDB

#endif
