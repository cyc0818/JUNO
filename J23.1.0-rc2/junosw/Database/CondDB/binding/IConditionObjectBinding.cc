#include <boost/python.hpp>

#include <CondDB/IConditionObject.h>
#include <CondDB/ICnvFromPayload.h>

namespace bp = boost::python;
struct IConditionObjectWrap: CondDB::IConditionObject, 
                             bp::wrapper<CondDB::IConditionObject> {

    CondDB::ICnvFromPayload* 
    getCnv(const std::string& streamer) {
        return this->get_override("getCnv")(streamer);
    }

};

void export_CondDB_IConditionObject() {

    using namespace boost::python;

    class_<IConditionObjectWrap, boost::noncopyable>
        ("IConditionObject", init<>())
        .def("getCnv", pure_virtual(&CondDB::IConditionObject::getCnv),
             return_value_policy<reference_existing_object>())
        ;
}
