#pragma once

#include <memory>
#include "network.hpp"

namespace core {

class NetworkRequestTask
{
public:
    NetworkRequestTask(const std::shared_ptr<NetworkRequest>& request);
    virtual ~NetworkRequestTask();

    const std::shared_ptr<NetworkRequest>& request();

    RequestPriority priority() const;

    RequestRoute requestRoute() const;

private:
    std::shared_ptr<NetworkRequest> _request;
};

}
