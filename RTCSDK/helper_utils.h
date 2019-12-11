#pragma once

#include "webrtc_service_events.h"

namespace vi {
	class HelperUtils
	{
	public:
		// Helper methods to parse a media object
		static bool isAudioSendEnabled(const absl::optional<MediaConfig>& media);

		static bool isAudioSendRequired(const absl::optional<MediaConfig>& media);

		static bool isAudioRecvEnabled(const absl::optional<MediaConfig>& media);

		static bool isVideoSendEnabled(const absl::optional<MediaConfig>& media);

		static bool isVideoSendRequired(const absl::optional<MediaConfig>& media);

		static bool isVideoRecvEnabled(const absl::optional<MediaConfig>& media);

		static bool isScreenSendEnabled(const absl::optional<MediaConfig>& media);

		static bool isDataEnabled(const absl::optional<MediaConfig>& media);

		static bool isTrickleEnabled(const absl::optional<bool>& trickle);
	};

}