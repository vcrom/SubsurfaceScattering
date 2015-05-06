#ifndef GLSLSHADERMANAGER_H
#define GLSLSHADERMANAGER_H

#include <map>
#include <string>
#include "glslshader.h"

class GlslShaderManager
{
public:
	enum class Shaders : unsigned int {TEXTURE_TO_SCREEN, PASS_THROUGH_SHADER};

	static GlslShaderManager& instance();
	~GlslShaderManager();

	void initializeShaders();
	void deleteShaders();

protected:
	GlslShaderManager();

private:
	std::map<std::string, GlslShader> _shaders;
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