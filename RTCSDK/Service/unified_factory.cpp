﻿#include "unified_factory.h"
#include "biz_service_factory.h"

namespace core {

UnifiedFactory::UnifiedFactory()
{

}

void UnifiedFactory::init()
{
    getBizServiceFactory();
}

std::shared_ptr<IBizServiceFactory> UnifiedFactory::getBizServiceFactory()
{
    if (!_bizServiceFactory) {
        _bizServiceFactory = std::make_shared<BizServiceFactory>(shared_from_this());
        _bizServiceFactory->init();
    }

    return _bizServiceFactory;
}

}
