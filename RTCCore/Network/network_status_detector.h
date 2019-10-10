#pragma once

#include <memory>
#include "network.hpp"
#include <QNetworkConfigurationManager>
#include <mutex>
#include "Utils/singleton.h"

namespace core {

class NetworkStatusDetector :
        public QObject,
        public INetworkStatusDetector,
        public std::enable_shared_from_this<NetworkStatusDetector>,
        public Singleton<NetworkStatusDetector>
{
    Q_OBJECT
public:
    ~NetworkStatusDetector() override;

    void init();

    void addListener(const std::shared_ptr<INetworkStatusListener>& listener) override;

    bool isOnline() const;

protected:
    void onlineStateChanged(bool isOnline);

private:
    NetworkStatusDetector();
    NetworkStatusDetector(const NetworkStatusDetector&);
    NetworkStatusDetector& operator=(const NetworkStatusDetector&);

private:
    std::vector<std::weak_ptr<INetworkStatusListener>> _listeners;
    QNetworkConfigurationManager _ncMgr;
    std::mutex _mutex;
    bool _isOnline = false;
    friend class Singleton<NetworkStatusDetector>;
};

}
