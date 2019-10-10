#include "websocket_client.h"
#include "websocket_client_impl.h"

namespace core {

	WebsocketClient::WebsocketClient()
		: _impl(std::make_shared<WebsocketClientImpl>())
	{
	}


	WebsocketClient::~WebsocketClient()
	{
	}

	void WebsocketClient::close()
	{
		_impl->close();
	}

	void WebsocketClient::open(const std::string& url)
	{
		_impl->open(url);
	}

	void WebsocketClient::ping(const std::vector<uint8_t> &data)
	{
		_impl->ping(data);
	}

	bool WebsocketClient::isValid() const
	{
		return _impl->isValid();
	}

	int64_t WebsocketClient::sendTextMessage(const std::string& message)
	{
		return _impl->sendTextMessage(message);
	}

	int64_t WebsocketClient::sendBinaryMessage(const std::vector<uint8_t> &data)
	{
		return _impl->sendBinaryMessage(data);
	}

	WebsocketCloseCode WebsocketClient::closeCode() const
	{
		return _impl->closeCode();
	}

	WebsocketState WebsocketClient::state() const
	{
		return _impl->state();
	}

	void WebsocketClient::setConnectedCallback(ConnectedCallback callback)
	{
		_impl->setConnectedCallback(callback);
	}

	void WebsocketClient::setDisconnectedCallback(DisconnectedCallback callback)
	{
		_impl->setDisconnectedCallback(callback);
	}

	void WebsocketClient::setErrorCallback(ErrorCallback callback)
	{
		_impl->setErrorCallback(callback);
	}

	void WebsocketClient::setPongCallback(PongCallback callback)
	{
		_impl->setPongCallback(callback);
	}

	void WebsocketClient::setStateChangedCallback(StateChangedCallback callback)
	{
		_impl->setStateChangedCallback(callback);
	}

	void WebsocketClient::setTextMessageReceivedCallback(TextMessageReceivedCallback callback)
	{
		_impl->setTextMessageReceivedCallback(callback);
	}

	void WebsocketClient::setBinaryMessageReceivedCallback(BinaryMessageReceivedCallback callback)
	{
		_impl->setBinaryMessageReceivedCallback(callback);
	}

}