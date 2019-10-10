#pragma once

#include <memory>
#include "abstract_biz_service.h"
#include "i_unified_factory.h"
#include "event_listeners.h"

namespace core {

class INotificationService : public AbstractBizService
{
public:
    INotificationService(const std::weak_ptr<IUnifiedFactory> unifiedFactory) : AbstractBizService(unifiedFactory) {

    }
    virtual ~INotificationService() {}

};

}
