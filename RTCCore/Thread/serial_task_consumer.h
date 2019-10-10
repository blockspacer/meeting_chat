#pragma once

#include "abstract_task_consumer.h"
#include <QThread>
#include <QThreadPool>

namespace core {

class SerialTaskConsumer 
	: public QThread
	, public AbstractTaskConsumer
	, public std::enable_shared_from_this<SerialTaskConsumer>
{
    Q_OBJECT
public:
    SerialTaskConsumer();
    ~SerialTaskConsumer() override;

    void start() override;
    void stop() override;
    bool canTakeTask() override;

protected:
    void notifyTaskArrived() override;
    void runTask();
    void run() override;

    QThreadPool _pool;
};

}
