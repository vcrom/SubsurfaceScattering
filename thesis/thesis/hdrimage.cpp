//#include "hdrimage.h"
//
//HDRImage::HDRImage()
//	: fImage()
//{
//
//}
//
//HDRImage::~HDRImage()
//{
//
//}
//
//void HDRImage::loadImage(const void* buffer, unsigned int width, unsigned int height)
//{
//	_image = fipImage(FIT_RGBF, width, height, 96);
//	std::memcpy(_image.accessPixels(), buffer, width * height * (96 / 8));
//}
//
//void HDRImage::loadImage(const std::string& path)
//{
//	fImage::loadFromPath(path);
//	_image.convertToRGBF();
//}
//
//GLint HDRImage::getOpenGLFormat()
//{
//	return GL_RGB;
//}
//
//GLint HDRImage::getOpenGLImageType()
//{
//	return GL_FLOAT;
//}
//
//GLint HDRImage::getOpenGLImageInternalFormat()
//{
//	return GL_RGB32F;
//}