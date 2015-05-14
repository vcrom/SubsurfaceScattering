#include "renderalgorithms.h"

RenderAlgorithms::RenderAlgorithms()
{

}

RenderAlgorithms::~RenderAlgorithms()
{

}

GlslShaderManager *RenderAlgorithms::shader_manager = GlslShaderManager::instance();
const GLenum render_buff[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
GLuint RenderAlgorithms::default_buffer = 0;

#include <iostream>
#include "screenquad.h"
#include "utils.h"
void RenderAlgorithms::renderTexture(FrameBuffer fbo, const Texture2D& tex)
{
	fbo.useFrameBuffer();
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glDrawBuffers(1, render_buff);
	
	std::cout << "Render" << std::endl;
	tex.use(GL_TEXTURE0);
	ScreenQuad* quad = ScreenQuad::getInstanceP();
	//shader 
	GlslShader* shader = shader_manager->getShader(GlslShaderManager::Shaders::TEXTURE_TO_SCREEN);
	shader->use();
		quad->render();
	shader->unUse();
	checkCritOpenGLError();

	//glBindFramebuffer(GL_FRAMEBUFFER, RenderAlgorithms::default_buffer);
}