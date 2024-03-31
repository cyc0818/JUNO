#ifndef CondDB_IConditionObject_h
#define CondDB_IConditionObject_h

/*
 * IConditionObject is base class of condition objects.
 *
 * To support different backend of payload, the developers need to
 * implement their own converters which convert from one type of 
 * payload to the same condition objects.
 *
 *                         .- Cnv B -- Payload B
 *     Condition Objects <--- Cnv A -- Payload A
 *                         `- Cnv C -- Payload C
 *
 * Because the types of payloads are unknown during compilation,
 * the condition database service is responsible to get the correct
 * converter according to the streamer info in Payload.
 * However, developers must register the correct converters.
 */

#include <CondDB/ICnvFromPayload.h>

namespace CondDB {

    struct IConditionObject {

        // Note: Cnv is maintained by CondObj.
        virtual ICnvFromPayload* getCnv(const std::string& streamer) = 0;

    }; // struct IConditionObject

}; // namespace CondDB

#endif
