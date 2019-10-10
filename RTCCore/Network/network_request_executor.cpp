#include "network_request_executor.h"
#include <QDebug>

namespace core {

NetworkRequestExecutor::NetworkRequestExecutor(const std::shared_ptr<NetworkRequest>& request,
                                               RequestRoute route,
                                               const std::shared_ptr<INetworkClient>& client)
    : _request(request)
    , _requestRoute(route)
    , _client(client)
    , _listener()
{
    _pluginName = request->pluginName();
}

NetworkRequestExecutor::~NetworkRequestExecutor()
{

}

// INetworkCallback

void NetworkRequestExecutor::onSuccess(int64_t /*requestID*/, const std::shared_ptr<NetworkResponse>& response)
{
    auto callback = _request->callback();
    if (callback != nullptr) {
        callback(response);
    }
    callback = nullptr;
    notifyCompletion();
}

void NetworkRequestExecutor::onFailure(int64_t /*requestID*/, const std::shared_ptr<NetworkResponse>& response)
{
    auto callback = _request->callback();
    if (callback != nullptr) {
        callback(response);
    }
    callback = nullptr;
    notifyCompletion();
}

void NetworkRequestExecutor::onProgress(int64_t /*requestID*/, int32_t /*contentLength*/, int32_t /*pos*/) {}

// public

int64_t NetworkRequestExecutor::requestId() const
{
    return _request->requestId();
}

void NetworkRequestExecutor::execute()
{
    qDebug() << "---------------------------- request execute ----------------------------";
    _request->activityTime().executedTime = std::chrono::steady_clock::now().time_since_epoch().count();

    if (_client->isNetworkReachable()) {
        _client->request(_request, shared_from_this());
    } else {

    }
}

void NetworkRequestExecutor::cancel()
{
    //std::shared_ptr<NetworkRequest> request = std::make_shared<NetworkRequest>(_request->host(),
    //                                                                           _request->path(),
    //                                                                           _request->method());
    //_client->cancelRequest(request);
}

void NetworkRequestExecutor::setListener(const std::weak_ptr<INetworkRequestExecutorListener>& listener)
{
    _listener = listener;
}

void NetworkRequestExecutor::notifyCompletion()
{
    auto listener = _listener.lock();

    if (listener != nullptr) {
        listener->onComplete(shared_from_this());
    }
}

}
