#include <boost/python.hpp>
#include "boost/python/suite/indexing/indexing_suite.hpp" 
#include "boost/python/suite/indexing/vector_indexing_suite.hpp" 

#include <CondDB/Tag.h>

#include <vector>

void export_CondDB_Tag() {
    using namespace boost::python;

    class_<CondDB::Tag>("Tag", init<std::string, std::string>((
                                                               arg("name"),
                                                               arg("type")
                                                               )))
        .def("name", &CondDB::Tag::name,
             return_value_policy<copy_const_reference>())
        .def("type", &CondDB::Tag::type,
             return_value_policy<copy_const_reference>())
        ;

    class_<std::vector<CondDB::Tag> >("TagVec")
        .def(vector_indexing_suite< std::vector<CondDB::Tag> >())
        ;


}
