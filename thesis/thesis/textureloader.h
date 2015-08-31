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

	static std::shared_ptr<Texture2D> create2DTexture(const std::string& path);
	static std::shared_ptr<Texture2D> create2DTexture(const std::string& path, GLint internalformat);
	static std::shared_ptr<Texture2D> createCubeMap(const std::string& path);
	static std::shared_ptr<Texture2D> createCubeMap(const std::string& path, GLint internalformat);

private:
	
};

#endif // TEXTURELOADER_H
