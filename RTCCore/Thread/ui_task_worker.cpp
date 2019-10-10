#include "ui_task_worker.h"
#include "i_task.h"
#include "i_task_scheduler.h"

namespace core {

	void UITaskWorker::processTask(std::shared_ptr<ITaskScheduler> scheduler, std::shared_ptr<ITask> task) {
		if (task && !task->canceled()) {
			task->execute();
		}
		if (scheduler) {
			scheduler->onTaskCompleted(task);
		}
		emit finished(task);
	}

}
