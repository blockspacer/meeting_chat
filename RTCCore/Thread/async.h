#pragma once

#include <memory>
#include "async_task.h"
#include "task_scheduler_manager.h"

namespace core {

class Async {
public:
    static void dispatch(TaskFunc func, SchedulerType type)
    {
        std::shared_ptr<ITask> task = std::make_shared<AsyncTask>(func);
        auto scheduler = TaskSchedulerManager::instance()->scheduler(type);
        if (scheduler) {
            scheduler->postTask(task);
        }
    }

    static void dispatchToSerial(TaskFunc func)
    {
        dispatch(func, SchedulerType::DEFAULT_SERIAL);
    }

    static void dispatchToConcurrency(TaskFunc func)
    {
        dispatch(func, SchedulerType::DEFAULT_CONCURRENCY);
    }

    static void dispatchToMain(TaskFunc func)
    {
        dispatch(func, SchedulerType::MAIN);
    }
};

}
