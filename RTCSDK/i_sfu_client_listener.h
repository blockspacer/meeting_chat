#pragma once

#include <memory>
#include "message_transport.h"

namespace vi {
	class ISFUClientListener
	{
	public:
		virtual ~ISFUClientListener() {}

		virtual void onOpened() = 0;

		virtual void onClosed() = 0;

		virtual void onFailed(int errorCode, const std::string& reason) = 0;

		virtual void onMessage(std::shared_ptr<JanusResponse> model) = 0;
	};
}