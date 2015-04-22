#ifndef FIMAGE_H
#define FIMAGE_H

#include "Image.h"
#include "FreeImage\FreeImagePlus.h"
#include "GL\glew.h"

class fImage : public Image
{
public:
	fImage();
	~fImage();

	virtual void loadImage(const void* buffer, unsigned int width, unsigned int height) = 0;
	virtual void loadImage(const std::string& path) = 0;
	virtual void writeImage(const std::string& path);
	unsigned int getHeight();
	unsigned int getWidth();
	virtual unsigned char* getImageData();
	virtual GLint getOpenGLFormat() = 0;
	virtual GLint getOpenGLImageType() = 0;
	virtual GLint getOpenGLImageInternalFormat() = 0;

protected:
	fipImage _image;
	void loadFromPath(const std::string& path);
};

#endif // FIMAGE_H
