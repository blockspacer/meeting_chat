#include "concurrency_task_consumer.h"
#include "i_task.h"
#include "i_task_scheduler.h"
#include <QDebug>

namespace core {

TaskRunner::TaskRunner(std::shared_ptr<ITaskScheduler> scheduler, std::shared_ptr<ITask> task)
    : _scheduler(scheduler)
    , _task(task)
{
    this->setAutoDelete(true);
}

void TaskRunner::run()
{
    if (_task && !_task->canceled()) {
        _task->execute();
    }
    if (auto scheduler = _scheduler.lock()) {
        scheduler->onTaskCompleted(_task);
    }
}

ConcurrencyTaskConsumer::ConcurrencyTaskConsumer()
{
    _pool.setMaxThreadCount(4);
    _type = TaskConsumerType::CONCURRENCY;
}

void ConcurrencyTaskConsumer::start()
{

}

void ConcurrencyTaskConsumer::stop()
{
    _pool.waitForDone(1000);
}

void ConcurrencyTaskConsumer::notifyTaskArrived()
{
    runTask();
}

bool ConcurrencyTaskConsumer::canTakeTask()
{
    return true;
}

void ConcurrencyTaskConsumer::runTask()
{
    std::shared_ptr<ITask> task;
    while (_pendingQueue.try_dequeue(task)) {
        if (auto scheduler = _scheduler.lock()) {
            TaskRunner* runner = new TaskRunner(scheduler, task);
            //qDebug() << "activeThreadCount:" << _pool.activeThreadCount();
            _pool.start(runner);
        }
    }
}

}
