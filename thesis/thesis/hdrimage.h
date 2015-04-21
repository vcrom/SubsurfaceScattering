#ifndef HDRIMAGE_H
#define HDRIMAGE_H

#include "fImage.h"

class HDRImage : public fImage
{
public:
	HDRImage();
	~HDRImage();

	void loadImage(const std::string& path);
	GLint getOpenGLFormat();
	GLint getOpenGLImageType();
	GLint getOpenGLImageInternalFormat();

private:
	
};

#endif // HDRIMAGE_H
