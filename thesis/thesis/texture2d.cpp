#include "texture2d.h"
//#define NDEBUG
//#include <cassert>
#include "utils.h"
#include <algorithm>

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

#include <chrono>
#include <iostream>

unsigned char* Texture2D::getTextureData()
{
	return getTextureData(0, GL_BGR, GL_UNSIGNED_BYTE);
}

unsigned char* Texture2D::getTextureData(GLint level, GLenum format, GLenum type, unsigned int channels)
{
	unsigned int n_pixels = getWidth()*getHeight() * channels;
	unsigned char *pixels = new unsigned char[n_pixels];

	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();

	//TODO: automatically choose format according to cannels
	glGetTexImage(GL_TEXTURE_2D, level, format, type, pixels);

	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed_mseconds = end - start;
	std::cout << "TEX time obtaining the texture: " << elapsed_mseconds.count() << std::endl;
	return pixels;
}

void Texture2D::resize(GLsizei width, GLsizei height)
{
	_width = width;
	_height = height;
	for (unsigned int i = 0; i < _levels; ++i)
	{
		loadEmptyTexture(i, _internal_format, width, height);
		width = std::max(1, (width/2));
		height = std::max(1, (height/2));
	}
}

void Texture2D::loadEmptyTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border)
{
	loadBufferToTexture(level, internalformat, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Texture2D::loadEmptyTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height)
{
	loadEmptyTexture(level, internalformat, width, height, 0);
}

void Texture2D::loadBufferToTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
	assert(isBinded());

	_width = width;
	_height = height;
	_levels = std::max( level + 1, _levels);
	_internal_format = internalformat;
	_border = border;
	//default params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//create/init openGl texture
	glTexImage2D(_target, level, internalformat, width, height, border, format, type, pixels);
	checkCritOpenGLError();

}

void Texture2D::loadBufferToTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels)
{
	loadBufferToTexture(level, internalformat, width, height, 0, format, type, pixels);
}

