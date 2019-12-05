#include "plugin_client.h"

namespace vi {
	PluginClient::PluginClient(std::shared_ptr<WebRTCServiceInterface> wrs)
	{
		_pluginContext = std::make_shared<PluginContext>(wrs);
	}

	PluginClient::~PluginClient()
	{
	}

	void PluginClient::setHandleId(int64_t handleId)
	{
		_pluginContext->handleId = handleId;
	}

	const std::shared_ptr<PluginContext>& PluginClient::pluginContext() const
	{
		return _pluginContext;
	}

	void PluginClient::attach()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				wrs->attach(_pluginContext->plugin, _pluginContext->opaqueId, shared_from_this());
			}
		}
	}

	int32_t PluginClient::getRemoteVolume()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				return wrs->getRemoteVolume(_pluginContext->handleId);
			}
		}
		return 0;
	}

	int32_t PluginClient::getLocalVolume()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				return wrs->getLocalVolume(_pluginContext->handleId);
			}
		}
		return 0;
	}

	bool PluginClient::isAudioMuted()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				return wrs->isAudioMuted(_pluginContext->handleId);
			}
		}
		return false;
	}

	bool PluginClient::isVideoMuted()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				return wrs->isVideoMuted(_pluginContext->handleId);
			}
		}
		return false;
	}

	bool PluginClient::muteAudio()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				return wrs->muteAudio(_pluginContext->handleId);
			}
		}
		return false;
	}

	bool PluginClient::muteVideo()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				return wrs->muteVideo(_pluginContext->handleId);
			}
		}
		return false;
	}

	bool PluginClient::unmuteAudio()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				return wrs->unmuteAudio(_pluginContext->handleId);
			}
		}
		return false;
	}

	bool PluginClient::unmuteVideo()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				return wrs->unmuteVideo(_pluginContext->handleId);
			}
		}
		return false;
	}

	std::string PluginClient::getBitrate()
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				return wrs->getBitrate(_pluginContext->handleId);
			}
		}
		return false;
	}

	void PluginClient::sendMessage(std::shared_ptr<SendMessageHandler> handler)
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				wrs->sendMessage(_pluginContext->handleId, handler);
			}
		}
	}

	void PluginClient::sendData(std::shared_ptr<SendDataHandler> handler)
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				wrs->sendData(_pluginContext->handleId, handler);
			}
		}
	}

	void PluginClient::sendDtmf(std::shared_ptr<SendDtmfHandler> handler)
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				wrs->sendDtmf(_pluginContext->handleId, handler);
			}
		}
	}

	void PluginClient::createOffer(std::shared_ptr<PrepareWebRTCHandler> handler)
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				wrs->createOffer(_pluginContext->handleId, handler);
			}
		}
	}

	void PluginClient::createAnswer(std::shared_ptr<PrepareWebRTCHandler> handler)
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				wrs->createAnswer(_pluginContext->handleId, handler);
			}
		}
	}

	void PluginClient::handleRemoteJsep(std::shared_ptr<PrepareWebRTCPeerHandler> handler)
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				wrs->handleRemoteJsep(_pluginContext->handleId, handler);
			}
		}
	}

	void PluginClient::hangup(bool sendRequest)
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				wrs->hangup(_pluginContext->handleId, sendRequest);
			}
		}
	}

	void PluginClient::detach(std::shared_ptr<DetachHandler> handler)
	{
		if (auto wrs = _pluginContext->webrtcService.lock()) {
			if (wrs->status() == ServiceStauts::UP) {
				wrs->detach(_pluginContext->handleId, handler);
			}
		}
	}
}


