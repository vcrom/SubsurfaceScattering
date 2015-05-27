#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <memory>

#include "texture2d.h"

class TextureLoader
{
public:
	TextureLoader() = delete;
	~TextureLoader() = delete;

	static std::shared_ptr<Texture2D> Create2DTexture(const std::string& path);
	static std::shared_ptr<Texture2D> Create2DTexture(const std::string& path, GLint internalformat);
	//static Texture2D LoadCubemap(const std::string& path);

private:
	
};

#endif // TEXTURELOADER_H
