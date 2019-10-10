#pragma once

#include <inttypes.h>
#include <functional>

namespace core {

enum TaskPriority : int {
    LOW,
    NORMAL,
    HIGH
};

using TaskFunc = std::function<void()>;

class ITask {
public:
    virtual ~ITask() {}
    virtual int64_t id() { return int64_t(this); }
    virtual TaskPriority priority() = 0;
    virtual void setPriority(TaskPriority priority) = 0;
    virtual void execute() = 0;
    virtual void cancel() = 0;
    virtual bool canceled()  = 0;
};

}
