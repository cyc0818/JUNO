#include <boost/python.hpp>

#include <CondDB/IRepo.h>

namespace bp = boost::python;

struct IRepoWrap: CondDB::IRepo, bp::wrapper<CondDB::IRepo> {

    const std::string& name() {
        return this->get_override("name")();
    }

    const std::string& type() {
        return this->get_override("type")();
    }

    const std::string& uri() {
        return this->get_override("uri")();
    }

    const std::vector<CondDB::GlobalTag>& 
    findGlobalTags(const std::string& gt_name) {
        return this->get_override("findGlobalTags")(gt_name);
    }

    const std::vector<CondDB::Tag>&
    findTags(const CondDB::GlobalTag& gt) {
        return this->get_override("findTags")(gt);
    }

    const std::vector<CondDB::IOV>&
    findIOVs(const CondDB::Tag& tag) {
        return this->get_override("findIOVs")(tag);
    }

    const std::vector<CondDB::Payload>&
    findPayloads(const CondDB::IOV& iov) {
        return this->get_override("findPayloads")(iov);
    }

};

void export_CondDB_IRepo() {
    using namespace boost::python;

    class_<IRepoWrap, boost::noncopyable>("IRepo", no_init)
        .def("name", pure_virtual(&CondDB::IRepo::name),
             return_value_policy<copy_const_reference>())
        .def("type", pure_virtual(&CondDB::IRepo::type),
             return_value_policy<copy_const_reference>())
        .def("uri", pure_virtual(&CondDB::IRepo::uri),
             return_value_policy<copy_const_reference>())
        .def("findGlobalTags", pure_virtual(&CondDB::IRepo::findGlobalTags),
             return_value_policy<copy_const_reference>())
        .def("findTags", pure_virtual(&CondDB::IRepo::findTags),
             return_value_policy<copy_const_reference>())
        .def("findIOVs", pure_virtual(&CondDB::IRepo::findIOVs),
             return_value_policy<copy_const_reference>())
        .def("findPayloads", pure_virtual(&CondDB::IRepo::findPayloads),
             return_value_policy<copy_const_reference>())
        ;
}
