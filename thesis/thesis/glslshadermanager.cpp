#include "glslshadermanager.h"

GlslShaderManager::GlslShaderManager()
{
	_shaders = std::map<std::string, GlslShader>();
}

GlslShaderManager::~GlslShaderManager()
{

}

GlslShaderManager& GlslShaderManager::instance()
{
	static GlslShaderManager *instance = new GlslShaderManager();
	return *instance;
}

void GlslShaderManager::initializeShaders()
{

}

void GlslShaderManager::deleteShaders()
{
	for (auto &it : _shaders) {
		it.second.deleteShaderProgram();
	}
	_shaders.clear();
}