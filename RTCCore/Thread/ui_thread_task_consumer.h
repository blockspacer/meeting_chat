#pragma once

#include "abstract_task_consumer.h"
#include <QObject>
#include "ui_task_worker.h"

namespace core {
	class ITask;
	class ITaskScheduler;

	class UIThreadTaskConsumer :
			public QObject,
			public AbstractTaskConsumer,
			public std::enable_shared_from_this<UIThreadTaskConsumer>
	{
		Q_OBJECT
	public:
		UIThreadTaskConsumer();
		~UIThreadTaskConsumer() override;

		void start() override;
		void stop() override;
		bool canTakeTask() override;

	signals:
		void post(std::shared_ptr<ITaskScheduler> scheduler, std::shared_ptr<ITask> task);

	protected:
		void notifyTaskArrived() override;
		void runTask();

	private:
		static UITaskWorker _gUITaskWorker;
	};

}
