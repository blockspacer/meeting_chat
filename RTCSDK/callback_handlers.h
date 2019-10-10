#pragma once

#include <memory>
#include <functional>
#include "api/peer_connection_interface.h"
#include "api/media_stream_interface.h"
#include "absl/types/optional.h"

namespace vi {
	using HandlerCallback = std::function<void(bool success, const std::string& reason)>;
	class IWebRTCEventHandler;

	class SendMessageHandler {
	public:
		std::string message;
		std::string jsep;
		std::shared_ptr<HandlerCallback> callback;
	};

	class SendDataHandler {
	public:
		std::string text;
		std::string label;
		std::shared_ptr<HandlerCallback> callback;
	};

	class SendDtmfHandler {
	public:
		std::shared_ptr<HandlerCallback> callback;
	};

	struct MediaSt {
		bool audio = true;
		bool video = true;
		bool data = false;

		bool update = false;

		bool addAudio = false;	
		bool addVideo = false;
		bool addData = false;

		bool keepAudio = false;
		bool keepVideo = false;

		bool replaceAudio = false;
		bool replaceVideo = false;

		bool removeAudio = false;
		bool removeVideo = false;

		absl::optional<bool> audioSend;
		absl::optional<bool> videoSend;

		absl::optional<bool> audioRecv;
		absl::optional<bool> videoRecv;

		absl::optional<bool> failIfNoAudio;
		absl::optional<bool> failIfNoVideo;
	};				   

	struct JsepSt {
		std::string type;
		std::string sdp;
		bool trickle;
	};

	using CreateAnswerOfferCallback = std::function<void(bool success, const std::string& reason, const JsepSt& jsep)>;

	class PrepareWebRTCHandler {
	public:
		absl::optional<JsepSt> jsep;
		absl::optional<MediaSt> media;
		absl::optional<bool> trickle;
		absl::optional<bool> simulcast;
		absl::optional<bool> simulcast2;
		absl::optional<bool> iceRestart;
		rtc::scoped_refptr<webrtc::MediaStreamInterface> stream;
		std::shared_ptr<HandlerCallback> callback;
		std::shared_ptr<CreateAnswerOfferCallback> answerOfferCallback;
	};

	class PrepareWebRTCPeerHandler {
	public:
		absl::optional<JsepSt> jsep;
		std::shared_ptr<HandlerCallback> callback;
	};

	class DetachHandler {
	public:
		bool noRequest;
		std::string jsep;
		std::shared_ptr<HandlerCallback> callback;
	};

	class CreateSessionHandler {
	public:
		bool reconnect;
		std::shared_ptr<HandlerCallback> callback;
	};

	class DestroySessionHandler {
	public:
		bool notifyDestroyed;
		bool cleanupHandles;
		std::string jsep;
		std::shared_ptr<HandlerCallback> callback;
	};
}