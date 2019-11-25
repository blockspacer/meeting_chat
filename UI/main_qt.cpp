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
#include <QSurfaceFormat>

#include "api/media_stream_interface.h"
#include "api/create_peerconnection_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "modules/audio_device/include/audio_device.h"
#include "modules/audio_processing/include/audio_processing.h"
#include "modules/video_capture/video_capture_factory.h"
#include "pc/video_track_source.h"
#include "local_video_capture.h"
#include "gl_video_renderer.h"
#include <QDebug>

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

static void initOpenGL() {
	//QSurfaceFormat format;
	//format.setDepthBufferSize(24);
	//format.setStencilBufferSize(8);
	//format.setVersion(4, 4);
	//format.setProfile(QSurfaceFormat::CoreProfile);
	//QSurfaceFormat::setDefaultFormat(format);
}

int main(int argc, char *argv[])
{
	registerMetaTypes();

	rtc::WinsockInitializer winsock_init;
	rtc::Win32SocketServer w32_ss;
	rtc::Win32Thread w32_thread(&w32_ss);
	rtc::ThreadManager::Instance()->SetCurrentThread(&w32_thread);

	rtc::InitializeSSL();

	//QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QApplication a(argc, argv);
	initOpenGL();

	const GLubyte* v = glGetString(GL_VERSION);

	XApp->initApp();

	auto wrs = FetchService(vi::IWebRTCService);

	std::shared_ptr<UI> w = std::make_shared<UI>();
	wrs->addListener(w);
	w->show();

	w->init();

	//rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _pcf;

	//_pcf = webrtc::CreatePeerConnectionFactory(
	//	nullptr /* network_thread */,
	//	nullptr /* worker_thread */,
	//	nullptr /* signaling_thread */,
	//	nullptr /* default_adm */,
	//	webrtc::CreateBuiltinAudioEncoderFactory(),
	//	webrtc::CreateBuiltinAudioDecoderFactory(),
	//	webrtc::CreateBuiltinVideoEncoderFactory(),
	//	webrtc::CreateBuiltinVideoDecoderFactory(),
	//	nullptr /* audio_mixer */,
	//	nullptr /* audio_processing */);

	//rtc::scoped_refptr<vi::CapturerTrackSource> videoDevice = vi::CapturerTrackSource::Create();
	//if (videoDevice) {
	//	rtc::scoped_refptr<webrtc::VideoTrackInterface> videoTrack(
	//		_pcf->CreateVideoTrack("video_label", videoDevice));
	//	rtc::VideoSinkWants wants;
	//	videoTrack->AddOrUpdateSink(w->_renderer.get(), wants);
	//}

	int ret = a.exec();

	XApp->clearnup();

	rtc::CleanupSSL();

	return ret; 
}
