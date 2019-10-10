#pragma once

#include "i_websocket_client.h"
#include "websocket.h"
#include <memory>

namespace core {
	class WebsocketClientImpl;
	class WebsocketClient : public IWebsocketClient, public std::enable_shared_from_this<WebsocketClient>
	{
	public:
		WebsocketClient();

		~WebsocketClient();

		void close() override;

		void open(const std::string& url) override;

		void ping(const std::vector<uint8_t> &data) override;

		bool isValid() const override;

		int64_t sendTextMessage(const std::string& message) override;

		int64_t sendBinaryMessage(const std::vector<uint8_t> &data) override;

		WebsocketCloseCode closeCode() const override;

		WebsocketState state() const override;

		void setConnectedCallback(ConnectedCallback callback) override;

		void setDisconnectedCallback(DisconnectedCallback callback) override;

		void setErrorCallback(ErrorCallback callback) override;

		void setPongCallback(PongCallback callback) override;

		void setStateChangedCallback(StateChangedCallback callback) override;

		void setTextMessageReceivedCallback(TextMessageReceivedCallback callback) override;

		void setBinaryMessageReceivedCallback(BinaryMessageReceivedCallback callback) override;

	private:
		std::shared_ptr<IWebsocketClient> _impl;
	};
}


