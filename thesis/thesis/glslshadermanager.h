#ifndef GLSLSHADERMANAGER_H
#define GLSLSHADERMANAGER_H

#include <vector>

#include "glslshader.h"

class GlslShaderManager
{
	using uint = unsigned int;
public:
	enum Shaders : uint { TEXTURE_TO_SCREEN, PASS_THROUGH_SHADER, Count };

	static GlslShaderManager* instance();
	//static GlslShaderManager& instance();
	~GlslShaderManager();

	void initializeShaders();
	void deleteShaders();

	GlslShader* getShader(Shaders shader);

protected:
	GlslShaderManager();
	std::vector<GlslShader> _shaders;

private:
	void initTextureToScreenShader();
	void initPassThroughShader();
	static GlslShaderManager _instance;
};

#endif // GLSLSHADERMANAGER_H

//class FileSystem
//{
//public:
//	static FileSystem& instance();
//
//	virtual ~FileSystem() {}
//	virtual char* readFile(char* path) = 0;
//	virtual void  writeFile(char* path, char* contents) = 0;
//
//protected:
//	FileSystem() {}
//};
//The clever part is how the instance is created :
//
//FileSystem& FileSystem::instance()
//{
//#if PLATFORM == PLAYSTATION3
//	static FileSystem *instance = new PS3FileSystem();
//#elif PLATFORM == WII
//	static FileSystem *instance = new WiiFileSystem();
//#endif
//
//	return *instance;
//}