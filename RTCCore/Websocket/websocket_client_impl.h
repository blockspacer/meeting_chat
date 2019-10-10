#pragma once

#include "i_websocket_client.h"
#include "websocket.h"
#include <memory>
#include <QObject>
#include <QAbstractSocket>

class QWebSocket;

namespace core {
	class WebsocketClientImpl : public QObject, public IWebsocketClient, public std::enable_shared_from_this<WebsocketClientImpl>
	{
		Q_OBJECT
	public:
		WebsocketClientImpl();

		~WebsocketClientImpl();

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

	private slots:
		void onConnected();

		void onDisconnected();

		void onError(QAbstractSocket::SocketError);

		void onPong(quint64, const QByteArray&);

		void onStateChanged(QAbstractSocket::SocketState);

		void onTextMessageReceived(const QString& message);

		void onBinaryMessageReceived(const QByteArray& messag);

	private:
		ConnectedCallback _connectedCallback;

		DisconnectedCallback _disconnectedCallback;

		ErrorCallback _errorCallback;

		PongCallback _pongCallback;

		StateChangedCallback _stateChangedCallback;

		TextMessageReceivedCallback _textMessageReceivedCallback;

		BinaryMessageReceivedCallback _binaryMessageReceivedCallback;

		QWebSocket* _websocket;
	};
}


