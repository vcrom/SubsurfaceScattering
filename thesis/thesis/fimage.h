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

	void loadImage(const void* buffer, unsigned int width, unsigned int height);
	void loadImage(const std::string& path);
	void writeImage(const std::string& path);
	unsigned int getHeight();
	unsigned int getWidth();
	unsigned char* getImageData();
	GLint getOpenGLFormat();
	GLint getOpenGLImageType();
	GLint getOpenGLImageInternalFormat();
	void flipDiagonal();

protected:
	fipImage _image;
	void loadFromPath(const std::string& path);
	void writeImageToPath(const std::string& path);

private:
	#define THREADED
};

#endif // FIMAGE_H
