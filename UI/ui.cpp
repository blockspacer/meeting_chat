#include "ui.h"
#include "Service/app_instance.h"
#include "i_webrtc_service.h"
#include "video_room.h"
#include "message_models.h"
#include "string_utils.h"
#include "callback_handlers.h"
#include "x2struct.hpp"
#include <QDebug>

UI::UI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.pushButtonStart, &QPushButton::clicked, this, &UI::onStartButtonClicked);
	connect(ui.pushButtonRegister, &QPushButton::clicked, this, &UI::onRegisterButtonClicked);
}

void UI::onStatus(vi::WRServiceStauts status)
{
	if (vi::WRServiceStauts::UP == status) {

	}
}

void UI::onStartButtonClicked()
{
	if (!_vr) {
		auto wrs = FetchService(vi::IWebRTCService);
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
