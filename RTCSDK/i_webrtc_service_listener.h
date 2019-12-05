#pragma once

#include <stdint.h>

namespace vi {
	enum class ServiceStauts : uint8_t {
		UP = 0,
		DOWN,
		RESTARTING
	};

	class IWebRTCServiceListener {
	public:
		virtual ~IWebRTCServiceListener() {}

		virtual void onStatus(ServiceStauts status) = 0;
	};
}