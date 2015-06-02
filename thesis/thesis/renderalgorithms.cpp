#include "renderalgorithms.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


#include "screenquad.h"
#include "utils.h"

RenderAlgorithms::RenderAlgorithms()
{

}

RenderAlgorithms::~RenderAlgorithms()
{

}

std::shared_ptr<GlslShaderManager> RenderAlgorithms::_shader_manager = GlslShaderManager::instance();
//const GLenum render_buff[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
GLuint RenderAlgorithms::default_buffer = 0;


//static void initialize(unsigned int num_tex);
//static void resizeTextures(unsigned int w, unsigned int h);
//static void resizeTexture(unsigned int id, unsigned int w, unsigned int h);

void RenderAlgorithms::renderTexture(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> tex)
{
	fbo->useFrameBuffer();
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glDrawBuffers(1, render_buff);
	
	//std::cout << "Render to Tex" << std::endl;
	tex->use(GL_TEXTURE0);
	ScreenQuad* quad = ScreenQuad::getInstanceP();
	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::TEXTURE_TO_SCREEN);
	shader->use();
		quad->render();
	shader->unUse();
	checkCritOpenGLError();

	//glBindFramebuffer(GL_FRAMEBUFFER, RenderAlgorithms::default_buffer);
}

void RenderAlgorithms::renderMesh(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 col)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glCullFace(GL_FRONT);
	assert(RenderAlgorithms::checkGLEnabled(GL_DEPTH_TEST));
	fbo->useFrameBuffer();
	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::PASS_THROUGH);
	shader->use();
		glUniformMatrix4fv(shader->operator()("MVP"), 1, GL_FALSE, glm::value_ptr(P*V*M));
		glUniform3fv(shader->operator()("color"), 1, glm::value_ptr(col));
		mesh->render();
	shader->unUse();
	glDisable(GL_CULL_FACE);

	checkCritOpenGLError();
}


/// <summary>
/// Uses the framebuffer fbo to write in its deepth channel the mesh deepth info from a ceirtain point of view.
/// </summary>
/// <param name="fbo">FrameBuffer with depth atachment.</param>
/// <param name="mesh">Mesh.</param>
/// <param name="M">The model transformation matrix.</param>
/// <param name="V">The view transformation matrix.</param>
/// <param name="P">The projection transformation matrix.</param>
/// <param name="viewport_size">The viewport_size.</param>
/// <param name="shadow_buffer_size">The shadow_buffer_size.</param>
void RenderAlgorithms::getShadowMap(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, const glm::vec2 &viewport_size, const glm::vec2 &shadow_buffer_size)
{
	assert(RenderAlgorithms::checkGLEnabled(GL_DEPTH_TEST));

	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::PASS_THROUGH);
	fbo->useFrameBuffer();
	glViewport(0, 0, shadow_buffer_size.x, shadow_buffer_size.y);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glCullFace(GL_BACK);


	shader->use();
		glUniformMatrix4fv(shader->operator()("MVP"), 1, GL_FALSE, glm::value_ptr(P*V*M));
		mesh->render();
	shader->unUse();

	glDisable(GL_CULL_FACE);
	glViewport(0, 0, viewport_size.x, viewport_size.y);
	checkCritOpenGLError();

}

void RenderAlgorithms::renderDiffuseAndShadows(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, std::shared_ptr<Texture2D> shadow_tex, glm::mat4 V_L, glm::mat4 P_L, glm::vec3 light_pos)
{
	assert(RenderAlgorithms::checkGLEnabled(GL_DEPTH_TEST));
	//RenderAlgorithms::renderMesh(fbo, mesh, M, V, P, glm::vec3(1, 0, 0));

	//bias matrix
	glm::mat4 B = glm::scale(glm::translate(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(0.5, 0.5, 0.5));
	glm::mat4 S = B * P_L * V_L * M;
	glm::vec4 aux = V*glm::vec4(light_pos, 1);
	glm::vec3 eye_light_pos = glm::vec3(aux.x, aux.y, aux.z)/aux.w;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shadow_tex->use(GL_TEXTURE0);
	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::SHADOWS_AND_DIFFUSE);
	fbo->useFrameBuffer();
	shader->use();
		glUniformMatrix4fv(shader->operator()("MVP"), 1, GL_FALSE, glm::value_ptr(P*V*M));
		glUniformMatrix4fv(shader->operator()("MV"), 1, GL_FALSE, glm::value_ptr(V*M));
		glUniformMatrix4fv(shader->operator()("M"), 1, GL_FALSE, glm::value_ptr(M));
		glUniformMatrix4fv(shader->operator()("N"), 1, GL_FALSE, glm::value_ptr(V*M));
		glUniformMatrix4fv(shader->operator()("S"), 1, GL_FALSE, glm::value_ptr(S));
		glUniform3fv(shader->operator()("eye_light_position"), 1, glm::value_ptr(eye_light_pos));
		mesh->render();
	shader->unUse();


	glDisable(GL_CULL_FACE);
	checkCritOpenGLError();
}

void RenderAlgorithms::getLinealShadowMap(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, float z_far, const glm::vec2 &viewport_size, const glm::vec2 &shadow_buffer_size)
{
	//renderMesh(fbo, mesh, M, V, P, glm::vec3(0, 1, 0));
	//return;
	assert(RenderAlgorithms::checkGLEnabled(GL_DEPTH_TEST));

	fbo->useFrameBuffer();
	glViewport(0, 0, shadow_buffer_size.x, shadow_buffer_size.y);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	checkCritOpenGLError();

	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::LINEAL_DEEPTH);
	shader->use();
		glUniformMatrix4fv(shader->operator()("MVP"), 1, GL_FALSE, glm::value_ptr(P*V*M));
		glUniformMatrix4fv(shader->operator()("MV"), 1, GL_FALSE, glm::value_ptr(V*M));
		glUniform1f(shader->operator()("z_far"), z_far);
		checkCritOpenGLError();
		mesh->render();
	shader->unUse();

	glDisable(GL_CULL_FACE);
	glViewport(0, 0, viewport_size.x, viewport_size.y);
	checkCritOpenGLError();
}
bool RenderAlgorithms::checkGLEnabled(GLenum param)
{
	return glIsEnabled(param);
}

void RenderAlgorithms::renderThickness(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, float z_far)//, const glm::vec2 &viewport_size, const glm::vec2 &shadow_buffer_size)
{
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);


	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::THICKNESS_SHADER);
	fbo->useFrameBuffer();
	//glViewport(0, 0, shadow_buffer_size.x, shadow_buffer_size.y);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	shader->use();
		glUniform1f(shader->operator()("z_far"), z_far);
		glUniformMatrix4fv(shader->operator()("MV"), 1, GL_FALSE, glm::value_ptr(V*M));
		glUniformMatrix4fv(shader->operator()("MVP"), 1, GL_FALSE, glm::value_ptr(P*V*M));
		mesh->render();
	shader->unUse();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//glViewport(0, 0, viewport_size.x, viewport_size.y);
	checkCritOpenGLError();

}