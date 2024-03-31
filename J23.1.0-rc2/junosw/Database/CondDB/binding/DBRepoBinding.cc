#include <boost/python.hpp>

#include "DBRepo.h"

void export_CondDB_DBRepo() {

    using namespace boost::python;

    // TODO: need to support DBI in the future
    // class_<CondDB::DBRepo, bases<CondDB::IRepo>, boost::noncopyable>
    //     ("DBRepo", init<std::string, std::string, std::list<std::string>>((arg("repo_name"),arg("repo_uri"),arg("repo_proxies"))))
    //     ;

}
