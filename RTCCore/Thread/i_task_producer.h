#pragma once

#include "i_task.h"
#include <memory>

namespace core {

class ITaskScheduler;

class ITaskProducer {
public:
    virtual ~ITaskProducer() {}

    virtual void setTaskScheduler(std::shared_ptr<ITaskScheduler> scheduler) = 0;
    virtual void postTask(std::shared_ptr<ITask> task) = 0;
    virtual std::shared_ptr<ITask> produceTask() = 0;
    virtual void removeTask(std::shared_ptr<ITask> task) = 0;
    virtual bool empty() = 0;
    virtual void onNextTask() = 0;
};

}
