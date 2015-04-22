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