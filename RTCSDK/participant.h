#pragma once

#include "plugin_client.h"

namespace vi {
	class Participant
		: public PluginClient
	{
	public:
		Participant(const std::string& plugin, 
			const std::string& opaqueId, 
			int64_t id,
			int64_t privateId,
			const std::string& displayName, 
			std::shared_ptr<WebRTCServiceInterface> wrs);

		~Participant();

	protected:
		void onAttached(bool success) override;

		void onHangup() override;

		void onIceState(webrtc::PeerConnectionInterface::IceConnectionState iceState) override;

		void onMediaState(const std::string& media, bool on) override;

		void onWebrtcState(bool isActive, const std::string& reason) override;

		void onSlowLink(bool uplink, bool lost) override;

		void onMessage(const EventData& data, const Jsep& jsep) override;

		void onLocalStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;

		void onRemoteStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;

		void onData(const std::string& data, const std::string& label) override;

		void onDataOpen(const std::string& label) override;

		void onCleanup() override;

		void onDetached() override;

	private:
		bool _videoOn;
		bool _audioOn;
		std::string _displayName;
		std::weak_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> _renderer;
	};
}


