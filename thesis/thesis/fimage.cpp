#include "fimage.h"

#include "utils.h"

fImage::fImage()
	: Image()
{

}

fImage::~fImage()
{
	_image.clear();
}

void fImage::writeImage(const std::string& path)
{
	bool b = _image.save(path.c_str());
	if (!b)
	{
		throw_non_critical("FAILED TO SAVE");
	}
}

unsigned int fImage::getHeight()
{
	return _image.getHeight();
}

unsigned int fImage::getWidth()
{
	return _image.getWidth();
}

unsigned char* fImage::getImageData()
{
	return _image.accessPixels();
}

void  fImage::loadImage(const void* buffer, unsigned int width, unsigned int height)
{
	////fipImage (FREE_IMAGE_TYPE image_type=FIT_BITMAP, unsigned width=0, unsigned height=0, unsigned bpp=0)
	_image = fipImage(FIT_BITMAP, width, height, 24);
	std::memcpy(_image.accessPixels(), buffer, width * height * (24 / 8));
}

void  fImage::loadImage(const std::string& path)
{
	loadFromPath(path);
	//bool load = _image.convertTo24Bits();
	//bool load = _image.convertToType(FIT_BITMAP);

}

void fImage::loadFromPath(const std::string& path)
{
	bool loaded = _image.load(path.c_str());
	if (!loaded)
	{
		throw_non_critical("Can not load the image " + path);
	}
	else
	{
		//_image.flipVertical();
	}
}

GLint fImage::getOpenGLFormat()
{
	return GL_RGB;
}

GLint fImage::getOpenGLImageType()
{
	return GL_UNSIGNED_BYTE;

}

GLint fImage::getOpenGLImageInternalFormat()
{
	return GL_RGB8;
}