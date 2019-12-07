#include "task_queue_manager.h"
#include "api/task_queue/default_task_queue_factory.h"
#include "api/task_queue/task_queue_factory.h"

namespace vi {
	TaskQueueManager::TaskQueueManager()
	{
	}

	TaskQueueManager::~TaskQueueManager()
	{
	}

	void TaskQueueManager::init()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_factory = webrtc::CreateDefaultTaskQueueFactory();

		_queuesMap[QueueName::WORKER] = _factory->CreateTaskQueue("vi::WORKER", webrtc::TaskQueueFactory::Priority::NORMAL);
	}

	webrtc::TaskQueueBase* TaskQueueManager::getQueue(QueueName name)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_queuesMap.find(name) != _queuesMap.end()) {
			return _queuesMap[name].get();
		}

		return nullptr;
	}
}
