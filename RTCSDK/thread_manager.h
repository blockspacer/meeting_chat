#pragma once

#include "rtc_base/thread.h"
#include <unordered_map>
#include <mutex>

namespace vi {
	enum class ThreadName : int{
		MAIN = 0,
		WORKER = 1,
		WEBRTC_SERVICE = 2,
		VIDEO_ROOM = 3
	};

	class ThreadManager
	{
	public:
		ThreadManager();

		~ThreadManager();

		void init();

		rtc::Thread* getMainThread();

		std::shared_ptr<rtc::Thread> getThread(ThreadName name);

	private:
		std::unordered_map<ThreadName, std::shared_ptr<rtc::Thread>> _threadsMap;
		
		std::mutex _mutex;

		rtc::Thread* _mainThread = nullptr;
	};
}
