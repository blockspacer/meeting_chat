#pragma once

#include <memory>
#include <string>
#include "i_plugin_client.h"
#include "i_webrtc_event_handler.h"

namespace vi {
	class IWebRTCService;
	class PluginClient
		: public IPluginClient
		, public IWebRTCEventHandler
		, public std::enable_shared_from_this<PluginClient>
	{
	public:
		PluginClient(std::shared_ptr<IWebRTCService> wrs);

		~PluginClient();

		// IWebRTCEventHandler
		void setHandleId(int64_t handleId) override;

		const std::shared_ptr<PluginContext>& pluginContext() const override;

		// IPluginClient
		void attach() override;

		int32_t getRemoteVolume() override;

		int32_t getLocalVolume() override;

		bool isAudioMuted() override;

		bool isVideoMuted() override;

		bool muteAudio() override;

		bool muteVideo() override;

		bool unmuteAudio() override;

		bool unmuteVideo() override;

		std::string getBitrate() override;

		void sendMessage(std::shared_ptr<SendMessageHandler> handler) override;

		void sendData(std::shared_ptr<SendDataHandler> handler) override;

		void sendDtmf(std::shared_ptr<SendDtmfHandler> handler) override;

		void createOffer(std::shared_ptr<PrepareWebRTCHandler> handler) override;

		void createAnswer(std::shared_ptr<PrepareWebRTCHandler> handler) override;

		void handleRemoteJsep(std::shared_ptr<PrepareWebRTCPeerHandler> handler) override;

		void hangup(bool sendRequest) override;

		void detach(std::shared_ptr<DetachHandler> handler) override;

	protected:
		int64_t _id;
		int64_t _privateId;
		std::shared_ptr<PluginContext> _pluginContext;
	};
}

