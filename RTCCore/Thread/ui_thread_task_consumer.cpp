#include "ui_thread_task_consumer.h"
#include "task_scheduler.h"
#include "i_task.h"
#include <QThread>
#include "concurrency_task_consumer.h"
#include <QDebug>

namespace core {

UITaskWorker UIThreadTaskConsumer::_gUITaskWorker;

UIThreadTaskConsumer::UIThreadTaskConsumer()
{
    _type = TaskConsumerType::SERIAL;

    connect(this, &UIThreadTaskConsumer::post, &_gUITaskWorker, &UITaskWorker::processTask, Qt::QueuedConnection);
}

UIThreadTaskConsumer::~UIThreadTaskConsumer()
{
}

void UIThreadTaskConsumer::start()
{

}

void UIThreadTaskConsumer::stop()
{

}

bool UIThreadTaskConsumer::canTakeTask()
{
    return true;
}

void UIThreadTaskConsumer::runTask()
{
    std::shared_ptr<ITask> task;
    if (_pendingQueue.wait_dequeue_timed(task, -1)) {
        if (auto scheduler = _scheduler.lock()) {
            if (task) {
                emit post(scheduler, task);
            }
        }
    } else {
        qDebug() << "UI-thread-task queue is empty.";
    }
    task = nullptr;
}

void UIThreadTaskConsumer::notifyTaskArrived()
{
    runTask();
}

}
