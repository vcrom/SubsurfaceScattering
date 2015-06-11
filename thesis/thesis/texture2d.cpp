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

/// <summary>
/// Loads an empty texture of size(width, height) of an internal format specified in internalformat in the texture level(level) with the specified openGl param border.
/// </summary>
/// <param name="level">Texture mipmap level.</param>
/// <param name="internalformat">The internalformat.</param>
/// <param name="width">The width.</param>
/// <param name="height">The height.</param>
/// <param name="border">The border openGl flag(must be 0) deprecated param GL_TEXTURE_BORDER_COLOR GL_CLAMP_TO_BORDER can be used to archieve the texture border effect</param>
void Texture2D::loadEmptyTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border)
{
	switch (internalformat)
	{
	//Depth textures
	case GL_DEPTH_COMPONENT16:
		loadBufferToTexture(level, internalformat, width, height, border, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
		break;
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_COMPONENT24:
		loadBufferToTexture(level, internalformat, width, height, border, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
		break;
	case GL_DEPTH_COMPONENT32F:
		loadBufferToTexture(level, internalformat, width, height, border, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		break;
	case GL_DEPTH_STENCIL:
	case GL_DEPTH24_STENCIL8:
		loadBufferToTexture(level, internalformat, width, height, border, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		break;
	case GL_DEPTH32F_STENCIL8:
		loadBufferToTexture(level, internalformat, width, height, border, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
		break;

	//RGB textures
	case GL_RGB:
		loadBufferToTexture(level, internalformat, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		break;
	case GL_RGB16F:
	case GL_RGB32F:
		loadBufferToTexture(level, internalformat, width, height, border, GL_RGB, GL_FLOAT, NULL);
		break;

	//RGBA textures
	case GL_RGBA:
		loadBufferToTexture(level, internalformat, width, height, border, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		break;
	case GL_RGBA16F:
	case GL_RGBA32F:
		loadBufferToTexture(level, internalformat, width, height, border, GL_RGB, GL_FLOAT, NULL);
		break;

	//R textures
	case GL_R8:
		loadBufferToTexture(level, internalformat, width, height, border, GL_RED, GL_UNSIGNED_BYTE, NULL);
		break;
	case GL_R8_SNORM:
		loadBufferToTexture(level, internalformat, width, height, border, GL_RED, GL_BYTE, NULL);
		break;
	case GL_R16F:
	case GL_R32F:
		loadBufferToTexture(level, internalformat, width, height, border, GL_RED, GL_FLOAT, NULL);
		break;

	//Unsized Internal limunance and alpha
	case GL_LUMINANCE_ALPHA:
		loadBufferToTexture(level, internalformat, width, height, border, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, NULL);
		break;
	case GL_LUMINANCE:
		loadBufferToTexture(level, internalformat, width, height, border, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
		break;
	case GL_ALPHA:
		loadBufferToTexture(level, internalformat, width, height, border, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);
		break;
	default:
		loadBufferToTexture(level, internalformat, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		break;
	}
}

void Texture2D::loadEmptyTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height)
{
	loadEmptyTexture(level, internalformat, width, height, 0);
}

/// <summary>
/// Loads an empty texture of size(width, height) of an internal format specified in internalformat.
/// </summary>
/// <param name="internalformat">The internalformat.</param>
/// <param name="width">The width.</param>
/// <param name="height">The height.</param>
void Texture2D::loadEmptyTexture(GLint internalformat, GLsizei width, GLsizei height)
{
	loadEmptyTexture(0, internalformat, width, height);
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

	//glTexImage2D(_target, level, internalformat, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	checkCritOpenGLError();

}

void Texture2D::loadBufferToTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels)
{
	loadBufferToTexture(level, internalformat, width, height, 0, format, type, pixels);
}

