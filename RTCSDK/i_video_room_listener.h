#pragma once

#include <memory>
#include <string>

namespace vi {
	class IVideoRoomListener {
	public:
		virtual ~IVideoRoomListener() {}

	protected:
		virtual void createParticipant(int64_t id, const std::string& displayName, const std::string& audio, const std::string& video) = 0;
	};
}