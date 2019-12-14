#pragma once

#include "i_video_room_listener.h"
#include "weak_proxy.h"
#include <algorithm>
#include <list>

namespace vi {
	class VideoRoomListenerInterface : public IVideoRoomListener {
	public:
		virtual ~VideoRoomListenerInterface() {}

		virtual void attach(std::shared_ptr<IVideoRoomListener> listener) = 0;

		virtual void detach(std::shared_ptr<IVideoRoomListener> listener) = 0;

		virtual void detachAll() = 0;

		void onCreateParticipant(std::shared_ptr<Participant> participant) override {}

		void onUpdateParticipant(std::shared_ptr<Participant> participant) override {}

		void onRemoveParticipant(std::shared_ptr<Participant> participant) override {}
	};

	class VideoRoomListener : public VideoRoomListenerInterface {
	public:
		~VideoRoomListener() {

		}

		void attach(std::shared_ptr<IVideoRoomListener> listener) override {
			if (!exist(listener)) {
				_listeners.emplace_back(listener);
			}
		}

		void detach(std::shared_ptr<IVideoRoomListener> listener) override {
			_listeners.remove_if([listener](const std::weak_ptr<IVideoRoomListener>& l) {
				return listener == l.lock();
			});
		}

		void detachAll() override {
			_listeners.clear();
		}

	private:
		void onCreateParticipant(std::shared_ptr<Participant> participant) override {
			for (const auto& listener : _listeners) {
				if (auto l = listener.lock()) {
					l->onCreateParticipant(participant);
				}
			}
		}

		void onUpdateParticipant(std::shared_ptr<Participant> participant) override {
			for (const auto& listener : _listeners) {
				if (auto l = listener.lock()) {
					l->onUpdateParticipant(participant);
				}
			}
		}

		void onRemoveParticipant(std::shared_ptr<Participant> participant) override {
			for (const auto& listener : _listeners) {
				if (auto l = listener.lock()) {
					l->onRemoveParticipant(participant);
				}
			}
		}

	private:
		bool exist(std::shared_ptr<IVideoRoomListener> listener) {
			return std::any_of(_listeners.begin(), _listeners.end(), [listener](const std::weak_ptr<IVideoRoomListener>& l) {
				return listener == l.lock();
			});
		}
	private:
		std::list<std::weak_ptr<IVideoRoomListener>> _listeners;
	};

	BEGIN_WEAK_PROXY_MAP(VideoRoomListener)
		WEAK_PROXY_THREAD_DESTRUCTOR()
		WEAK_PROXY_METHOD1(void, attach, std::shared_ptr<IVideoRoomListener>)
		WEAK_PROXY_METHOD1(void, detach, std::shared_ptr<IVideoRoomListener>)
		WEAK_PROXY_METHOD0(void, detachAll)
		WEAK_PROXY_METHOD1(void, onCreateParticipant, std::shared_ptr<Participant>)
		WEAK_PROXY_METHOD1(void, onUpdateParticipant, std::shared_ptr<Participant>)
		WEAK_PROXY_METHOD1(void, onRemoveParticipant, std::shared_ptr<Participant>)
	END_WEAK_PROXY_MAP()
}