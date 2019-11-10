#pragma once

#include <memory>
#include <qopengl.h>
#include <qopenglfunctions_4_5_core.h>
#include "absl/types/optional.h"
#include "api/video/video_rotation.h"

class GLVideoShader 
	: public QOpenGLFunctions_4_5_Core
	, public std::enable_shared_from_this<GLVideoShader>
{
public:
	GLVideoShader();
	~GLVideoShader();

	bool createAndSetupI420Program();

	bool createAndSetupNV12Program();

	bool prepareVertexBuffer(webrtc::VideoRotation rotation);

	void applyShadingForFrame(int width,
		int height,
		webrtc::VideoRotation rotation,
		GLuint yPlane,
		GLuint uPlane,
		GLuint vPlane);

	void applyShadingForFrame(int width,
		int height,
		webrtc::VideoRotation rotation,
		GLuint yPlane,
		GLuint uvPlane);

protected:
	GLuint createShader(GLenum type, const GLchar* source);
	GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint createProgramFromFragmentSource(const char fragmentShaderSource[]);
	bool createVertexBuffer(GLuint* vertexBuffer, GLuint* vertexArray);
	void setVertexData(webrtc::VideoRotation rotation);

private:
	GLuint _vertexBuffer;
	GLuint _vertexArray;
	// Store current rotation and only upload new vertex data when rotation changes.
	absl::optional<webrtc::VideoRotation> _currentRotation;

	GLuint _i420Program;
	GLuint _nv12Program;
};

