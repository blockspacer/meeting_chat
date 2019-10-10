#include "ui.h"
#include <QtWidgets/QApplication>
#include "rtc_base/checks.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/win32_socket_init.h"
#include "rtc_base/win32_socket_server.h"
#include <QObject>
#include <memory>
#include "Network/network.hpp"
#include "Thread/i_task.h"
#include "Thread/i_task_scheduler.h"
#include "Thread/task_scheduler_manager.h"
#include "Service/app_instance.h"
#include "i_webrtc_service.h"

using namespace core;

//#include "x2struct/x2struct.hpp"
//
//using namespace std;
//
//struct Test {
//	int64_t id;
//	string  name;
//	XTOSTRUCT(O(id, name));
//};
//
//struct OutTest {
//	Test name;
//	int id;
//	XTOSTRUCT(O(name, id));
//};
//
//int test() {
//	OutTest t;
//	string json = "{\"name\":{\"name\":\"Pony\", \"id\":123}, \"id\":1234}";
//
//	x2struct::X::loadjson(json, t, false, true); // 4th parameter apply true
//	cout << t.xhas("id") << endl;       // use xhas to check exist
//	cout << t.xhas("name") << endl;
//	return 0;
//}
static void registerMetaTypes()
{
	qRegisterMetaType<std::function<void()>>("std::function<void()>");
	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<std::vector<std::string>>("std::vector<std::string>");

	// For Network Model
	qRegisterMetaType<std::shared_ptr<NetworkRequest>>("std::shared_ptr<NetworkRequest>");
	qRegisterMetaType<std::shared_ptr<INetworkCallback>>("std::shared_ptr<INetworkCallback>");
	qRegisterMetaType<std::shared_ptr<NetworkResponse>>("std::shared_ptr<NetworkResponse>");

	// For Thread Model
	qRegisterMetaType<int64_t>("int64_t");
	qRegisterMetaType<std::shared_ptr<ITaskScheduler>>("std::shared_ptr<ITaskScheduler>");
	qRegisterMetaType<std::shared_ptr<ITask>>("std::shared_ptr<ITask>");
}


int main(int argc, char *argv[])
{
	registerMetaTypes();

	QApplication a(argc, argv);

	rtc::InitializeSSL();

	rtc::WinsockInitializer winsock_init;
	rtc::Win32SocketServer w32_ss;
	rtc::Win32Thread w32_thread(&w32_ss);
	rtc::ThreadManager::Instance()->SetCurrentThread(&w32_thread);

	XApp->initApp();

	auto wrs = FetchService(vi::IWebRTCService);

	std::shared_ptr<UI> w = std::make_shared<UI>();
	wrs->addListener(w);
	w->show();

	int ret = a.exec();

	rtc::CleanupSSL();

	XApp->clearnup();

	rtc::ThreadManager::Instance()->CurrentThread()->Quit();

	return ret;
}
