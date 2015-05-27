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

std::shared_ptr<Texture2D> TextureLoader::Create2DTexture(const std::string& path)
{
	std::shared_ptr<Texture2D> tex = Create2DTexture(path, GL_RGB8);
	return tex;
}

std::shared_ptr<Texture2D> TextureLoader::Create2DTexture(const std::string& path, GLint internalformat)
{
	fImage image;
	image.loadImage(path);

	std::shared_ptr<Texture2D> tex = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	//tex->createTexture();
	tex->use();
	//TODO: adapt the parameters for each image automatically
	tex->loadBufferToTexture(0, internalformat, image.getWidth(), image.getHeight(), GL_BGR, GL_UNSIGNED_BYTE, image.getImageData());
	return tex;
}