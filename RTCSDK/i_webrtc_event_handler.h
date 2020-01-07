#pragma once

#include <memory>
#include <string>
#include "api/peer_connection_interface.h"
#include "api/media_stream_interface.h"
#include "plugin_context.h"

namespace vi {
	class IWebRTCEventHandler
	{
	public:
		virtual ~IWebRTCEventHandler() {}

		virtual void setHandleId(int64_t handleId) = 0;

		virtual const std::shared_ptr<PluginContext>& pluginContext() const = 0;

		virtual void onAttached(bool success) = 0;

		virtual void onHangup() = 0;

		virtual void onIceState(webrtc::PeerConnectionInterface::IceConnectionState iceState) = 0;

		virtual void onMediaState(const std::string& media, bool on) = 0;

		virtual void onWebrtcState(bool isActive, const std::string& reason) = 0;

		virtual void onSlowLink(bool uplink, bool lost) = 0;

		virtual void onMessage(const EventData& data, const Jsep& jsep) = 0;

		virtual void onCreateLocalStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) = 0;

		virtual void onDeleteLocalStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) = 0;

		virtual void onCreateRemoteStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) = 0;

		virtual void onDeleteRemoteStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) = 0;

		virtual void onData(const std::string& data, const std::string& label) = 0;

		virtual void onDataOpen(const std::string& label) = 0;

		virtual void onCleanup() = 0;

		virtual void onDetached() = 0;
	};
}
