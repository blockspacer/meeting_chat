#include "abstract_task_consumer.h"
#include "i_task_scheduler.h"
#include "i_task.h"

namespace core {

AbstractTaskConsumer::AbstractTaskConsumer()
{
    _threadId = -1;
}

AbstractTaskConsumer::~AbstractTaskConsumer()
{

}

void AbstractTaskConsumer::setTaskScheduler(std::shared_ptr<ITaskScheduler> scheduler)
{
    _scheduler = scheduler;
}

TaskConsumerType AbstractTaskConsumer::type()
{
    return _type;
}

void AbstractTaskConsumer::takeTask(std::shared_ptr<ITask> task)
{
    if (task) {
        _pendingQueue.enqueue(task);
        notifyTaskArrived();
    }
}

int64_t AbstractTaskConsumer::threadId()
{
    return _threadId;
}

}

