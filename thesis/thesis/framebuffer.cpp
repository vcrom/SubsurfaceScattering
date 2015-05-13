#include "framebuffer.h"

#include "utils.h"

const GLenum render_buff[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7, GL_COLOR_ATTACHMENT8, GL_COLOR_ATTACHMENT9, GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11, GL_COLOR_ATTACHMENT12 };


FrameBuffer::FrameBuffer()
{
	_id = 0;
	_color_levels = 0;
}

FrameBuffer::FrameBuffer(int id_buff, unsigned int color_levels) : _id(id_buff), _color_levels(color_levels)
{
	//assert(_id != 0);
}

FrameBuffer::~FrameBuffer()
{
	int a = 0;
	a = a + 2;
}

void FrameBuffer::createFrameBuffer()
{
	glGenFramebuffers(1, &_id);
	checkCritOpenGLError();
}

void FrameBuffer::deleteFrameBuffer()
{
	glDeleteBuffers(1, &_id);
	checkCritOpenGLError();
}

void FrameBuffer::useFrameBuffer()
{
	assert(_id != 0);
	FrameBuffer::useFrameBuffer(_id);
	glDrawBuffers(_color_levels, render_buff);
	checkCritOpenGLError();
}

GLuint FrameBuffer::getFrameBufferID()
{
	return _id;
}

GLuint FrameBuffer::getColorLevels()
{
	return _color_levels;
}

#include <algorithm>
void FrameBuffer::colorBuffer(GLuint tex, unsigned int attachment, GLint level)
{
#ifdef _DEBUG
	GLint max_color;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_color);
	assert(level < max_color);
#endif
	max_color = std::max(unsigned int(max_color), unsigned int(attachment+1));
	attachTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, tex, level);
	checkCritOpenGLError();
}

void FrameBuffer::depthBuffer(GLuint tex, GLint level)
{
	attachTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex, level);
}

//raw opengl
void FrameBuffer::attachTexture(GLenum target, GLenum attachment, GLuint texture, GLint level)
{
	debugAsserts();
	glFramebufferTexture(target, attachment, texture, level);
	//glFramebufferTexture2D(target, attachment, GL_TEXTURE_2D, texture, level);
	checkCritOpenGLError();
}

void FrameBuffer::attachRenderBuffer(GLenum attachment, GLuint renderbuffer)
{
	//TODO
}

void FrameBuffer::debugAsserts()
{
	assert(_id != 0);
	assert(isBinded());
}

bool FrameBuffer::checkStatus()
{
	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	return status == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::useFrameBuffer(GLuint fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffers(1, render_buff);
}
bool FrameBuffer::isBinded() const
{
	GLint idx_fbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &idx_fbo);
	return idx_fbo == _id;
}