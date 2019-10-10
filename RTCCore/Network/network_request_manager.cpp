#include "network_request_manager.h"
#include "network_request_plugin.h"
#include "network_http_client.h"
#include "network_request_consumer.h"

namespace core {

NetworkRequestManager::NetworkRequestManager()
    : _plugins()
{

}

NetworkRequestManager::~NetworkRequestManager()
{

}

void NetworkRequestManager::init()
{
    auto bsphpPlugin = std::make_shared<NetworkRequestPlugin>();
    auto bsphpHttpClient = std::make_shared<NetworkHttpClient>();
    auto bsphpConsumer = std::make_shared<NetworkRequestConsumer>(bsphpPlugin, bsphpHttpClient, 5, RequestRoute::HTTP);
    bsphpPlugin->addRequestConsumer(RequestRoute::HTTP, bsphpConsumer);
    _plugins["bsphp"] = bsphpPlugin;

    auto ccPlugin = std::make_shared<NetworkRequestPlugin>();
    auto ccHttpClient = std::make_shared<NetworkHttpClient>();
    auto ccConsumer = std::make_shared<NetworkRequestConsumer>(ccPlugin, ccHttpClient, 5, RequestRoute::HTTP);
    ccPlugin->addRequestConsumer(RequestRoute::HTTP, ccConsumer);
    _plugins["cc"] = ccPlugin;

    auto lrmpsPlugin = std::make_shared<NetworkRequestPlugin>();
    auto lrmpsHttpClient = std::make_shared<NetworkHttpClient>();
    auto lrmpsConsumer = std::make_shared<NetworkRequestConsumer>(lrmpsPlugin, lrmpsHttpClient, 5, RequestRoute::HTTP);
    lrmpsPlugin->addRequestConsumer(RequestRoute::HTTP, lrmpsConsumer);
    _plugins["lrmps"] = lrmpsPlugin;

    auto dsPlugin = std::make_shared<NetworkRequestPlugin>();
    auto dsHttpClient = std::make_shared<NetworkHttpClient>();
    auto dsConsumer = std::make_shared<NetworkRequestConsumer>(dsPlugin, dsHttpClient, 5, RequestRoute::HTTP);
    dsPlugin->addRequestConsumer(RequestRoute::HTTP, dsConsumer);
    _plugins["ds"] = dsPlugin;
}

void NetworkRequestManager::registerPlugin(const std::string& name, std::shared_ptr<INetworkRequestHandler> plugin)
{
	if (_plugins.find(name) == _plugins.end()) {
		_plugins[name] = plugin;
	}
}

void NetworkRequestManager::unregisterPlugin(const std::string& name)
{
	if (_plugins.find(name) != _plugins.end()) {
		_plugins.erase(name);
	}
}

void NetworkRequestManager::addRequest(const std::shared_ptr<NetworkRequest>& request, bool isTail)
{
    std::shared_ptr<INetworkRequestHandler> handler = _plugins[request->pluginName()];

    if (handler) {
        handler->addRequest(request, isTail);
    }
}

void NetworkRequestManager::cancelAll()
{
    for (auto it = _plugins.begin(); it != _plugins.end(); ++it) {
        (it->second)->cancelAll();
    }
}

void NetworkRequestManager::cancelAll(const std::string& pluginName)
{
    auto plugin = _plugins[pluginName];
    if (plugin) {
        plugin->cancelAll();
    }
}

void NetworkRequestManager::cancelRequest(const std::shared_ptr<NetworkRequest>& request)
{
    std::shared_ptr<INetworkRequestHandler> handler = _plugins[request->pluginName()];

    handler->cancelRequest(request);
}

}
