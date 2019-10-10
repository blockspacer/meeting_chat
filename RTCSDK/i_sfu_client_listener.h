#pragma once

#include <memory>
#include "message_transportor.h"

namespace vi {
	class ISFUClientListener
	{
	public:
		virtual ~ISFUClientListener() {}

		virtual void onConnected() = 0;

		virtual void onDisconnected() = 0;

		virtual void onMessage(std::shared_ptr<JanusResponse> model) = 0;

		virtual void onError(int errorCode, const std::string& reason) = 0;
	};
}