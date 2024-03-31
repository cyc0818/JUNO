#include <boost/python.hpp>
#include "DetSimMT/GlobalTask.h"
#include "SniperKernel/DeclareDLE.h"
#include "SniperKernel/SniperLog.h"
#include <pthread.h>
#include "TROOT.h"
pthread_mutex_t shutdown_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t shutdown_cv;
//static bool shutdown_flag = false;
bool shutdown_flag = false;

// let global task thread tell main, initialization is done.
pthread_mutex_t main_continue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t main_continue_cv;
bool main_continue_flag = false;


SNIPER_DECLARE_DLE(GlobalTask);

GlobalTask::GlobalTask(const std::string& name)
    : Task(name)
{

}

GlobalTask::~GlobalTask()
{

}

bool
GlobalTask::run()
{
    // initialize ROOT
    ROOT::EnableThreadSafety();

    // initialize mutex and cond val
    pthread_mutex_init(&shutdown_mutex, NULL);
    pthread_cond_init (&shutdown_cv, NULL);

    pthread_mutex_init(&main_continue_mutex, NULL);
    pthread_cond_init (&main_continue_cv, NULL);


    // start a new thread
    pthread_create(&thread_id,
                   NULL,
                   &GlobalTask::helper_start_thread,
                   this);

    // let main wait for global task's initialization
    pthread_mutex_lock(&main_continue_mutex);
    while(!main_continue_flag) {
	std::cout << "WAIT to continue" << std::endl;
	pthread_cond_wait(&main_continue_cv, &main_continue_mutex);
    }
    pthread_mutex_unlock(&main_continue_mutex);

    return true;
}

bool
GlobalTask::global_finalize()
{
    // ok, shutdown (invoke in master thread)

    LogInfo << "waiting to shutdown. " << std::endl;
    LogDebug << "pthread_mutex_lock(&shutdown_mutex)" << std::endl;
    pthread_mutex_lock(&shutdown_mutex);
    LogDebug << "pthread_mutex_lock(&shutdown_mutex) done" << std::endl;
    shutdown_flag = true;
    LogDebug << "pthread_cond_signal(&shutdown_cv)" << std::endl;
    pthread_cond_signal(&shutdown_cv);
    LogDebug << "pthread_mutex_unlock(&shutdown_mutex)" << std::endl;
    pthread_mutex_unlock(&shutdown_mutex);
    LogInfo << "shutdown: false->true" << std::endl;
    pthread_join(thread_id, NULL);
    LogInfo << "shutdown. " << std::endl;
    return true;
}

bool
GlobalTask::global_initialize()
{
    // invoke the real work
    if ( m_snoopy.config() ) {
        LogInfo << "config() done." << std::endl;
	LogInfo << "current status: " << static_cast<unsigned short>(m_snoopy.state()) << std::endl;
        if ( m_snoopy.initialize() ) {
            LogInfo << "initialize() done." << std::endl;
	}
    }
    return true;
}

void*
GlobalTask::helper_start_thread(void* context)
{
    GlobalTask* self = (GlobalTask*)context;
    self->global_initialize();

    // tell main, continue to work
    pthread_mutex_lock(&main_continue_mutex);
    main_continue_flag = true;
    pthread_cond_signal(&main_continue_cv);
    pthread_mutex_unlock(&main_continue_mutex);


    // wait until users tell us to shutdown this thread
    //
    pthread_mutex_lock(&shutdown_mutex);
    while(!shutdown_flag) {
	std::cout << "WAIT to shutdown" << std::endl;
	pthread_cond_wait(&shutdown_cv, &shutdown_mutex);
    }
    pthread_mutex_unlock(&shutdown_mutex);
    std::cout << "thread is shutdown." << std::endl;

    // do the real finalize work
    self->m_snoopy.finalize();
    return NULL;
}
