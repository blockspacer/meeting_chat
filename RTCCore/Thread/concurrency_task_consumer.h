#pragma once

#include "abstract_task_consumer.h"
#include <QThreadPool>
#include <QRunnable>

namespace core {

class TaskRunner : public QRunnable
{
public:
    TaskRunner(std::shared_ptr<ITaskScheduler> scheduler, std::shared_ptr<ITask> task);

private:
    void run() override;

private:
    std::weak_ptr<ITaskScheduler> _scheduler;
    std::shared_ptr<ITask> _task;
};

class ConcurrencyTaskConsumer 
	: public AbstractTaskConsumer
	, public std::enable_shared_from_this<ConcurrencyTaskConsumer>
{
public:
    ConcurrencyTaskConsumer();

    void start() override;
    void stop() override;
    bool canTakeTask() override;

protected:
    void notifyTaskArrived() override;
    void runTask();

private:
    QThreadPool _pool;
};

}
