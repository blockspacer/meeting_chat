#include "websocket_endpoint.h"
#include "Websocket/i_connection_listener.h"

namespace vi {
	WebsocketEndpoint::WebsocketEndpoint()
		: _nextId(0) {
		_endpoint.clear_access_channels(websocketpp::log::alevel::all);
		_endpoint.clear_error_channels(websocketpp::log::elevel::all);

		_endpoint.init_asio();
		_endpoint.start_perpetual();

		_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &_endpoint);
	}

	WebsocketEndpoint::~WebsocketEndpoint() {
		_endpoint.stop_perpetual();

		for (ConnectionList::const_iterator it = _connectionList.begin(); it != _connectionList.end(); ++it) {
			if (it->second->getStatus() != "Open") {
				// Only close open connections
				continue;
			}

			std::cout << "> Closing connection " << it->second->getId() << std::endl;

			websocketpp::lib::error_code ec;
			_endpoint.close(it->second->getHdl(), websocketpp::close::status::going_away, "", ec);
			if (ec) {
				std::cout << "> Error closing connection " << it->second->getId() << ": "
					<< ec.message() << std::endl;
			}
		}

		_thread->join();
	}

	int WebsocketEndpoint::connect(std::string const& uri, std::shared_ptr<IConnectionListener> listener, const std::string& subprotocol) {
		websocketpp::lib::error_code ec;

		client::connection_ptr con = _endpoint.get_connection(uri, ec); 

		if (ec) {
			std::cout << "> Connect initialization error: " << ec.message() << std::endl;
			return -1;
		}

		if (!subprotocol.empty()) {
			con->add_subprotocol(subprotocol, ec);
			if (ec) {
				std::cout << "> add subprotocol error: " << ec.message() << std::endl;
				return -1;
			}
		}

		int newId = _nextId++;
		ConnectionMetadata::ptr metadataPtr = websocketpp::lib::make_shared<ConnectionMetadata>(newId, con->get_handle(), uri, listener);
		_connectionList[newId] = metadataPtr;

		con->set_open_handler(websocketpp::lib::bind(
			&ConnectionMetadata::onOpen,
			metadataPtr,
			&_endpoint,
			websocketpp::lib::placeholders::_1
		));
		con->set_fail_handler(websocketpp::lib::bind(
			&ConnectionMetadata::onFail,
			metadataPtr,
			&_endpoint,
			websocketpp::lib::placeholders::_1
		));
		con->set_close_handler(websocketpp::lib::bind(
			&ConnectionMetadata::onClose,
			metadataPtr,
			&_endpoint,
			websocketpp::lib::placeholders::_1
		));
		con->set_message_handler(websocketpp::lib::bind(
			&ConnectionMetadata::onMessage,
			metadataPtr,
			&_endpoint,
			websocketpp::lib::placeholders::_1,
			websocketpp::lib::placeholders::_2
		));

		con->set_ping_handler(websocketpp::lib::bind(
			&ConnectionMetadata::onPing,
			metadataPtr,
			&_endpoint,
			websocketpp::lib::placeholders::_1,
			websocketpp::lib::placeholders::_2
		));

		con->set_pong_handler(websocketpp::lib::bind(
			&ConnectionMetadata::onPong,
			metadataPtr,
			&_endpoint,
			websocketpp::lib::placeholders::_1,
			websocketpp::lib::placeholders::_2
		));

		con->set_pong_timeout_handler(websocketpp::lib::bind(
			&ConnectionMetadata::onPongTimeout,
			metadataPtr,
			&_endpoint,
			websocketpp::lib::placeholders::_1,
			websocketpp::lib::placeholders::_2
		));

		_endpoint.connect(con);

		return newId;
	}

	void WebsocketEndpoint::close(int id, websocketpp::close::status::value code, const std::string& reason) {
		websocketpp::lib::error_code ec;

		ConnectionList::iterator metadataIt = _connectionList.find(id);
		if (metadataIt == _connectionList.end()) {
			std::cout << "> No connection found with id " << id << std::endl;
			return;
		}

		_endpoint.close(metadataIt->second->getHdl(), code, reason, ec);
		if (ec) {
			std::cout << "> Error initiating close: " << ec.message() << std::endl;
		}
	}

	void WebsocketEndpoint::sendText(int id, const std::string& data) {
		websocketpp::lib::error_code ec;

		ConnectionList::iterator metadataIt = _connectionList.find(id);
		if (metadataIt == _connectionList.end()) {
			std::cout << "> No connection found with id " << id << std::endl;
			return;
		}

		_endpoint.send(metadataIt->second->getHdl(), data, websocketpp::frame::opcode::text, ec);
		if (ec) {
			std::cout << "> Error sending text message: " << ec.message() << std::endl;
			return;
		}
	}

	void WebsocketEndpoint::sendBinary(int id, const std::vector<uint8_t>& data)
	{
		websocketpp::lib::error_code ec;

		ConnectionList::iterator metadataIt = _connectionList.find(id);
		if (metadataIt == _connectionList.end()) {
			std::cout << "> No connection found with id " << id << std::endl;
			return;
		}

		_endpoint.send(metadataIt->second->getHdl(), data.data(), data.size(), websocketpp::frame::opcode::binary, ec);
		if (ec) {
			std::cout << "> Error sending binary message: " << ec.message() << std::endl;
			return;
		}
	}

	void WebsocketEndpoint::sendPing(int id, const std::string& data) {
		websocketpp::lib::error_code ec;

		ConnectionList::iterator metadataIt = _connectionList.find(id);
		if (metadataIt == _connectionList.end()) {
			std::cout << "> No connection found with id " << id << std::endl;
			return;
		}

		_endpoint.send(metadataIt->second->getHdl(), data, websocketpp::frame::opcode::ping, ec);
		if (ec) {
			std::cout << "> Error sending ping message: " << ec.message() << std::endl;
			return;
		}
	}

	void WebsocketEndpoint::sendPong(int id, const std::string& data) {
		websocketpp::lib::error_code ec;

		ConnectionList::iterator metadataIt = _connectionList.find(id);
		if (metadataIt == _connectionList.end()) {
			std::cout << "> No connection found with id " << id << std::endl;
			return;
		}

		_endpoint.send(metadataIt->second->getHdl(), data, websocketpp::frame::opcode::pong, ec);
		if (ec) {
			std::cout << "> Error sending pong message: " << ec.message() << std::endl;
			return;
		}
	}

	ConnectionMetadata::ptr WebsocketEndpoint::getMetadata(int id) const {
		ConnectionList::const_iterator metadataIt = _connectionList.find(id);
		if (metadataIt == _connectionList.end()) {
			return ConnectionMetadata::ptr();
		}
		else {
			return metadataIt->second;
		}
	}
}
