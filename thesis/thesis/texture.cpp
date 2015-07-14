#include "texture.h"

//#define NDEBUG
#include <cassert>
#include "utils.h"

Texture::Texture(GLenum target) :_target(target)
{
	_levels = 0;
	_internal_format = 0;
	_id = 0;
	createTexture();
	checkCritOpenGLError();
}

Texture::~Texture()
{
	//glDeleteTextures(1, &_id);
	//checkCritOpenGLError();
	deleteTexture();
}

void Texture::createTexture()
{
	glGenTextures(1, &_id);
}

void Texture::deleteTexture()
{
	glDeleteTextures(1, &_id);
}

GLuint Texture::getTextureID()
{
	return _id;
}

GLenum Texture::getTarget()
{
	return _target;
}

GLenum Texture::getInternalFormat()
{
	return _internal_format;
}

GLsizei Texture::getLevels()
{
	return _levels;
}

void Texture::use(GLenum tex_unit) const
{
	assert(_id != 0);
	glActiveTexture(tex_unit);
	glBindTexture(_target, _id);
	checkCritOpenGLError();
	assert(isBinded());
}

void Texture::setTexParameter(GLenum pname, GLint param)
{
	assert(isBinded());
	glTexParameteri(_target, pname, param);
	checkCritOpenGLError();
}

void Texture::setTexParameter(GLenum pname, GLfloat param)
{
	assert(isBinded());
	glTexParameterf(_target, pname, param);
	checkCritOpenGLError();
}

void Texture::setTexParameter(GLenum pname, const GLint *param)
{
	assert(isBinded());
	glTexParameteriv(_target, pname, param);
	checkCritOpenGLError();
}

void Texture::setTexParameter(GLenum pname, const GLfloat *param)
{
	assert(isBinded());
	glTexParameterfv(_target, pname, param);
	checkCritOpenGLError();
}

bool Texture::isBinded() const
{
	return _id == getBindedTextured();
}

GLint Texture::getBindedTextured()
{
	GLint texture_binded;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture_binded);
	checkCritOpenGLError();
	return texture_binded;
}

void Texture::generateMipMaps()
{
	assert(isBinded());
	glGenerateMipmap(_target);
}