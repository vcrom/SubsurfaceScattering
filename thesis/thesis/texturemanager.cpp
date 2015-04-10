#include "texturemanager.h"

#include "FreeImage\FreeImagePlus.h"
#include <iostream>

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

}

void TextureManager::loadTexture(const std::string& path)
{
	fipImage image; 
	image.load(path.c_str());
	std::cout << "Size: " << image.getWidth() << " " << image.getHeight() << std::endl;
	std::cout << image.isGrayscale() << std::endl;
	std::cout << image.getBitsPerPixel() << std::endl;
}