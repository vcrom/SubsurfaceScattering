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
	std::cout << "Loading Image... " << std::endl;
	std::cout << path << std::endl;
	fipImage image; 
	bool loaded = image.load(path.c_str());
	if (loaded)
	{
		//bool converted = image.convertToFloat();
		bool converted = image.flipVertical();
		std::cout << "size: " << image.getWidth() << " " << image.getHeight() << std::endl;
		std::cout << "Line W: " << image.getLine() << std::endl;
		std::cout << "bytes per pix: " << image.getBitsPerPixel() << std::endl;
		std::cout << "line: " << image.getLine() << std::endl;
		std::cout << "size of float: " << CHAR_BIT * sizeof (float) << std::endl;
		//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
		int channels = 3;
		//float* tex_data = new float[channels*image.getWidth()*image.getHeight()];
		auto pixeles = (float*)/*static_cast<unsigned char*>*//*(float*)*/(image.accessPixels());

		for (unsigned int i = 0; i < image.getWidth()*image.getHeight(); ++i){
			std::cout << "pixel " << i << ": (";
			for (int j = 0; j < channels; ++j)
			{
				//tex_data[i * channels + j] = pixeles[i * channels + j];
				std::cout << pixeles[i * channels + j] << ", ";
			}
			std::cout << ")" << std::endl;
			//tex_data[j * channels + 0] = pixeles[j * 4 + 2];
			//tex_data[j * 4 + 1] = pixeles[j * 4 + 1];
			//tex_data[j * 4 + 2] = pixeles[j * 4 + 0];
			//tex_data[j * 4 + 3] = pixeles[j * 4 + 3];
		}
	}
	else std::cout << "ERROR" << std::endl;
}