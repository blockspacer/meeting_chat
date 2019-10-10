#pragma once

#include "i_task_producer.h"
#include <string>
#include "Utils/concurrentqueue.h"

namespace core {

class ITaskScheduler;

class TaskProducer : public ITaskProducer, public std::enable_shared_from_this<TaskProducer> {
public:
    TaskProducer();

    ~TaskProducer() override;

    void setTaskScheduler(std::shared_ptr<ITaskScheduler> scheduler) override;
    void postTask(std::shared_ptr<ITask> task) override;
    std::shared_ptr<ITask> produceTask() override;
    void removeTask(std::shared_ptr<ITask> task) override;
    void onNextTask() override;
    bool empty() override;

private:
    bool addTask(std::shared_ptr<ITask> task);
    void notifyTaskArrived();

private:
    std::weak_ptr<ITaskScheduler> _scheduler;
    moodycamel::ConcurrentQueue<std::shared_ptr<ITask>> _taskQueue;
};

}
