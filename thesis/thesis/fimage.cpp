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

unsigned int fImage::imageHeight()
{
	return _image.getHeight();
}

unsigned int fImage::imageWidth()
{
	return _image.getWidth();
}

void* fImage::getImageData()
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
		_image.flipVertical();
	}
}