#pragma once

#include "memory"
#include "i_task_consumer.h"
#include "Utils/concurrentqueue.h"
#include "Utils/blockingconcurrentqueue.h"
#include <atomic>

namespace core {

class AbstractTaskConsumer : public ITaskConsumer
{
public:
    AbstractTaskConsumer();
    ~AbstractTaskConsumer() override;

    void setTaskScheduler(std::shared_ptr<ITaskScheduler> scheduler) override;
    TaskConsumerType type() override;
    virtual void takeTask(std::shared_ptr<ITask> task) override;
    int64_t threadId() override;

protected:
    TaskConsumerType _type;
    std::weak_ptr<ITaskScheduler> _scheduler;
    moodycamel::BlockingConcurrentQueue<std::shared_ptr<ITask>> _pendingQueue;
    std::atomic<int64_t> _threadId;
};

}
