#pragma once

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <chrono>
#include "network.hpp"

namespace core {

class NetworkRequestManager : public std::enable_shared_from_this<NetworkRequestManager> {
public:
    NetworkRequestManager();

    ~NetworkRequestManager();

    void init();

	void registerPlugin(const std::string& name, std::shared_ptr<INetworkRequestHandler> plugin);

	void unregisterPlugin(const std::string& name);

    void addRequest(const std::shared_ptr<NetworkRequest>& request, bool isTail = true);

    void cancelAll();
    void cancelAll(const std::string& pluginName);
    void cancelRequest(const std::shared_ptr<NetworkRequest>& request);

private:
    std::unordered_map<std::string, std::shared_ptr<INetworkRequestHandler>> _plugins;
};

}
