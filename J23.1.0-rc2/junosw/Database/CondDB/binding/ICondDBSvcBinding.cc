#include <boost/python.hpp>

#include <CondDB/ICondDBSvc.h>
#include <CondDB/IRepo.h>

namespace bp = boost::python;

struct ICondDBSvcWrap: CondDB::ICondDBSvc, bp::wrapper<CondDB::ICondDBSvc> {

    const std::vector<std::string>&
    getRepoNames() {
        return this->get_override("getRepoNames")();
    }

    CondDB::IRepo*
    getRepo(const std::string& name) {
        return this->get_override("getRepo")(name);
    }

    bool
    setCurrent(const int64_t& cur) {
        return this->get_override("setCurrent")(cur);
    }

    bool
    declCondObj(const std::string& cond_obj_path, CondDB::IConditionObject& cond_obj) {
        return this->get_override("declCondObj")(cond_obj_path, cond_obj);
    }

};

void export_CondDB_ICondDBSvc() {
    using namespace boost::python;

    class_<ICondDBSvcWrap, boost::noncopyable>("ICondDBSvc", no_init)
        .def("getRepoNames", pure_virtual(&CondDB::ICondDBSvc::getRepoNames),
             return_value_policy<copy_const_reference>())
        .def("getRepo", pure_virtual(&CondDB::ICondDBSvc::getRepo),
             return_value_policy<reference_existing_object>())
        .def("setCurrent", pure_virtual(&CondDB::ICondDBSvc::setCurrent))
        .def("declCondObj", pure_virtual(&CondDB::ICondDBSvc::declCondObj))
        ;
}
