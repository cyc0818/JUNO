#include <boost/python.hpp>

#include <CondDB/ICnvFromPayload.h>

namespace bp = boost::python;
struct ICnvFromPayloadWrap: CondDB::ICnvFromPayload, 
                            bp::wrapper<CondDB::ICnvFromPayload> {

    bool
    cnv(const CondDB::Payload& payload) {
        return this->get_override("cnv")(payload);
    }

};

void export_CondDB_ICnvFromPayload() {

    using namespace boost::python;

    class_<ICnvFromPayloadWrap, boost::noncopyable>
        ("ICnvFromPayload", init<>())
        .def("cnv", pure_virtual(&CondDB::ICnvFromPayload::cnv))
        ;

}
