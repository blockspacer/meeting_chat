#pragma once

#include <memory>

namespace core {

class ITaskProducer;
class ITaskConsumer;
class ITask;

class ITaskScheduler {
public:
    virtual ~ITaskScheduler() {}

    virtual void init() = 0;
    virtual void postTask(std::shared_ptr<ITask> task) = 0;
    virtual void onTaskArrived() = 0;
    virtual void onTaskCompleted(std::shared_ptr<ITask> task) = 0;
    virtual void stop() = 0;
    virtual int64_t threadId() = 0;
};

}
