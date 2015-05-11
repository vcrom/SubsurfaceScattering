#include "glslshadermanager.h"

#include <iostream>
#include <memory>

#include "utils.h"

GlslShaderManager::GlslShaderManager()
{
	_shaders = std::vector<GlslShader>(uint(Shaders::Count));
}

GlslShaderManager::~GlslShaderManager()
{
	deleteShaders();
}

GlslShaderManager* GlslShaderManager::instance()
{
	static GlslShaderManager *instance = new GlslShaderManager();
	return instance;
}

//GlslShaderManager& GlslShaderManager::instance()
//{
//	static GlslShaderManager *instance = new GlslShaderManager();
//	return *instance;
//}

void GlslShaderManager::initializeShaders()
{
	std::cout << "Initializeing default shaders..." << std::endl;
	initTextureToScreenShader();
}

void GlslShaderManager::deleteShaders()
{
	for (auto &shader : _shaders) {
		shader.deleteShaderProgram();
	}
	_shaders.clear();
}

GlslShader& GlslShaderManager::getShader(Shaders shader)
{
	assert(uint(shader) < _shaders.size());
	return _shaders[shader];
}

void GlslShaderManager::initTextureToScreenShader()
{

	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/screen_space_quad.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::TEXTURE_TO_SCREEN] = shader;
	std::cout << "\tTexture to screen shader initialized" << std::endl;
}

void initPassThroughShader()
{

}