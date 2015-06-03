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

std::shared_ptr<GlslShaderManager> GlslShaderManager::instance()
{
	static std::shared_ptr<GlslShaderManager> instance = std::make_shared<GlslShaderManager>(GlslShaderManager());
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
	initPassThroughShader();
	initShadowsAndDiffuseShader();
	initLinealDeepthShader();
	//initThicknessShader();
}

void GlslShaderManager::deleteShaders()
{
	std::cout << "Deleteing default shaders..." << std::endl;
	for (auto &shader : _shaders) {
		shader.deleteShaderProgram();
	}
	_shaders.clear();
}

void GlslShaderManager::reloadShaders()
{
	std::cout << "Reloading default shaders..." << std::endl;
	deleteShaders();
	initializeShaders();
}

std::shared_ptr<GlslShader> GlslShaderManager::getShader(Shaders shader)
{
	assert(uint(shader) < _shaders.size());
	return std::make_shared<GlslShader>(_shaders[shader]);
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

void GlslShaderManager::initPassThroughShader()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/pass_through_shader.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/pass_through_shader.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addAttribute("vNormal");
	shader.addUniform("MVP");
	shader.addUniform("color");
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::PASS_THROUGH] = shader;
	std::cout << "\tPass Through shader initialized" << std::endl;
}

void GlslShaderManager::initShadowsAndDiffuseShader()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/diffuse_and_shadows.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/diffuse_and_shadows.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addAttribute("vNormal");
	shader.addUniform("MVP");
	shader.addUniform("MV");
	shader.addUniform("M");
	shader.addUniform("N");
	shader.addUniform("S");
	shader.addUniform("eye_light_position");
	shader.addUniform("shadow_map");
	glUniform1i(shader("shadow_map"), 0);
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::SHADOWS_AND_DIFFUSE] = shader;
	std::cout << "\tShadows and diffuse shader initialized" << std::endl;
}

void GlslShaderManager::initThicknessShader()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/thickness_shader.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/thickness_shader.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addUniform("MVP");
	shader.addUniform("MV");
	shader.addUniform("z_far");
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::THICKNESS_SHADER] = shader;
	std::cout << "\tthickness shader initialized" << std::endl;
}

void GlslShaderManager::initLinealDeepthShader()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/linear_depth_shader.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/linear_depth_shader.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addUniform("MVP");
	shader.addUniform("MV");
	shader.addUniform("z_far");
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::LINEAL_DEEPTH] = shader;
	std::cout << "\tLineal deepth shader initialized" << std::endl;
}