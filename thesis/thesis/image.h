#ifndef IMAGE_H
#define IMAGE_H

#include <string>

//static_assert (CHAR_BIT * sizeof (float) == 32, "Size of float is not 32bits");

class Image
{
public:
	Image();
	~Image();

	virtual void loadImage(const void* buffer, unsigned int width, unsigned int height) = 0;
	virtual void loadImage(const std::string& path) = 0;
	virtual void writeImage(const std::string& path) = 0;
	virtual unsigned int getHeight() = 0;
	virtual unsigned int getWidth() = 0;
	virtual unsigned char* getImageData() = 0;

};

#endif // IMAGE_H
