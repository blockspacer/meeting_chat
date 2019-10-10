#pragma once

#include <string>
#include <vector>
#include <functional>
#include "message_models.h"

namespace vi {
	using JCCallback = std::function<void(std::shared_ptr<JanusResponse> model)>;
	
	class IMessageTransportListener;

	struct JCHandler {
		JCHandler(std::string trans, std::shared_ptr<JCCallback> cb)
		: transaction(trans)
		, callback(cb) {

		}

		bool valid() {
			return !transaction.empty() && nullptr != callback;
		}

		std::string transaction;
		std::shared_ptr<JCCallback> callback;
	};

	class IMessageTransportor {
	public:
		virtual ~IMessageTransportor() {}

		virtual void addListener(std::shared_ptr<IMessageTransportListener> listener) = 0;

		virtual void removeListener(std::shared_ptr<IMessageTransportListener> listener) = 0;

		virtual void connect(const std::string& url) = 0;

		virtual void disconnect() = 0;

		virtual void send(const std::string& data, std::shared_ptr<JCHandler> handler) = 0;

		virtual void send(const std::vector<uint8_t>& data, std::shared_ptr<JCHandler> handler) = 0;

	};
}
