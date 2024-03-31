#include <boost/python.hpp>

#include "LocalFSRepo.h"

void export_CondDB_LocalFSRepo() {

    using namespace boost::python;

    class_<CondDB::LocalFSRepo, bases<CondDB::IRepo>, boost::noncopyable>
        ("LocalFSRepo", init<std::string, std::string>((arg("repo_name"),arg("repo_uri"))))
        ;

}
