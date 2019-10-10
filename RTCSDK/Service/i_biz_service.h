#pragma once

namespace core {
class IBizService
{
public:
    virtual ~IBizService() {}

    virtual void init() = 0;

    virtual void cleanup() = 0;

};
}
