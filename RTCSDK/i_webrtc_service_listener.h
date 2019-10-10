#pragma once

#include <stdint.h>

namespace vi {
	enum class WRServiceStauts : uint8_t {
		UP = 0,
		DOWN,
		RESTARTING
	};

	class IWebRTCServiceListener {
	public:
		virtual ~IWebRTCServiceListener() {}

		virtual void onStatus(WRServiceStauts status) = 0;
	};
}