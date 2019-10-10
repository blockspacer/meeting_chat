#pragma once

#include "callback_handlers.h"

namespace vi {
	class HelperUtils
	{
	public:
		// Helper methods to parse a media object
		static bool isAudioSendEnabled(const absl::optional<MediaSt>& media);

		static bool isAudioSendRequired(const absl::optional<MediaSt>& media);

		static bool isAudioRecvEnabled(const absl::optional<MediaSt>& media);

		static bool isVideoSendEnabled(const absl::optional<MediaSt>& media);

		static bool isVideoSendRequired(const absl::optional<MediaSt>& media);

		static bool isVideoRecvEnabled(const absl::optional<MediaSt>& media);

		static bool isScreenSendEnabled(const absl::optional<MediaSt>& media);

		static bool isDataEnabled(const absl::optional<MediaSt>& media);

		static bool isTrickleEnabled(const absl::optional<bool>& trickle);
	};

}