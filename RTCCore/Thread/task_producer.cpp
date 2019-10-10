#include "task_producer.h"
#include "i_task_scheduler.h"
#include <QDebug>

namespace core {

TaskProducer::TaskProducer()
{

}

TaskProducer::~TaskProducer()
{

}

void TaskProducer::setTaskScheduler(std::shared_ptr<ITaskScheduler> scheduler)
{
    _scheduler = scheduler;
}

void TaskProducer::postTask(std::shared_ptr<ITask> task)
{
    if (addTask(task)) {
        notifyTaskArrived();
    } else {
        qDebug() << "add task failed";
        //exit(0);
    }
}

std::shared_ptr<ITask> TaskProducer::produceTask()
{
    std::shared_ptr<ITask> task;
    if (_taskQueue.try_dequeue(task)) {
        return task;
    }
    return nullptr;
}

void TaskProducer::removeTask(std::shared_ptr<ITask> /*task*/)
{

}

bool TaskProducer::addTask(std::shared_ptr<ITask> task)
{
    return _taskQueue.try_enqueue(task);
}

void TaskProducer::notifyTaskArrived()
{
    if (auto scheduler = _scheduler.lock()) {
        scheduler->onTaskArrived();
    }
}

void TaskProducer::onNextTask()
{
    notifyTaskArrived();
}

bool TaskProducer::empty()
{
    return _taskQueue.size_approx() == 0;
}

}
