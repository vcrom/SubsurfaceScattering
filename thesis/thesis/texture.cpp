#include "texture.h"

//#define NDEBUG
#include <cassert>

Texture::Texture(GLenum target) :_target(target)
{
	_levels = 0;
	_internal_format = 0;
	_id = 0;
	glGenTextures(1, &_id);
	assert(target == _target);
}

Texture::~Texture()
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

void Texture::use(GLenum tex_unit)
{
	glActiveTexture(tex_unit);
	glBindTexture(_target, _id);
	assert(isBinded());
}

void Texture::setTexParameter(GLenum pname, GLint param)
{
	assert(isBinded());
	glTexParameteri(_target, pname, param);
}

void Texture::setTexParameter(GLenum pname, GLfloat param)
{
	assert(isBinded());
	glTexParameterf(_target, pname, param);
}

//void Texture::allocateTextureStorage(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
//{
//	assert(isBinded());
//	glTexStorage2D(_target, levels, internalformat, width, height);
//}

bool Texture::isBinded()
{
	GLint texture_binded;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture_binded);
	return texture_binded == _id;
}