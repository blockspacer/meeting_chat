#include "ui.h"
#include "Service/app_instance.h"
#include "webrtc_service_interface.h"
#include "video_room.h"
#include "message_models.h"
#include "string_utils.h"
#include "callback_handlers.h"
#include "x2struct.hpp"
#include <QDebug>

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

UI::UI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.pushButtonStart, &QPushButton::clicked, this, &UI::onStartButtonClicked);
	connect(ui.pushButtonRegister, &QPushButton::clicked, this, &UI::onRegisterButtonClicked);
}

void UI::init()
{
	//_renderer = std::make_shared<GLVideoRenderer>(this);
	//_renderer->init();
	//this->setCentralWidget(_renderer.get());
	//_renderer->show();
}

void UI::onStatus(vi::ServiceStauts status)
{
	if (vi::ServiceStauts::UP == status) {

	}
}

void UI::onStartButtonClicked()
{
	if (!_vr) {
		//auto wrs = FetchService(vi::WebRTCServiceInterface);
		auto wrs = rtcApp->getWebrtcService();
		_vr = std::make_shared<vi::VideoRoom>(wrs);
	}
	_vr->attach();
}

void UI::onRegisterButtonClicked()
{
	vi::RegisterRequest request;
	request.request = "join";
	request.room = 1234;
	request.ptype = "publisher";
	request.display = ui.lineEditUserName->text().toStdString();

	if (_vr) {
		std::shared_ptr<vi::SendMessageHandler> handler = std::make_shared<vi::SendMessageHandler>();
		auto lambda = [](bool success, const std::string& message) {
			qDebug() << "message: " << message.c_str();
		};
		std::shared_ptr<vi::HandlerCallback> callback = std::make_shared<vi::HandlerCallback>(lambda);
		handler->message = x2struct::X::tojson(request);
		handler->callback = callback;
		_vr->sendMessage(handler);
	}
}
