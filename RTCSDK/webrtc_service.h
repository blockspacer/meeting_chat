#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "api/peer_connection_interface.h"
#include "i_webrtc_service.h"
#include "i_sfu_client.h"
#include "i_sfu_client_listener.h"
#include <QObject>
#include <QTimer>
#include "callback_handlers.h"

namespace vi {
	class WebRTCService
		: public QObject
		, public IWebRTCService
		, public ISFUClientListener
		, public std::enable_shared_from_this<WebRTCService>
	{
		Q_OBJECT
	public:
		WebRTCService(const std::weak_ptr<IUnifiedFactory> unifiedFactory);

		~WebRTCService() override;

		void init() override;

		void cleanup() override;

	protected:
		// IWebRTCService implement

	    void addListener(std::shared_ptr<IWebRTCServiceListener> listener) override;

		void removeListener(std::shared_ptr<IWebRTCServiceListener> listener) override;

		WRServiceStauts status() override;

		void attach(const std::string& plugin, const std::string& opaqueId, std::shared_ptr<IWebRTCEventHandler> wreh) override;

		void destroy(std::shared_ptr<DestroySessionHandler> handler) override;

		void reconnect() override;


		int32_t getVolume(int64_t handleId, bool isRemote);

		int32_t getRemoteVolume(int64_t handleId) override;

		int32_t getLocalVolume(int64_t handleId) override;

		bool isAudioMuted(int64_t handleId) override;

		bool isVideoMuted(int64_t handleId) override;

		bool isMuted(int64_t handleId, bool isVideo);

		bool muteAudio(int64_t handleId) override;

		bool muteVideo(int64_t handleId) override;

		bool unmuteAudio(int64_t handleId) override;

		bool unmuteVideo(int64_t handleId) override;

		bool mute(int64_t handleId, bool isVideo, bool mute);

		std::string getBitrate(int64_t handleId) override;

		void sendMessage(int64_t handleId, std::shared_ptr<SendMessageHandler> handler) override;

		void sendData(int64_t handleId, std::shared_ptr<SendDataHandler> handler) override;

		void sendDtmf(int64_t handleId, std::shared_ptr<SendDtmfHandler> handler) override;

		void prepareWebrtc(int64_t handleId, bool isOffer, std::shared_ptr<PrepareWebRTCHandler> handler);
		void createOffer(int64_t handleId, std::shared_ptr<PrepareWebRTCHandler> handler) override;

		void createAnswer(int64_t handleId, std::shared_ptr<PrepareWebRTCHandler> handler) override;

		void prepareWebrtcPeer(int64_t handleId, std::shared_ptr<PrepareWebRTCPeerHandler> handler);
		void handleRemoteJsep(int64_t handleId, std::shared_ptr<PrepareWebRTCPeerHandler> handler) override;

		void cleanupWebrtc(int64_t handleId, bool sendRequest = true);
		void hangup(int64_t handleId, bool sendRequest) override;

		void destroyHandle(int64_t handleId, std::shared_ptr<DetachHandler> handler);
		void detach(int64_t handleId, std::shared_ptr<DetachHandler> handler) override;

	protected:
		// ISFUClientListener
	    void onConnected() override;

		void onDisconnected() override;

		void onMessage(std::shared_ptr<JanusResponse> model) override;

		void onError(int errorCode, const std::string& reason) override;

	protected slots:
		void heartbeat();

	private:
		void createSession(std::shared_ptr<CreateSessionHandler> handler);

		void startHeartbeat();

		std::shared_ptr<IWebRTCEventHandler> getHandler(int64_t handleId);

		void prepareStreams(int64_t handleId, 
			std::shared_ptr<PrepareWebRTCHandler> handler,
			rtc::scoped_refptr<webrtc::MediaStreamInterface> stream);

		void sendSDP(int64_t handleId, std::shared_ptr<PrepareWebRTCHandler> handler);

		void createDataChannel(int64_t handleId, const std::string& dcLabel, rtc::scoped_refptr<webrtc::DataChannelInterface> incoming);

		void _createOffer(int64_t handleId, std::shared_ptr<PrepareWebRTCHandler> handler);

		void _createAnswer(int64_t handleId, std::shared_ptr<PrepareWebRTCHandler> handler);

		void destroySession(std::shared_ptr<DestroySessionHandler> handler);

		void configTracks(const MediaSt& media, rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc);

	private:
		std::string _server;	
		std::string _token;
		std::string _opaqueId;
		std::string _apiSecret;
		bool _unifiedPlan = true;
		std::vector<std::string> _iceServers;
		bool _ipv6Support;
		webrtc::PeerConnectionInterface::BundlePolicy _bundlePolicy;
		webrtc::PeerConnectionInterface::IceTransportsType _iceTransportPolicy;
		webrtc::PeerConnectionInterface::TlsCertPolicy _tlsCertPolicy;
		bool _destroyOnUnload;
		int64_t _sessionId = -1;
		int32_t _keepAlivePeriod;
		bool _connected = false;
		int32_t _retries = 3;
		bool _reconnect = false;

		std::unordered_map<int64_t, std::shared_ptr<IWebRTCEventHandler>> _wrehs;

		std::shared_ptr<ISFUClient> _sfuClient;
		std::shared_ptr<QTimer> _heartbeatTimer;

		WRServiceStauts _serviceStatus = WRServiceStauts::DOWN;

		std::vector<std::weak_ptr<IWebRTCServiceListener>> _listeners;

		rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _pcf;
	};
}

