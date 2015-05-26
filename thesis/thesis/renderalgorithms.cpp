#include "renderalgorithms.h"

RenderAlgorithms::RenderAlgorithms()
{

}

RenderAlgorithms::~RenderAlgorithms()
{

}

GlslShaderManager *RenderAlgorithms::_shader_manager = GlslShaderManager::instance();
//const GLenum render_buff[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
GLuint RenderAlgorithms::default_buffer = 0;

bool RenderAlgorithms::checkGLEnabled(GLenum param)
{
	return glIsEnabled(param);
}

static void initialize(unsigned int num_tex);
static void resizeTextures(unsigned int w, unsigned int h);
static void resizeTexture(unsigned int id, unsigned int w, unsigned int h);

#include <iostream>
#include "screenquad.h"
#include "utils.h"
void RenderAlgorithms::renderTexture(const std::shared_ptr<FrameBuffer> fbo, const Texture2D& tex)
{
	fbo->useFrameBuffer();
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glDrawBuffers(1, render_buff);
	
	//std::cout << "Render to Tex" << std::endl;
	tex.use(GL_TEXTURE0);
	ScreenQuad* quad = ScreenQuad::getInstanceP();
	//shader 
	GlslShader* shader = _shader_manager->getShader(GlslShaderManager::Shaders::TEXTURE_TO_SCREEN);
	shader->use();
		quad->render();
	shader->unUse();
	checkCritOpenGLError();

	//glBindFramebuffer(GL_FRAMEBUFFER, RenderAlgorithms::default_buffer);
}

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void RenderAlgorithms::renderMesh(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 col)
{
	assert(RenderAlgorithms::checkGLEnabled(GL_DEPTH_TEST));
	fbo->useFrameBuffer();
	GlslShader* shader = _shader_manager->getShader(GlslShaderManager::Shaders::PASS_THROUGH_SHADER);
	shader->use();
		glUniformMatrix4fv(shader->operator()("MVP"), 1, GL_FALSE, glm::value_ptr(P*V*M));
		glUniform3fv(shader->operator()("color"), 1, glm::value_ptr(col));
		mesh->render();
	shader->unUse();
	checkCritOpenGLError();
}