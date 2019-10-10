#include "message_transportor.h"
#include <iostream>
#include "Websocket/i_connection_listener.h"
#include "Websocket/websocket_endpoint.h"
#include "i_message_transport_listener.h"
#include <QDebug>

namespace vi {
	MessageTransportor::MessageTransportor(const std::string& url)
		: _url(url)
	{
		_websocket = std::make_shared<WebsocketEndpoint>();
		if (_url.empty()) {
			_url = "ws://182.61.33.148:8188/janus";
		}
	}

	MessageTransportor::~MessageTransportor()
	{

	}

	bool MessageTransportor::isValid()
	{
		if (_websocket && _connectionId != -1) {
			return true;
		}
		return false;
	}

	// IMessageTransportor
	void MessageTransportor::addListener(std::shared_ptr<IMessageTransportListener> listener)
	{
		std::lock_guard<std::mutex> locker(_listenerMutex);
		addBizObserver<IMessageTransportListener>(_listeners, listener);
	}

	void MessageTransportor::removeListener(std::shared_ptr<IMessageTransportListener> listener)
	{
		std::lock_guard<std::mutex> locker(_listenerMutex);
		removeBizObserver<IMessageTransportListener>(_listeners, listener);
	}

	void MessageTransportor::connect(const std::string& url)
	{
		if (_websocket) {
			_connectionId = _websocket->connect(_url, shared_from_this(), "janus-protocol");
		}
	}

	void MessageTransportor::disconnect()
	{
		if (isValid()) {
			_websocket->close(_connectionId, websocketpp::close::status::normal, "");
		}
	}

	void MessageTransportor::send(const std::string& data, std::shared_ptr<JCHandler> handler)
	{
		if (isValid()) {
			_websocket->sendText(_connectionId, data);
			qDebug() << "sendText: " << data.c_str();
			if (handler->valid()) {
				std::lock_guard<std::mutex> locker(_callbackMutex);
				_callbacksMap[handler->transaction] = handler->callback;
			}
		}
	}

	void MessageTransportor::send(const std::vector<uint8_t>& data, std::shared_ptr<JCHandler> handler)
	{
		if (isValid()) {
			_websocket->sendBinary(_connectionId, data);
			if (handler->valid()) {
				std::lock_guard<std::mutex> locker(_callbackMutex);
				_callbacksMap[handler->transaction] = handler->callback;
			}
		}
	}

	// IConnectionListener
	void MessageTransportor::onOpen()
	{
		qDebug() << "MessageTransportor::onOpen()";
		std::lock_guard<std::mutex> locker(_listenerMutex);
		for (const auto& listener : _listeners) {
			if (auto li = listener.lock()) {
				li->onOpened();
			}
		}
	}

	void MessageTransportor::onFail(int errorCode, const std::string& reason)
	{
		qDebug() << "MessageTransportor::onFail(), errorCode = " << errorCode << ", reason = " << reason.c_str();
	}

	void MessageTransportor::onClose(int closeCode, const std::string& reason)
	{
		qDebug() << "MessageTransportor::onClose(), errorCode = " << closeCode << ", reason = " << reason.c_str();
		std::lock_guard<std::mutex> locker(_listenerMutex);
		for (const auto& listener : _listeners) {
			if (auto li = listener.lock()) {
				li->onClosed();
			}
		}
	}

	bool MessageTransportor::onValidate()
	{
		qDebug() << "MessageTransportor::onValidate()";
		return true;
	}

	void MessageTransportor::onTextMessage(const std::string& text)
	{
		qDebug() << "MessageTransportor::onTextMessage(), text = " << text.c_str();
		std::shared_ptr<JanusResponse> model = std::make_shared<JanusResponse>();
		//JanusResponse  model;
		x2struct::X::loadjson(text, *model, false, true);

		if (!model->xhas("janus")) {
			std::cout << "!response->xhas(\"janus\")"  << std::endl;
			return;
		}
		if (model->janus == "event") {
			std::string x = model->janus;
		}
		if (model->xhas("transaction") && (model->janus == "ack" || model->janus == "success" || model->janus == "error")) {
			std::lock_guard<std::mutex> locker(_callbackMutex);
			const std::string& transaction = model->transaction;
			if (_callbacksMap.find(transaction) != _callbacksMap.end()) {
				const auto& callback = _callbacksMap[transaction];
				if (callback) {
					(*callback)(model);
				}
				_callbacksMap.erase(transaction);
			}
		}
		else {
			std::lock_guard<std::mutex> locker(_listenerMutex);
			for (const auto& listener : _listeners) {
				if (auto li = listener.lock()) {
					li->onMessage(model);
				}
			}
		}
	}

	void MessageTransportor::onBinaryMessage(const std::vector<uint8_t>& data)
	{
		qDebug() << "MessageTransportor::onBinaryMessage(), data.size() = " << data.size();
	}

	bool MessageTransportor::onPing(const std::string& text)
	{
		qDebug() << "MessageTransportor::onPing(), text = " << text.c_str();
		return true;
	}

	void MessageTransportor::onPong(const std::string& text)
	{
		qDebug() << "MessageTransportor::onPong(), text = " << text.c_str();
	}

	void MessageTransportor::onPongTimeout(const std::string& text)
	{
		qDebug() << "MessageTransportor::onPongTimeout(), text = " << text.c_str();
	}
}