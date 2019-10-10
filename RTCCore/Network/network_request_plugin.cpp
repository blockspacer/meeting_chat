#include "network_request_plugin.h"
#include <assert.h>
#include "network_request_task.h"
#include <QDebug>

namespace core {

NetworkRequestPlugin::NetworkRequestPlugin()
{
    _pendingTasks.insert(std::make_pair(RequestPriority::LOW, std::deque<std::shared_ptr<NetworkRequestTask>>()));
    _pendingTasks.insert(std::make_pair(RequestPriority::NORMAL, std::deque<std::shared_ptr<NetworkRequestTask>>()));
    _pendingTasks.insert(std::make_pair(RequestPriority::HIGH, std::deque<std::shared_ptr<NetworkRequestTask>>()));
    _pendingTasks.insert(std::make_pair(RequestPriority::SEPECIFIC, std::deque<std::shared_ptr<NetworkRequestTask>>()));
}

NetworkRequestPlugin::~NetworkRequestPlugin()
{

}

void NetworkRequestPlugin::addRequestConsumer(RequestRoute route, const std::shared_ptr<INetworkRequestConsumer>& consumer)
{
    _consumers[route] = consumer;
}

void NetworkRequestPlugin::cancelAll()
{
    cancelAllPendingTasks();

    cancelAllConsumers();
}

void NetworkRequestPlugin::cancelRequest(const std::shared_ptr<NetworkRequest>& request)
{
    assert(request != nullptr);

    request->setCancelled(true);

    if (isRequestInPending(request)) {
        //callXApiResponseCallback(NetworkFailType::CANCELLED, request);
    } else {
        for (auto iter = _consumers.begin(); iter != _consumers.end(); ++iter) {
            (iter->second)->cancelRequest(request);
        }
    }
}

void NetworkRequestPlugin::appendTask(const std::shared_ptr<NetworkRequestTask>& task, bool isTail)
{
    {
        std::lock_guard<std::mutex> guard(_lock);
        if (isTail) {
            _pendingTasks[task->priority()].push_back(task);
        } else {
            _pendingTasks[task->priority()].push_front(task);
        }
    }
}

void NetworkRequestPlugin::addRequest(const std::shared_ptr<NetworkRequest>& request, bool isTail)
{
    assert(request != nullptr);

    auto task = std::make_shared<NetworkRequestTask>(request);

    appendTask(task, isTail);

    notifyRequestArrived(request->requestRoute());
}

std::string NetworkRequestPlugin::pluginName() const
{
    return _pluginName;
}

void NetworkRequestPlugin::onNetworkStatusChanged(bool online)
{

}

const std::shared_ptr<NetworkRequest> NetworkRequestPlugin::produceRequest(RequestRoute route)
{
    std::shared_ptr<NetworkRequestTask> result = nullptr;
    qDebug() << "---------------------------- produceRequest ----------------------------";
    {
        std::lock_guard<std::mutex> guard(_lock);

        for (int32_t index = (int32_t)RequestPriority::SEPECIFIC; index >= (int32_t)RequestPriority::LOW; --index) {
            if (!canProduceRequest(static_cast<RequestPriority>(index))) {
                break;
            }

            result = nextTaskInQueue(route, _pendingTasks[(RequestPriority)index]);

            if (result) {
                break;
            }
        }

        if (result != nullptr) {
            assert(result->request() != nullptr);

            //changeTaskWeight(m_pendingTasks[ApiRequestPriority::NORMAL], m_pendingTasks[ApiRequestPriority::HIGH], kApiRequestHighWeight);
            //changeTaskWeight(m_pendingTasks[ApiRequestPriority::LOW], m_pendingTasks[ApiRequestPriority::NORMAL], kApiRequestNormalWeight);
        }
    }

    return result == nullptr ? nullptr : result->request();
}

// private

bool NetworkRequestPlugin::canProduceRequest(RequestPriority /*priority*/)
{
    return true;
}

bool NetworkRequestPlugin::isConsumerAvailable(RequestRoute route)
{
    bool isAvailable = true;

    auto iter = _consumers.find(route);
    if (iter != _consumers.end()) {
        isAvailable = (iter->second)->isAvailiable();
    }

    return isAvailable;
}

bool NetworkRequestPlugin::canBeHandled(RequestRoute route, const std::shared_ptr<NetworkRequestTask>& task)
{
    return (task->requestRoute() == route) || ((static_cast<int>(task->requestRoute() == route) != 0) && isConsumerAvailable(route));
}

std::shared_ptr<NetworkRequestTask> NetworkRequestPlugin::nextTaskInQueue(RequestRoute route,
                                                                          std::deque<std::shared_ptr<NetworkRequestTask>>& queue)
{
    auto iter = queue.begin();

    while (iter != queue.end()) {
        if (canBeHandled(route, (*iter))) {
            auto task = *iter;
            queue.erase(iter);
            return task;
        } else {
            ++iter;
        }
    }

    return nullptr;
}

void NetworkRequestPlugin::notifyRequestArrived(RequestRoute route)
{
    for (int32_t i = (int)RequestRoute::REQUEST_ROUTE_MAX; i >= (int)RequestRoute::HTTP; i >>= 1) {
        auto iter = _consumers.find(RequestRoute(i));

        if (iter != _consumers.end()) {
            if ((iter->second)->canHandleRequest(route)) {
                (iter->second)->onNetworkRequestArrived();
            }
        }
    }
}

void NetworkRequestPlugin::cancelAllConsumers()
{
    for (auto iter = _consumers.begin(); iter != _consumers.end(); ++iter) {
        (iter->second)->cancelAll();
    }
}

void NetworkRequestPlugin::cancelAllPendingTasks()
{
    {
        std::lock_guard<std::mutex> guard(_lock);

        for (auto iter = _pendingTasks.begin(); iter != _pendingTasks.end(); ++iter) {
            auto queue = iter->second;

            for (auto queueIter = queue.begin(); queueIter != queue.end(); ++queueIter)  {
                auto task = *queueIter;
                //callXApiResponseCallback(NetworkFailType::CANCELLED, task->request());
            }
        }

        _pendingTasks.clear();
    }
}

bool NetworkRequestPlugin::isRequestInPending(const std::shared_ptr<NetworkRequest>& request)
{
    bool exist = false;

    {
        std::lock_guard<std::mutex> guard(_lock);

        for (auto iter = _pendingTasks.begin(); iter != _pendingTasks.end(); ++iter) {
            auto queue = iter->second;

            for (auto queueIter = queue.begin(); queueIter != queue.end(); ++queueIter) {
                auto task = *queueIter;
                if (task->request()->requestId() == request->requestId()) {
                    exist = true;
                    break;
                }
            }

            if (exist) {
                break;
            }
        }
    }

    return exist;
}

}
