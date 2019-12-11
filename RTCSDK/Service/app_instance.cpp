#include "app_instance.h"
#include <mutex>
#include <memory>
#include "unified_factory.h"
#include <QDebug>
#include "notification_service.h"
#include "webrtc_service.h"
#include "webrtc_service_interface.h"
#include "webrtc_service_proxy.h"
#include "thread_manager.h"
#include "task_queue_manager.h"
#include "sfu_listener_proxy.h"
#include "janus_client.h"

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
	if (!_threadMgr) {
		_threadMgr = std::make_shared<vi::ThreadManager>();
		_threadMgr->init();
	}

	if (!_taskQueueMgr) {
		_taskQueueMgr = std::make_shared<vi::TaskQueueManager>();
		_taskQueueMgr->init();
	}
	
    this->getNetworkRequestManager()->init();

    // init services here
    installBizServices();

	installWebRTCService();
}

void AppInstance::clearnup()
{
	//auto wrs = FetchService(vi::WebRTCServiceInterface);
	_webrtcService->cleanup();
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
    if (!_networkRequestMgr) {
		_networkRequestMgr = std::make_shared<NetworkRequestManager>();
    }
    return _networkRequestMgr;
}

std::shared_ptr<vi::ThreadManager> AppInstance::getThreadManager()
{
	return _threadMgr;
}

std::shared_ptr<vi::TaskQueueManager> AppInstance::getTaskQueueManager()
{
	return _taskQueueMgr;
}

std::shared_ptr<vi::WebRTCServiceInterface> AppInstance::getWebrtcService()
{
	return _webrtcService;
}

void AppInstance::installBizServices()
{
    auto uf = getUnifiedFactory();

    auto ns = std::make_shared<NotificationService>(uf);
    ns->init();
}

void AppInstance::installWebRTCService()
{
	std::shared_ptr<rtc::Thread> wst = getThreadManager()->getThread(vi::ThreadName::WEBRTC_SERVICE);

	auto wsi = std::make_shared<vi::WebRTCService>();
	_webrtcService = vi::WebRTCServiceProxy::Create(wst, wsi);

	auto scl = std::make_shared<vi::SFUListener>();
	_sfuListener = vi::SFUListenerProxy::Create(wst, scl);
	_sfuListener->init(wsi);

	auto jc = std::make_shared<vi::JanusClient>("ws://106.13.6.35:8188/janus");
	jc->addListener(_sfuListener);

	_webrtcService->init(jc);
}

}
