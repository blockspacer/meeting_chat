#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>
#include "websocket.h"
#include <functional>

namespace core {

	class IWebsocketClient;

	using ConnectedCallback = std::function<void(std::shared_ptr<IWebsocketClient>)>;

	using DisconnectedCallback = std::function<void(std::shared_ptr<IWebsocketClient>)>;

	using ErrorCallback = std::function<void(WebsocketError)>;

	using PongCallback = std::function<void(const std::vector<uint8_t>&)>;

	using StateChangedCallback = std::function<void(WebsocketState)>;

	using TextMessageReceivedCallback = std::function<void(const std::string&)>;

	using BinaryMessageReceivedCallback = std::function<void(const std::vector<uint8_t>&)>;

	class IWebsocketClient
	{
	public:
		virtual ~IWebsocketClient() {}

		virtual void close() = 0;

		virtual void open(const std::string& url) = 0;

		virtual void ping(const std::vector<uint8_t> &data) = 0;

		virtual bool isValid() const = 0;

		virtual int64_t sendTextMessage(const std::string& message) = 0;

		virtual int64_t sendBinaryMessage(const std::vector<uint8_t> &data) = 0;

		virtual WebsocketCloseCode closeCode() const = 0;

		virtual WebsocketState state() const = 0;

		virtual void setConnectedCallback(ConnectedCallback callback) = 0;

		virtual void setDisconnectedCallback(DisconnectedCallback callback) = 0;

		virtual void setErrorCallback(ErrorCallback callback) = 0;

		virtual void setPongCallback(PongCallback callback) = 0;

		virtual void setStateChangedCallback(StateChangedCallback callback) = 0;

		virtual void setTextMessageReceivedCallback(TextMessageReceivedCallback callback) = 0;

		virtual void setBinaryMessageReceivedCallback(BinaryMessageReceivedCallback callback) = 0;
	};
}

