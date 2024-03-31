#include <boost/python.hpp>

#include "CondDBSvc.h"

void export_CondDB_CondDBSvc() {
    using namespace boost::python;

    class_<CondDBSvc, bases<CondDB::ICondDBSvc, SvcBase>, boost::noncopyable>
        ("CondDBSvc", init<std::string>())
         ;
}
