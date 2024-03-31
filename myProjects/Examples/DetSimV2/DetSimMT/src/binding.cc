#include <boost/python.hpp>
#include <SniperKernel/Task.h>
#include <DetSimMT/GlobalTask.h>

namespace bp = boost::python;

struct GlobalTaskWrap: GlobalTask, bp::wrapper<GlobalTask>
{
    GlobalTaskWrap(const std::string& name)
        : GlobalTask(name)
    {
    }

    bool run() {
        if ( bp::override f = this->get_override("run")) return f();
        return GlobalTask::run();
    }

    bool default_run() {
	return this->GlobalTask::run();
    }

    bool global_finalize() {
        if ( bp::override f = this->get_override("global_finalize")) return f();
        return GlobalTask::global_finalize();
    }

    bool default_global_finalize() {
	return this->GlobalTask::global_finalize();
    }
};

void export_Sniper_GlobalTask() {
    using namespace boost::python;
    class_<GlobalTaskWrap, bases<Task>, boost::noncopyable>
	("GlobalTask", init<const std::string&>())
	.def("run", &GlobalTask::run, &GlobalTaskWrap::default_run)
	.def("global_finalize", &GlobalTask::global_finalize, &GlobalTaskWrap::default_global_finalize)
	;
}

BOOST_PYTHON_MODULE(libDetSimMT)
{
    export_Sniper_GlobalTask();
}
