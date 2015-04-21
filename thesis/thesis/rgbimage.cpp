#include "rgbimage.h"

RGBImage::RGBImage()
	: fImage()
{

}

RGBImage::~RGBImage()
{

}

void RGBImage::loadImage(const std::string& path)
{
	fImage::loadFromPath(path);
	_image.convertTo24Bits();
}

GLint RGBImage::getOpenGLFormat()
{
	return GL_RGB;
}

GLint RGBImage::getOpenGLImageType()
{
	return GL_UNSIGNED_BYTE;

}

GLint RGBImage::getOpenGLImageInternalFormat()
{
	return GL_RGB;
}