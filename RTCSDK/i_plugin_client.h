#pragma once

#include "callback_handlers.h"

namespace vi {
	class IPluginClient
	{
	public:
		virtual ~IPluginClient() {}

		virtual void attach() = 0;

		virtual int32_t getRemoteVolume() = 0;

		virtual int32_t getLocalVolume() = 0;

		virtual bool isAudioMuted() = 0;

		virtual bool isVideoMuted() = 0;

		virtual bool muteAudio() = 0;

		virtual bool muteVideo() = 0;

		virtual bool unmuteAudio() = 0;

		virtual bool unmuteVideo() = 0;

		virtual std::string getBitrate() = 0;

		virtual void sendMessage(std::shared_ptr<SendMessageHandler> handler) = 0;

		virtual void sendData(std::shared_ptr<SendDataHandler> handler) = 0;

		virtual void sendDtmf(std::shared_ptr<SendDtmfHandler> handler) = 0;

		virtual void createOffer(std::shared_ptr<PrepareWebRTCHandler> handler) = 0;

		virtual void createAnswer(std::shared_ptr<PrepareWebRTCHandler> handler) = 0;

		virtual void handleRemoteJsep(std::shared_ptr<PrepareWebRTCPeerHandler> handler) = 0;

		virtual void hangup(bool sendRequest) = 0;

		virtual void detach(std::shared_ptr<DetachHandler> handler) = 0;
	};
}