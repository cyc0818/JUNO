#include "boost/python.hpp"
#include "boost/noncopyable.hpp"
#include "boost/make_shared.hpp"

namespace bp = boost::python;

#include "JunoTimer/JunoTimer.h"
#include "JunoTimer/IJunoTimerSvc.h"
struct IJunoTimerSvcWrap: IJunoTimerSvc, bp::wrapper<IJunoTimerSvc>
{
    JunoTimerPtr get(const std::string& name) {
        return this->get_override("get")(name);
    }

};
#include "SniperKernel/SvcBase.h"
#include "JunoTimerSvc.h"

BOOST_PYTHON_MODULE(libJunoTimer)
{
    bp::class_<IJunoTimerSvcWrap, boost::shared_ptr<IJunoTimerSvcWrap>, boost::noncopyable>
        ("IJunoTimerSvc")
        .def("get", bp::pure_virtual(&IJunoTimerSvc::get))
    ;
    bp::class_<JunoTimer, boost::shared_ptr<JunoTimer> >
        ("JunoTimer", bp::init<std::string>())
        .def("start", &JunoTimer::start)
        .def("stop", &JunoTimer::stop)
        .def("pause", &JunoTimer::pause)
        .def("resume", &JunoTimer::resume)
        .def("reset", &JunoTimer::reset)
        .def("name", &JunoTimer::name,
                bp::return_value_policy<bp::copy_const_reference>())
        .def("elapsed", &JunoTimer::elapsed)
        .def("mean", &JunoTimer::mean)
        .def("rms", &JunoTimer::rms)
        .def("number_of_measurements", &JunoTimer::number_of_measurements)
    ;
    bp::class_<JunoTimerSvc, bp::bases<IJunoTimerSvc, SvcBase>, boost::noncopyable >
        ("JunoTimerSvc", bp::init<std::string>())
    ;
}
