#include <boost/python.hpp>
#include "boost/python/suite/indexing/indexing_suite.hpp" 
#include "boost/python/suite/indexing/vector_indexing_suite.hpp" 

#include "tests/Testing_CD_LS_MatProp.h"

#include <vector>

void export_CondDB_Testing_CD_LS_MatProp() {
    using namespace boost::python;

    class_<CondDB::Testing::CD_LS_MatProp,
        bases<CondDB::IConditionObject>, boost::noncopyable>
        ("CD_LS_MatProp", init<>())
        .def_readonly("light_yield", &CondDB::Testing::CD_LS_MatProp::light_yield)
        .def_readonly("refractive_energy", &CondDB::Testing::CD_LS_MatProp::refractive_energy)
        .def_readonly("refractive_index", &CondDB::Testing::CD_LS_MatProp::refractive_index)
        ;
    class_<std::vector<double> >("DoubleVec")
        .def(vector_indexing_suite< std::vector<double> >())
        ;

}
