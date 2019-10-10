#include "notification_service.h"
#include <assert.h>
#include "unified_factory.h"

namespace core {

NotificationService::NotificationService(const std::weak_ptr<IUnifiedFactory> unifiedFactory)
    : INotificationService(unifiedFactory)
{

}

void NotificationService::init()
{
    auto uf = _unifiedFactory.lock();
    assert(uf != nullptr);
    auto bsf = uf->getBizServiceFactory();
    assert(bsf != nullptr);
    bsf->registerService(typeid(INotificationService).name(), shared_from_this());
}

void NotificationService::cleanup()
{

}

}
