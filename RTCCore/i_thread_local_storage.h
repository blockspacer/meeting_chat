#pragma once

namespace core {
	class IThreadLocalStorage {
	public:
		virtual ~IThreadLocalStorage() {}

		virtual bool hasLocalData() = 0;

		virtual void localData() = 0;
	};
}