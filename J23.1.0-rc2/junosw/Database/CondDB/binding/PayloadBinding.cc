#include <boost/python.hpp>
#include "boost/python/suite/indexing/indexing_suite.hpp" 
#include "boost/python/suite/indexing/vector_indexing_suite.hpp" 

#include <CondDB/Payload.h>

#include <vector>

void export_CondDB_Payload() {
    using namespace boost::python;

    class_<CondDB::Payload>("Payload", 
                            init<std::string, std::string, std::string, std::string, std::string>
                            (
                             (arg("hash"),
                              arg("type"),
                              arg("path"),
                              arg("data"),
                              arg("streamer")
                              )))
        .def("hash", &CondDB::Payload::hash,
             return_value_policy<copy_const_reference>())
        .def("object_type", &CondDB::Payload::object_type,
             return_value_policy<copy_const_reference>())
        .def("path", &CondDB::Payload::path,
             return_value_policy<copy_const_reference>())
        .def("data", &CondDB::Payload::data,
             return_value_policy<copy_const_reference>())
        .def("streamer", &CondDB::Payload::streamer,
             return_value_policy<copy_const_reference>())
        ;

    class_<std::vector<CondDB::Payload> >("PayloadVec")
        .def(vector_indexing_suite< std::vector<CondDB::Payload> >())
        ;

}
