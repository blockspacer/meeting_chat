#pragma once

#include "message_models.h"
#include <memory>

namespace vi {
	class IMessageTransportListener
	{
	public:
		virtual ~IMessageTransportListener() {}

		virtual void onOpened() = 0;

		virtual void onClosed() = 0;

		virtual void onFailed(int errorCode, const std::string& reason) = 0;

		virtual void onMessage(std::shared_ptr<JanusResponse> model) = 0;

	};
}