#include <boost/python.hpp>
#include "boost/python/suite/indexing/indexing_suite.hpp" 
#include "boost/python/suite/indexing/vector_indexing_suite.hpp" 

#include <CondDB/IOV.h>

#include <vector>

void export_CondDB_IOV() {
    using namespace boost::python;

    class_<CondDB::IOV>("IOV", init<int64_t, std::string, std::string>((
                                                                        arg("since"),
                                                                        arg("payload_hash"),
                                                                        arg("object_type")
                                                                        )))
        .def("since", &CondDB::IOV::since,
             return_value_policy<copy_const_reference>())
        .def("payload_hash", &CondDB::IOV::payload_hash,
             return_value_policy<copy_const_reference>())
        .def("object_type", &CondDB::IOV::object_type,
             return_value_policy<copy_const_reference>())
        ;


    class_<std::vector<CondDB::IOV> >("IOVVec")
        .def(vector_indexing_suite< std::vector<CondDB::IOV> >())
        ;

}
