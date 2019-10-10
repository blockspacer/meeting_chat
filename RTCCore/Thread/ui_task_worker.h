#pragma once

#include <QObject>
#include <memory>

namespace core {
	class ITask;
	class ITaskScheduler;
	class UITaskWorker : public QObject
	{
		Q_OBJECT
	public:
		UITaskWorker() {}
		~UITaskWorker() {}

	signals:
		void finished(std::shared_ptr<ITask> task);

	public slots:
		void processTask(std::shared_ptr<ITaskScheduler> scheduler, std::shared_ptr<ITask> task);
	};
}

