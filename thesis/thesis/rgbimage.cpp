//#include "rgbimage.h"
//
//RGBImage::RGBImage()
//	: fImage()
//{
//
//}
//
//RGBImage::~RGBImage()
//{
//
//}
//
//void RGBImage::loadImage(const void* buffer, unsigned int width, unsigned int height)
//{
//	//fipImage (FREE_IMAGE_TYPE image_type=FIT_BITMAP, unsigned width=0, unsigned height=0, unsigned bpp=0)
//	_image = fipImage(FIT_BITMAP, width, height, 24);
//	std::memcpy(_image.accessPixels(), buffer, width * height * (24/8));
//}
//
//void RGBImage::loadImage(const std::string& path)
//{
//	fImage::loadFromPath(path);
//	bool load = _image.convertTo24Bits();
//	//bool load = _image.convertToType(FIT_BITMAP);
//
//}
//
//GLint RGBImage::getOpenGLFormat()
//{
//	return GL_RGB;
//}
//
//GLint RGBImage::getOpenGLImageType()
//{
//	return GL_UNSIGNED_BYTE;
//
//}
//
//GLint RGBImage::getOpenGLImageInternalFormat()
//{
//	return GL_RGB;
//}