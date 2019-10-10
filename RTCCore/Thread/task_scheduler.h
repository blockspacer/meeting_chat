#pragma once

#include "i_task_scheduler.h"
#include <memory>

namespace core {

class ITaskProducer;
class ITaskConsumer;

class TaskScheduler : public ITaskScheduler, public std::enable_shared_from_this<TaskScheduler>
{
public:
    TaskScheduler(std::shared_ptr<ITaskProducer> producer, std::shared_ptr<ITaskConsumer> consumer);
    ~TaskScheduler() override;

    void init() override;
    void postTask(std::shared_ptr<ITask> task) override;
    void onTaskArrived() override;
    void onTaskCompleted(std::shared_ptr<ITask> task) override;
    void stop() override;
    int64_t threadId() override;

private:
    std::shared_ptr<ITaskProducer> _producer;
    std::shared_ptr<ITaskConsumer> _consumer;
};

}
