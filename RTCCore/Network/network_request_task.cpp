#include "network_request_task.h"

namespace core {

NetworkRequestTask::NetworkRequestTask(const std::shared_ptr<NetworkRequest>& request)
    : _request(request)
{

}

NetworkRequestTask::~NetworkRequestTask()
{

}

RequestPriority NetworkRequestTask::priority() const
{
    return _request->priority();
}

const std::shared_ptr<NetworkRequest>& NetworkRequestTask::request()
{
    return _request;
}

RequestRoute NetworkRequestTask::requestRoute() const
{
    return _request->requestRoute();
}

}
