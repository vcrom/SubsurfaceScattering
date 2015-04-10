#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include<string>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	static void loadTexture(const std::string& path);

private:
	
};

#endif // TEXTUREMANAGER_H
