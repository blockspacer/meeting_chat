#pragma once

#include "i_sfu_client_listener.h"
#include "weak_proxy.h"

namespace vi {
	class SFUClientListenerInterface : public ISFUClientListener {
	public:
		virtual ~SFUClientListenerInterface() {}

		virtual void init(std::shared_ptr<ISFUClientListener> listener) = 0;

		void onOpened() override {}

		void onClosed() override {}

		void onFailed(int errorCode, const std::string& reason) override {}

		void onMessage(std::shared_ptr<JanusResponse> model) override {}
	};

	class SFUClientListener : public SFUClientListenerInterface {
	public:
		~SFUClientListener() {

		}
		void init(std::shared_ptr<ISFUClientListener> listener) {
			_listener = listener;
		}

	private:
		void onOpened() override {
			if (auto listener = _listener.lock()) {
				listener->onOpened();
			}
		}

		void onClosed() override {
			if (auto listener = _listener.lock()) {
				listener->onClosed();
			}
		}

		void onFailed(int errorCode, const std::string& reason) override {
			if (auto listener = _listener.lock()) {
				listener->onFailed(errorCode, reason);
			}
		}

		void onMessage(std::shared_ptr<JanusResponse> model) override {
			if (auto listener = _listener.lock()) {
				listener->onMessage(model);
			}
		}
		std::weak_ptr<ISFUClientListener> _listener;
	};

	BEGIN_WEAK_PROXY_MAP(SFUClientListener)
		WEAK_PROXY_THREAD_DESTRUCTOR()
		WEAK_PROXY_METHOD1(void, init, std::shared_ptr<ISFUClientListener>)
		WEAK_PROXY_METHOD0(void, onOpened)
		WEAK_PROXY_METHOD0(void, onClosed)
		WEAK_PROXY_METHOD2(void, onFailed, int, const std::string&)
		WEAK_PROXY_METHOD1(void, onMessage, std::shared_ptr<JanusResponse>)
	END_WEAK_PROXY_MAP()
}