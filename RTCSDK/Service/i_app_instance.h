#pragma once

#include <memory>

namespace core {

class IUnifiedFactory;
class NetworkRequestManager;

class IAppInstance {
public:
    virtual ~IAppInstance() {}

    virtual void initApp() = 0;

    virtual void clearnup() = 0;

    virtual std::shared_ptr<IUnifiedFactory> getUnifiedFactory() = 0;

    virtual std::shared_ptr<NetworkRequestManager> getNetworkRequestManager() = 0;
};

}
