#pragma once

#include "task_scheduler.h"
#include "i_task_producer.h"
#include "i_task_consumer.h"

namespace core {

TaskScheduler::TaskScheduler(std::shared_ptr<ITaskProducer> producer, std::shared_ptr<ITaskConsumer> consumer)
    : _producer(producer)
    , _consumer(consumer)
{

}

TaskScheduler::~TaskScheduler()
{
    if (_consumer) {
        _consumer->stop();
    }
}

void TaskScheduler::init()
{
    if (_producer) {
        _producer->setTaskScheduler(shared_from_this());
    }
    if (_consumer) {
        _consumer->setTaskScheduler(shared_from_this());
        _consumer->start();
    }
}

void TaskScheduler::postTask(std::shared_ptr<ITask> task)
{
    if (_producer) {
        _producer->postTask(task);
    }
}

void TaskScheduler::onTaskArrived()
{
    if (_producer && _consumer) {
        if (!_consumer->canTakeTask()) {
            return;
        }
        if (_consumer->type() == TaskConsumerType::SERIAL) {
            if (auto task = _producer->produceTask()) {
                _consumer->takeTask(task);
            }
        } else if (_consumer->type() == TaskConsumerType::CONCURRENCY) {
            auto task = _producer->produceTask();
            while (task != nullptr) {
                _consumer->takeTask(task);
                task = _producer->produceTask();
            }
        }
    }
}

void TaskScheduler::onTaskCompleted(std::shared_ptr<ITask> task)
{
    // Note: async
    if (_producer) {
        _producer->removeTask(task);
        _producer->onNextTask();
    }
}

void TaskScheduler::stop()
{
    if (_consumer) {
        _consumer->stop();
    }
}

int64_t TaskScheduler::threadId()
{
    if (_consumer && _consumer->type() == TaskConsumerType::SERIAL) {
        return _consumer->threadId();
    }
    return -1;
}

}
