#include "gl_video_renderer.h"
//#include <QOpenGLShaderProgram>
//#include <QOpenGLShader>
//#include <QOpenGLTexture>
#include "gl_video_shader.h"
#include "i420_texture_cache.h"
#include <QDebug>
#include <thread>
#include "absl/types/optional.h"
#include "api/video/video_rotation.h"
#include <array>
#include "common_video/libyuv/include/webrtc_libyuv.h"

GLVideoRenderer::GLVideoRenderer(QWidget *parent)
	: QOpenGLWidget(parent)
{
	//_timer = new QTimer(this);
	//connect(_timer, &QTimer::timeout, this, &GLVideoRenderer::timeout);
	//_timer->setInterval(40);
	//_timer->start();
}

GLVideoRenderer::~GLVideoRenderer()
{
	// Make sure the context is current and then explicitly
	// destroy all underlying OpenGL resources.
	//makeCurrent();

	////delete _texture;
	////delete _shader;
	////delete _program;

	//doneCurrent();
}

void GLVideoRenderer::init()
{
	_videoShader = std::make_shared<GLVideoShader>();
}

void GLVideoRenderer::initializeGL() 
{
	initializeOpenGLFunctions();
	glewInit();
	glEnable(GL_DEPTH_TEST);
	// Set up the rendering context, load shaders and other resources, etc.:
	//QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
}

void GLVideoRenderer::resizeGL(int w, int h)
{
	std::lock_guard<std::mutex> lock(_mutex);
	// Update projection matrix and other size related settings:
	if (_frame) {
		glViewport(0, 0, _frame->width(), _frame->height());
	}
	else {
		glViewport(0, 0, 640, 480);
	}
}

void GLVideoRenderer::paintGL()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPointSize(10.0);
	//glColor3d(1.0, 0.0, 0.0);
	//glBegin(GL_POINTS);
	//glVertex3d(0.0, 0.0, 0.0);
	//glEnd();

	if (!_i420TextureCache) {
		_i420TextureCache = std::make_shared<I420TextureCache>();
		_i420TextureCache->init();
	}
	std::lock_guard<std::mutex> lock(_mutex);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	if (_frame) {
		_i420TextureCache->uploadFrameToTextures(*_frame);
		_videoShader->applyShadingForFrame(_frame->width(),
			_frame->height(),
			_frame->rotation(),
			_i420TextureCache->yTexture(),
			_i420TextureCache->uTexture(),
			_i420TextureCache->vTexture());
	}
}

void GLVideoRenderer::OnFrame(const webrtc::VideoFrame& frame)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_frame = std::make_shared<webrtc::VideoFrame>(frame);
	static int counter = 0;
	qDebug() << "--> frame:" << ++counter << "ts:" << _frame->timestamp_us();
	//static int flag = 0;
	//if (++flag == 10) {
	//	update();
	//}
	
	//std::string output_file_name = "frame.yuv";
	//FILE* output_file = fopen(output_file_name.c_str(), "wb");
	//assert(output_file != NULL);
	//webrtc::PrintVideoFrame(frame, output_file);
	//fclose(output_file);
	//Sleep(5000);
	update();
}

void GLVideoRenderer::timeout()
{
	//update();
}

void GLVideoRenderer::resizeEvent(QResizeEvent *event)
{
	QOpenGLWidget::resizeEvent(event);
}
