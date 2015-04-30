#include "texture2d.h"
//#define NDEBUG
//#include <cassert>
#include "utils.h"

Texture2D::Texture2D() : Texture(GL_TEXTURE_2D)
{
	_width = _height = 0;
}

Texture2D::Texture2D(GLenum target) : Texture(target)
{
	_width = _height = 0;
}

Texture2D::~Texture2D()
{
}

GLsizei Texture2D::getWidth()
{
	return _width;
}

GLsizei Texture2D::getHeight()
{
	return _height;
}

#include <iostream>
void Texture2D::allocateTextureStorage(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
	assert(isBinded());
	_width = width;
	_height = height;
	_internal_format = internalformat;
	_levels = levels; //GL_RGB8UI
	glTexStorage2D(_target, levels, internalformat, width, height);
	//{GLenum err; while ((err = glGetError()) != GL_NO_ERROR) { std::cerr << "OpenGL error: " << err << std::endl; } }

}

void Texture2D::loadImageToTexture(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
	assert(isBinded());
	assert(level >= 0 && level < _levels);
	glTextureSubImage2D(_target, level, xoffset, yoffset, width, height, format, type,  pixels);
	//{GLenum err; while ((err = glGetError()) != GL_NO_ERROR) { std::cerr << "OpenGL error: " << err << std::endl; } }
}