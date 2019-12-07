#pragma once

#include <memory>

namespace vi {
	class WebRTCServiceInterface;
	class ThreadManager;
	class TaskQueueManager;
}

namespace core {

class IUnifiedFactory;
class NetworkRequestManager;

class IAppInstance {
public:
    virtual ~IAppInstance() {}

    virtual void initApp() = 0;

    virtual void clearnup() = 0;

    virtual std::shared_ptr<IUnifiedFactory> getUnifiedFactory() = 0;

	virtual std::shared_ptr<vi::WebRTCServiceInterface> getWebrtcService() = 0;

    virtual std::shared_ptr<NetworkRequestManager> getNetworkRequestManager() = 0;

	virtual std::shared_ptr<vi::ThreadManager> getThreadManager() = 0;

	virtual std::shared_ptr<vi::TaskQueueManager> getTaskQueueManager() = 0;
};

}
