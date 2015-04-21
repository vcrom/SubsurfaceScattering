#ifndef IMAGE_H
#define IMAGE_H

#include <string>

//static_assert (CHAR_BIT * sizeof (float) == 32, "Size of float is not 32bits");

class Image
{
public:
	Image();
	~Image();

	virtual void loadImage(const std::string& path) = 0;
	virtual unsigned int imageHeight() = 0;	
	virtual unsigned int imageWidth() = 0;
	virtual void* getImageData() = 0;

};

#endif // IMAGE_H
