#include "weak_proxy.h"

namespace vi {
	namespace internal {
		SynchronousMethodCall::SynchronousMethodCall(rtc::MessageHandler* proxy)
			: proxy_(proxy) {}

		SynchronousMethodCall::~SynchronousMethodCall() = default;

		void SynchronousMethodCall::Invoke(const rtc::Location& posted_from,
			std::weak_ptr<rtc::Thread> t) {
			if (auto th = t.lock()) {
				if (th->IsCurrent()) {
					proxy_->OnMessage(nullptr);
				}
				else {
					th->Post(posted_from, this, 0);
					e_.Wait(rtc::Event::kForever);
				}
			}
		}

		void SynchronousMethodCall::OnMessage(rtc::Message*) {
			proxy_->OnMessage(nullptr);
			e_.Set();
		}

	}  
}  
