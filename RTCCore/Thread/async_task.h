#pragma once

#include "i_task.h"
#include <functional>
#include <sstream>
#include <QDebug>
#include <atomic>

namespace core {

// for testing
static std::atomic<int64_t> _counter(0);

class AsyncTask : public ITask {
public:
    AsyncTask(TaskFunc func) : _func(func) { }

    ~AsyncTask() override {
        qDebug() << "~AsyncTask: " << ++_counter;
    }

    TaskPriority priority() override
    {
        return _priority;
    }

    void setPriority(TaskPriority priority) override
    {
        _priority = priority;
    }

    void execute() override
    {
        if (_func) {
            _func();
        }
    }

    void cancel() override
    {
        _canceled = true;
    }

    bool canceled() override
    {
        return _canceled;
    }

private:
    TaskPriority _priority;
    TaskFunc _func;
    bool _canceled = false;
};

}
