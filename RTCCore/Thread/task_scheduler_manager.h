#pragma once

#include "task_scheduler.h"
#include "Utils/singleton.h"
#include <unordered_map>
#include <string>
#include "task_producer.h"
#include "serial_task_consumer.h"
#include "concurrency_task_consumer.h"
#include "ui_thread_task_consumer.h"

namespace core {

enum SchedulerType : int {
    DEFAULT_SERIAL,
    DEFAULT_CONCURRENCY,
    MAIN,
    SCHEDULER_TYPE_MAX
};

class TaskSchedulerManager : public Singleton<TaskSchedulerManager> {
public:
    void init()
    {
        auto serialProducer = std::make_shared<TaskProducer>();
        auto serialConsumer = std::make_shared<SerialTaskConsumer>();
        auto serialScheduler = std::make_shared<TaskScheduler>(serialProducer, serialConsumer);
        _schedulers[SchedulerType::DEFAULT_SERIAL] = serialScheduler;

        auto concurrencyProducer = std::make_shared<TaskProducer>();
        auto concurrencyConsumer = std::make_shared<ConcurrencyTaskConsumer>();
        auto concurrencyScheduler = std::make_shared<TaskScheduler>(concurrencyProducer, concurrencyConsumer);
        _schedulers[SchedulerType::DEFAULT_CONCURRENCY] = concurrencyScheduler;

        auto uiProducer = std::make_shared<TaskProducer>();
        auto uiConsumer = std::make_shared<UIThreadTaskConsumer>();
        auto uiScheduler = std::make_shared<TaskScheduler>(uiProducer, uiConsumer);
        _schedulers[SchedulerType::MAIN] = uiScheduler;

        for (auto scheduler : _schedulers) {
            scheduler.second->init();
        }
    }

    std::shared_ptr<ITaskScheduler> scheduler(SchedulerType type)
    {
        return _schedulers[type];
    }

private:
    void createScheduler(SchedulerType type,
                         std::shared_ptr<ITaskProducer> producer,
                         std::shared_ptr<ITaskConsumer> consumer)
    {
        auto scheduler = std::make_shared<TaskScheduler>(producer, consumer);
        if (type >= 0 && type < SchedulerType::SCHEDULER_TYPE_MAX) {
            _schedulers[type] = scheduler;
        }
    }

private:
    std::unordered_map<SchedulerType, std::shared_ptr<ITaskScheduler>> _schedulers;
};

}
