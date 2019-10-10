#include "serial_task_consumer.h"
#include "task_scheduler.h"
#include "i_task.h"
#include <QThread>
#include "concurrency_task_consumer.h"
#include <QDebug>

namespace core {

SerialTaskConsumer::SerialTaskConsumer()
{
    _type = TaskConsumerType::SERIAL;
    QThread::start(QThread::NormalPriority);
    _pool.setMaxThreadCount(1);
}

SerialTaskConsumer::~SerialTaskConsumer()
{
    this->requestInterruption();
    QThread::wait();
}

void SerialTaskConsumer::start()
{

}

void SerialTaskConsumer::stop()
{
    this->requestInterruption();
    QThread::wait();
}

bool SerialTaskConsumer::canTakeTask()
{
    return true;
}

void SerialTaskConsumer::runTask()
{
    std::shared_ptr<ITask> task;
    if (_pendingQueue.wait_dequeue_timed(task, 1000 * 1000)) {
        if (task && !task->canceled()) {
            //TaskRunner* runner = new TaskRunner(scheduler, task);
            //_pool.start(runner);
            task->execute();
        }

        //if (auto scheduler = _scheduler.lock()) {
        //    scheduler->onTaskCompleted(task);
        //}
    } else {
        qDebug() << "serial-task-queue is empty.";
    }
    task = nullptr;
}

void SerialTaskConsumer::notifyTaskArrived()
{
    //runTask();
}

void SerialTaskConsumer::run()
{
    if (_threadId == -1) {
        _threadId = int64_t(QThread::currentThreadId());
    }

    while (!QThread::currentThread()->isInterruptionRequested()) {
        runTask();
    }
}

}
