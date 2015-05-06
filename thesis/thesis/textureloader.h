#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>

#include "texture2d.h"

class TextureLoader
{
public:
	TextureLoader() = delete;
	~TextureLoader() = delete;

	static Texture2D* Create2DTexture(const std::string& path);
	static Texture2D* Create2DTexture(const std::string& path, GLint internalformat);
	//static Texture2D LoadCubemap(const std::string& path);

private:
	
};

#endif // TEXTURELOADER_H
