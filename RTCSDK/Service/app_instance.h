#pragma once

#include "Utils/singleton.h"
#include "i_app_instance.h"
#include "i_unified_factory.h"
#include <memory>
#include <sstream>
#include "Network/network_request_manager.h"

namespace core {

class AppInstance : public IAppInstance, public core::Singleton<AppInstance>
{
public:
    ~AppInstance() override;

    void initApp() override;

    void clearnup() override;

    std::shared_ptr<IUnifiedFactory> getUnifiedFactory() override;

    std::shared_ptr<NetworkRequestManager> getNetworkRequestManager() override;

protected:
    void installBizServices();

private:
    AppInstance();
    AppInstance(const AppInstance&) = delete;
    AppInstance& operator=(const AppInstance&) = delete;

private:
    friend class core::Singleton<AppInstance>;
    std::shared_ptr<IUnifiedFactory> _unifiedFactory;
    std::shared_ptr<NetworkRequestManager> _nrMgr;
};

}

#define XApp core::AppInstance::instance()
#define UFactory XApp->getUnifiedFactory()
#define FetchService(S) UFactory->getBizServiceFactory()->getService<S>()
#define Logger core::AppInstance::instance()->getLogger()
