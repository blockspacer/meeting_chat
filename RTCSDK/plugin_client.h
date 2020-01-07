#pragma once

#include <memory>
#include <string>
#include "i_plugin_client.h"
#include "i_webrtc_event_handler.h"

namespace vi {
	class WebRTCServiceInterface;
	class PluginClient
		: public IPluginClient
		, public IWebRTCEventHandler
		, public std::enable_shared_from_this<PluginClient>
	{
	public:
		PluginClient(std::shared_ptr<WebRTCServiceInterface> wrs);

		~PluginClient();

		// IWebRTCEventHandler
		void setHandleId(int64_t handleId) override;

		const std::shared_ptr<PluginContext>& pluginContext() const override;

		// IPluginClient
		void attach() override;

		int32_t remoteVolume() override;

		int32_t localVolume() override;

		bool isAudioMuted() override;

		bool isVideoMuted() override;

		bool muteAudio() override;

		bool muteVideo() override;

		bool unmuteAudio() override;

		bool unmuteVideo() override;

		std::string getBitrate() override;

		void sendMessage(std::shared_ptr<SendMessageEvent> event) override;

		void sendData(std::shared_ptr<SendDataEvent> event) override;

		void sendDtmf(std::shared_ptr<SendDtmfEvent> event) override;

		void createOffer(std::shared_ptr<PrepareWebRTCEvent> event) override;

		void createAnswer(std::shared_ptr<PrepareWebRTCEvent> event) override;

		void handleRemoteJsep(std::shared_ptr<PrepareWebRTCPeerEvent> event) override;

		void hangup(bool sendRequest) override;

		void detach(std::shared_ptr<DetachEvent> event) override;

	protected:
		int64_t _id;
		int64_t _privateId;
		std::shared_ptr<PluginContext> _pluginContext;
	};
}

