#pragma once

#include <memory>
#include <QOpenGLWidget>
#include <qopenglfunctions_4_5_core.h>
#include <QOpenGLBuffer>
#include "api/video/video_sink_interface.h"
#include "api/video/video_frame.h"

//class QOpenGLShaderProgram;
//class QOpenGLShader;
//class QOpenGLTexture;
class GLVideoShader;
class I420TextureCache;

class GLVideoRenderer 
	: public QOpenGLWidget
	, public QOpenGLFunctions_4_5_Core
	, public rtc::VideoSinkInterface<webrtc::VideoFrame>
{
	Q_OBJECT

public:
	GLVideoRenderer(QWidget *parent);
	~GLVideoRenderer();

protected:
	void initializeGL() override;

	void resizeGL(int w, int h) override;

	void paintGL() override;

	void OnFrame(const webrtc::VideoFrame& frame) override;

private:
	std::shared_ptr<GLVideoShader> _videoShader;
	std::shared_ptr<I420TextureCache> _i420TextureCache;
	rtc::scoped_refptr<webrtc::VideoFrameBuffer> _buffer;
	webrtc::VideoRotation _rotation;
	int64_t _timestamp_us;
	//QOpenGLShaderProgram* _program;
	//QOpenGLShader* _shader;
	//QOpenGLTexture* _texture;
};
