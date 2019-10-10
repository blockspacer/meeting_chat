#pragma once

#include <memory>

namespace core {

enum TaskConsumerType : int {
    SERIAL,
    CONCURRENCY
};

class ITask;
class ITaskScheduler;

class ITaskConsumer
{
public:
    virtual ~ITaskConsumer() {}

    virtual void setTaskScheduler(std::shared_ptr<ITaskScheduler> scheduler) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool canTakeTask() = 0;
    virtual void takeTask(std::shared_ptr<ITask> task) = 0;
    virtual TaskConsumerType type() = 0;
    virtual int64_t threadId() = 0;

protected:
    virtual void notifyTaskArrived() = 0;
};

}
