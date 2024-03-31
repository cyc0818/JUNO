#include "boost/python.hpp"
#include "boost/noncopyable.hpp"
#include "boost/make_shared.hpp"

namespace bp = boost::python;
using namespace boost::python;

#include "RandomSvc/IRandomSvc.h"

struct IRandomSvcWrap: IRandomSvc, wrapper<IRandomSvc>
{
    double random() {
        return this->get_override("random")();
    }

    long getSeed() {
        return this->get_override("getSeed")();
    }
    void setSeed(long seed) {
        this->get_override("setSeed")(seed);
    }
};

#include "SniperKernel/SvcBase.h"
IRandomSvc*
rndmsvc(SvcBase* sb) {
    return dynamic_cast<IRandomSvc*>(sb);
}

#include "RandomSvc.h"

BOOST_PYTHON_MODULE(libRandomSvc)
{
    class_<IRandomSvcWrap, boost::shared_ptr<IRandomSvcWrap>, boost::noncopyable>
        ("IRandomSvc")
        .def("random", pure_virtual(&IRandomSvc::random))
    ;
    def("rndmsvc", rndmsvc, 
            return_value_policy<reference_existing_object>());

    class_<RandomSvc, bases<IRandomSvc, SvcBase>, boost::noncopyable >
        ("RandomSvc", init<std::string>())
    ;
}
