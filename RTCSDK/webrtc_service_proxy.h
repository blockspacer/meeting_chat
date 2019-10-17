//#pragma once
//
//#include "webrtc_service.h"
//#include "api/proxy.h"
//
//namespace webrtc {
//	using namespace vi;
//	BEGIN_OWNED_PROXY_MAP(WebRTCService)
//		PROXY_SIGNALING_THREAD_DESTRUCTOR()
//		PROXY_METHOD1(void, addListener, std::shared_ptr<IWebRTCServiceListener>)
//		PROXY_METHOD1(void, removeListener, std::shared_ptr<IWebRTCServiceListener>)
//		PROXY_METHOD0(WRServiceStauts, status)
//		PROXY_METHOD3(void, attach, const std::string&, const std::string&, std::shared_ptr<IWebRTCEventHandler>)
//		PROXY_METHOD1(void, destroy, std::shared_ptr<DestroySessionHandler>)
//		PROXY_METHOD0(void, reconnect)
//		PROXY_METHOD1(int32_t, getRemoteVolume, int64_t)
//		PROXY_METHOD1(int32_t, getLocalVolume, int64_t)
//		PROXY_METHOD1(bool, isAudioMuted, int64_t)
//		PROXY_METHOD1(bool, isVideoMuted, int64_t)
//		PROXY_METHOD1(bool, muteAudio, int64_t)
//		PROXY_METHOD1(bool, muteVideo, int64_t)
//		PROXY_METHOD1(bool, unmuteAudio, int64_t)
//		PROXY_METHOD1(bool, unmuteVideo, int64_t)
//		PROXY_METHOD1(std::string, getBitrate, int64_t)
//		PROXY_METHOD2(void, sendMessage, int64_t, std::shared_ptr<SendMessageHandler>)
//		PROXY_METHOD2(void, sendData, int64_t, std::shared_ptr<SendDataHandler>)
//		PROXY_METHOD2(void, sendDtmf, int64_t, std::shared_ptr<SendDtmfHandler>)
//		PROXY_METHOD2(void, createOffer, int64_t, std::shared_ptr<PrepareWebRTCHandler>)
//		PROXY_METHOD2(void, createAnswer, int64_t, std::shared_ptr<PrepareWebRTCHandler>)
//		PROXY_METHOD2(void, handleRemoteJsep, int64_t, std::shared_ptr<PrepareWebRTCPeerHandler>)
//		PROXY_METHOD2(void, hangup, int64_t, bool)
//		PROXY_METHOD2(void, detach, int64_t, std::shared_ptr<DetachHandler>)
//	END_PROXY_MAP()
//}