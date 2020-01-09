#include "ui.h"
#include "Service/app_instance.h"
#include "webrtc_service_interface.h"
#include "video_room.h"
#include "message_models.h"
#include "string_utils.h"
#include "webrtc_service_events.h"
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
#include "participant.h"

UI::UI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionStart, &QAction::triggered, this, &UI::onActionStartTriggered);
	connect(ui.actionRegister, &QAction::triggered, this, &UI::onActionRegisterTriggered);
}

UI::~UI()
{
	if (_galleryView) {
		_galleryView->removeAll();
	}
}

void UI::init()
{
	_galleryView = new GalleryView(this);
	setCentralWidget(_galleryView);
}

void UI::onStatus(vi::ServiceStauts status)
{
	if (vi::ServiceStauts::UP == status) {

	}
}

void UI::onCreateParticipant(std::shared_ptr<vi::Participant> participant)
{

}

void UI::onUpdateParticipant(std::shared_ptr<vi::Participant> participant)
{

}

void UI::onDeleteParticipant(std::shared_ptr<vi::Participant> participant)
{

}

void UI::onCreateStream(int64_t pid, rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
	rtc::VideoSinkWants wants;
	for (auto track : stream->GetVideoTracks()) {
		GLVideoRenderer* renderer = new GLVideoRenderer(_galleryView);
		renderer->init();
		renderer->show();

		// TODO:
		static int64_t id = -1;
		std::shared_ptr<ContentView> view = std::make_shared<ContentView>(++id, track, renderer);
		view->init();

		_galleryView->insertView(view);
	}
}

void UI::onDeleteStream(int64_t pid, rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
}

void UI::onActionStartTriggered()
{
	if (!_vr) {
		//auto wrs = FetchService(vi::WebRTCServiceInterface);
		auto wrs = rtcApp->getWebrtcService();
		_vr = std::make_shared<vi::VideoRoom>(wrs);
		_vr->init();
		_vr->addListener(shared_from_this());
	}
	_vr->attach();
}

void UI::onActionNameTriggered()
{

}

void UI::onActionRegisterTriggered()
{
	vi::RegisterRequest request;
	request.request = "join";
	request.room = 1234;
	request.ptype = "publisher";
	request.display = "dfaa";// ui.lineEditUserName->text().toStdString();

	if (_vr) {
		std::shared_ptr<vi::SendMessageEvent> event = std::make_shared<vi::SendMessageEvent>();
		auto lambda = [](bool success, const std::string& message) {
			qDebug() << "message: " << message.c_str();
		};
		std::shared_ptr<vi::EventCallback> callback = std::make_shared<vi::EventCallback>(lambda);
		event->message = x2struct::X::tojson(request);
		event->callback = callback;
		_vr->sendMessage(event);
	}
}

void UI::closeEvent(QCloseEvent* event)
{
	//_vr->hangup(true);
	if (_galleryView) {
		_galleryView->removeAll();
	}
}

