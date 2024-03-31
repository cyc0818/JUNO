/*
 * To initialize geometry before SniperMuster, we create an extra task.
 * At the same time, we have to create an extra thread for geant4 run manager.
 */

#ifndef SNIPER_GLOBAL_TASK_H
#define SNIPER_GLOBAL_TASK_H

#include "SniperKernel/Task.h"

class GlobalTask: public Task
{
 public:
    GlobalTask(const std::string& name);
    virtual ~GlobalTask();

    virtual bool run();

    virtual bool global_initialize();

    virtual bool global_finalize(); // shutdown the thread.
 private:
    static void* helper_start_thread(void* context);
    // create an extra thread
    pthread_t thread_id;
};

#endif
