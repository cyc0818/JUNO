#include <boost/python.hpp>
#include "boost/python/suite/indexing/indexing_suite.hpp" 
#include "boost/python/suite/indexing/vector_indexing_suite.hpp" 

#include <CondDB/GlobalTag.h>

#include <vector>

void export_CondDB_GlobalTag() {
    using namespace boost::python;

    class_<CondDB::GlobalTag>("GlobalTag", init<std::string>((
                                                              arg("name")
                                                              )))
        .def("name", &CondDB::GlobalTag::name,
                      return_value_policy<copy_const_reference>())
        ;

    class_<std::vector<CondDB::GlobalTag> >("GlobalTagVec")
        .def(vector_indexing_suite< std::vector<CondDB::GlobalTag> >())
        ;
}
