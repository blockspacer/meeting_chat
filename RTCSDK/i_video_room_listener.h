#pragma once

#include <memory>
#include <string>

namespace vi {
	class Participant;
	class IVideoRoomListener {
	public:
		virtual ~IVideoRoomListener() {}

	protected:
		virtual void onCreateParticipant(std::shared_ptr<Participant> participant) = 0;

		virtual void onRemoveParticipant(std::shared_ptr<Participant> participant) = 0;
	};
}