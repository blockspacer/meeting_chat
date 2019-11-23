#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ui.h"
#include <memory>
#include "i_webrtc_service_listener.h"
#include "video_room.h"

#include "gl_video_renderer.h"
#include "api/create_peerconnection_factory.h"

class UI
	: public QMainWindow
	, public vi::IWebRTCServiceListener
	, public std::enable_shared_from_this<UI>
{
	Q_OBJECT

public:
	UI(QWidget *parent = Q_NULLPTR);

	void init();

	std::shared_ptr<GLVideoRenderer> _renderer;

private:
	// IWebRTCServiceListener
	void onStatus(vi::WRServiceStauts status) override;

private slots:
	void onStartButtonClicked();

	void onRegisterButtonClicked();

private:
	Ui::UIClass ui;

	std::shared_ptr<vi::VideoRoom> _vr;

	//std::shared_ptr<GLVideoRenderer> _renderer;
};
