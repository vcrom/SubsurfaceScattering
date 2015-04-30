#include "textureloader.h"

#include "fimage.h"

//TextureLoader::TextureLoader()
//{
//
//}

//TextureLoader::~TextureLoader()
//{
//
//}

#include "utils.h"
#include <iostream>


#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

Texture2D TextureLoader::Load2DTexture(const std::string& path)
{
	fImage image;
	image.loadImage(path);
	Texture2D tex(GL_TEXTURE_2D);
	tex.use();
	//tex.allocateTextureStorage(1, image.getOpenGLImageInternalFormat(), image.getWidth(), image.getHeight());
	//tex.loadImageToTexture(0, 0, 0, image.getWidth(), image.getHeight(), image.getOpenGLFormat(), image.getOpenGLImageType(), image.getImageData());
	int w = image.getWidth();
	int h = image.getHeight();
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);

	//checkCritOpenGLError();
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, w, h);
	checkCritOpenGLError();
	//glTextureSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, image.getImageData());
	//checkCritOpenGLError();

	//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
	int channels = 3;
	BYTE* tex_data = new unsigned char[channels*image.getWidth()*image.getHeight()];
	auto pixeles = static_cast<unsigned char*>(image.getImageData());

	for (unsigned int i = 0; i < image.getWidth()*image.getHeight(); ++i){
		std::cout << "pixel " << i << ": (";
		for (int j = 0; j < channels; ++j)
		{
			tex_data[i * channels + j] = pixeles[i * channels + j];
			//std::cout << unsigned int(pixeles[i * channels + j]);
			std::cout << BYTE(tex_data[i*channels + j]);
			if (j < channels - 1) std::cout << ", ";
		}
		std::cout << ")" << std::endl;
		//tex_data[j * channels + 0] = pixeles[j * 4 + 2];
		//tex_data[j * 4 + 1] = pixeles[j * 4 + 1];
		//tex_data[j * 4 + 2] = pixeles[j * 4 + 0];
		//tex_data[j * 4 + 3] = pixeles[j * 4 + 3];
	}

	glTexImage2D(GL_TEXTURE_2D,	0, GL_RGB8, w, h, 0, GL_RGB, GL_BYTE, tex_data);
	glTextureSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_BYTE, tex_data);
	checkCritOpenGLError();

	{
		std::cout << "Loading Image... " << std::endl;
		std::cout << path << std::endl;
		fipImage image;
		bool loaded = image.load(path.c_str());
		if (loaded)
		{
			//bool converted = image.convertToFloat();
			//bool converted = image.flipVertical();
			image.convertTo24Bits();
			std::cout << "size: " << image.getWidth() << " " << image.getHeight() << std::endl;
			std::cout << "Line W: " << image.getLine() << std::endl;
			std::cout << "bytes per pix: " << image.getBitsPerPixel() << std::endl;
			std::cout << "line: " << image.getLine() << std::endl;
			//std::cout << "size of float: " << CHAR_BIT * sizeof (float) << std::endl;
			//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
			int channels = 3;
			//float* tex_data = new float[channels*image.getWidth()*image.getHeight()];
			auto pixeles = static_cast<unsigned char*>(image.accessPixels());

			for (unsigned int i = 0; i < image.getWidth()*image.getHeight(); ++i){
				std::cout << "pixel " << i << ": (";
				for (int j = 0; j < channels; ++j)
				{
					//tex_data[i * channels + j] = pixeles[i * channels + j];
					std::cout << unsigned int(pixeles[i * channels + j]);
					if (j < channels-1) std::cout << ", ";
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
	return tex;
}