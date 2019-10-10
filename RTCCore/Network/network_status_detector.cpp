#include "network_status_detector.h"
#include <assert.h>

namespace core {

NetworkStatusDetector::NetworkStatusDetector()
{
    connect(&_ncMgr, &QNetworkConfigurationManager::onlineStateChanged, this, &NetworkStatusDetector::onlineStateChanged);
    _isOnline = true;//_ncMgr.isOnline();
}


NetworkStatusDetector::~NetworkStatusDetector()
{

}

void NetworkStatusDetector::init()
{
}

void NetworkStatusDetector::addListener(const std::shared_ptr<INetworkStatusListener>& listener)
{
    assert(listener != nullptr);
    {
        std::lock_guard<std::mutex> locker(_mutex);
        _listeners.emplace_back(listener);
    }
}

void NetworkStatusDetector::onlineStateChanged(bool isOnline)
{
    _isOnline = isOnline;
    auto it = _listeners.begin();
    while (it!= _listeners.end()) {
        if (auto listener = (*it).lock()) {
            listener->onNetworkStatusChanged(isOnline);
            ++it;
        } else {
            std::lock_guard<std::mutex> locker(_mutex);
            it = _listeners.erase(it);
        }
    }
}


bool NetworkStatusDetector::isOnline() const
{
    return true;//_isOnline;
}

}
