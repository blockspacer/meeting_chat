#include "app_instance.h"
#include <mutex>
#include <memory>
#include "unified_factory.h"
#include <QDebug>
#include "notification_service.h"
#include "Thread/task_scheduler_manager.h"
#include "webrtc_service.h"
#include "webrtc_service_proxy.h"

namespace core {

AppInstance::AppInstance()
{

}

AppInstance::~AppInstance()
{
    qDebug() << "~AppInstance()";
}

void AppInstance::initApp()
{
    TaskSchedulerManager::instance()->init();
    this->getNetworkRequestManager()->init();

    // init services here
    installBizServices();
}

void AppInstance::clearnup()
{
	auto wrs = FetchService(vi::IWebRTCService);
	wrs->cleanup();
}

std::shared_ptr<IUnifiedFactory> AppInstance::getUnifiedFactory()
{
    if (!_unifiedFactory) {
        _unifiedFactory = std::make_shared<UnifiedFactory>();
        _unifiedFactory->init();
    }
    return _unifiedFactory;
}

std::shared_ptr<NetworkRequestManager> AppInstance::getNetworkRequestManager()
{
    if (!_nrMgr) {
        _nrMgr = std::make_shared<NetworkRequestManager>();
    }
    return _nrMgr;
}

void AppInstance::installBizServices()
{
    auto uf = getUnifiedFactory();

    auto ns = std::make_shared<NotificationService>(uf);
    ns->init();

	auto ws = std::make_shared<vi::WebRTCService>(uf);
	ws->init();

	//std::unique_ptr<vi::WebRTCService> uws;
	//uws.reset(new vi::WebRTCService(uf));
	//webrtc::WebRTCServiceProxy::Create(nullptr, nullptr, std::move(uws));
}

}
