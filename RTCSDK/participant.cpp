#include "participant.h"
#include <QDebug>
#include "Thread/async.h"

namespace vi {
	Participant::Participant(const std::string& plugin, 
		const std::string& opaqueId,
		int64_t id,
		int64_t privateId,
		const std::string& displayName,
		std::shared_ptr<WebRTCServiceInterface> wrs)
		: PluginClient(wrs)
		, _displayName(displayName)
	{
		_pluginContext->plugin = plugin;
		_pluginContext->opaqueId = opaqueId;
		_id = id;
		_privateId = privateId;
	}

	Participant::~Participant()
	{
	}

	void Participant::onAttached(bool success)
	{
		if (success) {
			SubscribeRequest request;
			request.request = "join";
			request.room = 1234;
			request.ptype = "subscriber";
			request.feed = _id;
			request.private_id = _privateId;
			// In case you don't want to receive audio, video or data, even if the
			// publisher is sending them, set the 'offer_audio', 'offer_video' or
			// 'offer_data' properties to false (they're true by default), e.g.:
			// 		subscribe["offer_video"] = false;
			if (auto wreh = _pluginContext->webrtcService.lock()) {
				std::shared_ptr<SendMessageHandler> handler = std::make_shared<vi::SendMessageHandler>();
				auto lambda = [](bool success, const std::string& message) {
					std::string text = message;
				};
				std::shared_ptr<vi::HandlerCallback> callback = std::make_shared<vi::HandlerCallback>(lambda);
				handler->message = x2struct::X::tojson(request);
				handler->callback = callback;
				sendMessage(handler);
			}
		}
		else {
			qDebug() << "  -- Error attaching plugin...";
		}
	}

	void Participant::onHangup() {}

	void Participant::onIceState(webrtc::PeerConnectionInterface::IceConnectionState iceState) {}

	void Participant::onMediaState(const std::string& media, bool on) {}

	void Participant::onWebrtcState(bool isActive, const std::string& reason)
	{
		qDebug() << "Janus says this WebRTC PeerConnection (feed #" << _id << ") is " << (isActive ? "up" : "down") << " now";
	}

	void Participant::onSlowLink(bool uplink, bool lost) {}

	void Participant::onMessage(const EventData& data, const Jsep& jsep)
	{
		qDebug() << " ::: Got a message (subscriber) :::";

		if (!data.xhas("videoroom")) {
			return;
		}
		const auto& event = data.videoroom;
		if (event == "attached") {
			int64_t remoteId = data.id;
			std::string remoteName = data.display;
			qDebug() << "Successfully attached to feed " << remoteId << " (" << remoteName.c_str() << ") in room " << data.room;
		}
		else if (event == "event") {
			// Check if we got an event on a simulcast-related event from this publisher
			//const auto& substream = data.substream;
			//const auto& temporal = data.temporal;
		}
		else if (event == "error") {
			qDebug() << "  -- Error attaching plugin...";
		}

		if (!jsep.type.empty() && !jsep.sdp.empty()) {
			qDebug() << "Handling SDP as well...";
			//// Answer and attach
			auto wself = weak_from_this();
			std::shared_ptr<PrepareWebRTCHandler> handler = std::make_shared<PrepareWebRTCHandler>();
			auto callback = std::make_shared<CreateAnswerOfferCallback>([wself](bool success, const std::string& reason, const JsepConfig& jsep) {
				qDebug() << "Got a sdp, type: " << jsep.type.c_str() << ", sdp = " << jsep.sdp.c_str();
				auto self = wself.lock();
				if (!self) {
					return;
				}
				if (success) {
					StartRequest request;
					request.room = 1234;
					if (auto wreh = self->pluginContext()->webrtcService.lock()) {
						std::shared_ptr<SendMessageHandler> handler = std::make_shared<vi::SendMessageHandler>();
						auto lambda = [](bool success, const std::string& message) {
							qDebug() << "message: " << message.c_str();
						};
						std::shared_ptr<vi::HandlerCallback> callback = std::make_shared<vi::HandlerCallback>(lambda);
						handler->message = x2struct::X::tojson(request);
						Jsep jp;
						jp.type = jsep.type;
						jp.sdp = jsep.sdp;
						handler->jsep = x2struct::X::tojson(jp);
						handler->callback = callback;
						self->sendMessage(handler);
					}
				}
				else {
					qDebug() << "WebRTC error: " << reason.c_str();
				}
			});
			handler->answerOfferCallback = callback;
			MediaConfig media;
			media.audioSend = false;
			media.videoSend = false;
			handler->media = media;
			JsepConfig st;
			st.type = jsep.type;
			st.sdp = jsep.sdp;
			handler->jsep = st;
			createAnswer(handler);
		}
	}

	void Participant::onLocalStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {}

	void Participant::onRemoteStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) 
	{
		// TODO: render
		//stream->GetVideoTracks()[0]->AddOrUpdateSink()
	}

	void Participant::onData(const std::string& data, const std::string& label) {}

	void Participant::onDataOpen(const std::string& label) {}

	void Participant::onCleanup() {}

	void Participant::onDetached() {}
}