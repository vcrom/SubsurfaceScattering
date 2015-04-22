#ifndef RGBIMAGE_H
#define RGBIMAGE_H

#include "fImage.h"

class RGBImage : public fImage
{
public:
	RGBImage();
	~RGBImage();

	void loadImage(const void* buffer, unsigned int width, unsigned int height);
	void loadImage(const std::string& path);
	GLint getOpenGLFormat();
	GLint getOpenGLImageType();
	GLint getOpenGLImageInternalFormat();

private:
	
};

#endif // RGBIMAGE_H
