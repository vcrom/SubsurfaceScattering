#include "textureloader.h"


//TextureLoader::TextureLoader()
//{
//
//}

//TextureLoader::~TextureLoader()
//{
//
//}

#include <iostream>

#include "fimage.h"
#include "utils.h"

Texture2D* TextureLoader::Create2DTexture(const std::string& path)
{
	Texture2D* tex = Create2DTexture(path, GL_RGB8);
	return tex;
}

Texture2D* TextureLoader::Create2DTexture(const std::string& path, GLint internalformat)
{
	fImage image;
	image.loadImage(path);

	Texture2D* tex = new Texture2D(GL_TEXTURE_2D);
	tex->createTexture();
	tex->use();
	//TODO: adapt the parameters for each image automatically
	tex->loadBufferToTexture(0, internalformat, image.getWidth(), image.getHeight(), GL_BGR, GL_UNSIGNED_BYTE, image.getImageData());
	return tex;
}