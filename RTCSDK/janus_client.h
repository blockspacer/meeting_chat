#pragma once
#include <memory>
#include <unordered_map>
#include "i_sfu_client.h"
#include "Websocket/websocket_endpoint.h"
#include "i_message_transport_listener.h"
#include "Service/observable.h"

namespace vi {
	class IMessageTransportor;
	class JanusClient
		: public ISFUClient
		, public IMessageTransportListener
		, public core::Observable
		, public std::enable_shared_from_this<JanusClient>
	{
	public:
		JanusClient(const std::string& url);

		~JanusClient() override;

		void setToken(const std::string& token);

		void setApiSecret(const std::string& apisecret);

		// ISFUClient
		void addListener(std::shared_ptr<ISFUClientListener> listener) override;

		void removeListener(std::shared_ptr<ISFUClientListener> listener) override;

		void init() override;

		void createSession(std::shared_ptr<JCCallback> callback) override;

		void destroySession(int64_t sessionId, std::shared_ptr<JCCallback> callback) override;

		void reconnectSession(int64_t sessionId, std::shared_ptr<JCCallback> callback) override;

		void keepAlive(int64_t sessionId, std::shared_ptr<JCCallback> callback) override;

		void attach(int64_t sessionId, const std::string& plugin, const std::string& opaqueId, std::shared_ptr<JCCallback> callback) override;

		void detach(int64_t sessionId, int64_t handleId, std::shared_ptr<JCCallback> callback) override;

		void sendMessage(int64_t sessionId, int64_t handleId, const std::string& message, const std::string& jsep, std::shared_ptr<JCCallback> callback) override;

		void sendTrickleCandidate(int64_t sessionId, int64_t handleId, const CandidateData& candidate, std::shared_ptr<JCCallback> callback) override;

		void hangup(int64_t sessionId, int64_t handleId, std::shared_ptr<JCCallback> callback) override;

	protected:
		// IMessageTransportListener
		void onOpened() override;

		void onClosed() override;

		void onFailed(int errorCode, const std::string& reason) override;

		void onMessage(std::shared_ptr<JanusResponse> model) override;

	private:
		std::string _url;
		std::string _token;
		std::string _apisecret;
		std::shared_ptr<IMessageTransport> _transport;
		std::vector<std::weak_ptr<ISFUClientListener>> _listeners;
	};
}