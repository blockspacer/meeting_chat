#include "gl_video_renderer.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include "gl_video_shader.h"
#include "i420_texture_cache.h"


GLVideoRenderer::GLVideoRenderer(QWidget *parent)
	: QOpenGLWidget(parent)
{
	_videoShader = std::make_shared<GLVideoShader>();
}

GLVideoRenderer::~GLVideoRenderer()
{
	// Make sure the context is current and then explicitly
	// destroy all underlying OpenGL resources.
	makeCurrent();

	//delete _texture;
	//delete _shader;
	//delete _program;

	doneCurrent();
}

void GLVideoRenderer::initializeGL() 
{
	// Set up the rendering context, load shaders and other resources, etc.:
	//QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GLVideoRenderer::resizeGL(int w, int h)
{
	// Update projection matrix and other size related settings:
	webrtc::VideoFrame frame(_buffer, _rotation, _timestamp_us);
	glViewport(0, 0, frame.width(), frame.height());
}

void GLVideoRenderer::paintGL()
{
	// Draw the scene:
	//QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	glClear(GL_COLOR_BUFFER_BIT);

	if (!_i420TextureCache) {
		_i420TextureCache = std::shared_ptr<I420TextureCache>();
	}

	webrtc::VideoFrame frame(_buffer, _rotation, _timestamp_us);
	_i420TextureCache->uploadFrameToTextures(frame);

	_videoShader->applyShadingForFrame(frame.width(),
		frame.height(),
		frame.rotation(),
		_i420TextureCache->yTexture(),
		_i420TextureCache->uTexture(),
		_i420TextureCache->vTexture());
}

void GLVideoRenderer::OnFrame(const webrtc::VideoFrame& frame)
{
	_buffer = frame.video_frame_buffer();
	_rotation = frame.rotation();
	_timestamp_us = frame.timestamp_us();
	update();
}