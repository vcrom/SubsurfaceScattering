#include "textureloader.h"

#include "rgbimage.h"
#include "hdrimage.h"

//TextureLoader::TextureLoader()
//{
//
//}

//TextureLoader::~TextureLoader()
//{
//
//}

#include "utils.h"
Texture2D TextureLoader::Load2DTexture(const std::string& path)
{
	//fImage image;
	RGBImage image;
	image.loadImage(path);
	Texture2D tex(GL_TEXTURE_2D);
	tex.allocateTextureStorage(1, image.getOpenGLImageInternalFormat(), image.getWidth(), image.getHeight());
	tex.loadImageToTexture(0, 0, 0, image.getWidth(), image.getHeight(), image.getOpenGLFormat(), image.getOpenGLImageType(), image.getImageData());
	checkCritOpenGLError();
	return tex;
}