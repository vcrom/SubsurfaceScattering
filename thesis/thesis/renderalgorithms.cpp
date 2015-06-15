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
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

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
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);


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

void RenderAlgorithms::renderShadows(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, std::shared_ptr<Texture2D> shadow_tex, glm::mat4 V_L, glm::mat4 P_L, glm::vec3 light_pos)
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
		glUniformMatrix4fv(shader->operator()("N"), 1, GL_FALSE, glm::value_ptr(V*M));
		glUniformMatrix4fv(shader->operator()("S"), 1, GL_FALSE, glm::value_ptr(S));
		glUniform3fv(shader->operator()("eye_light_position"), 1, glm::value_ptr(eye_light_pos));
		mesh->render();
	shader->unUse();


	glDisable(GL_CULL_FACE);
	checkCritOpenGLError();
}

void RenderAlgorithms::getLinealShadowMap(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, float z_far, const glm::vec2 &viewport_size, const glm::vec2 &shadow_buffer_size, const glm::vec3 &light_pos)
{
	//renderMesh(fbo, mesh, M, V, P, glm::vec3(0, 1, 0));
	//return;
	assert(RenderAlgorithms::checkGLEnabled(GL_DEPTH_TEST));

	fbo->useFrameBuffer(2);
	glViewport(0, 0, shadow_buffer_size.x, shadow_buffer_size.y);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	checkCritOpenGLError();

	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::LINEAL_DEEPTH);
	shader->use();
		glUniformMatrix4fv(shader->operator()("M"), 1, GL_FALSE, glm::value_ptr(M));
		glUniformMatrix4fv(shader->operator()("V"), 1, GL_FALSE, glm::value_ptr(V));
		glUniformMatrix4fv(shader->operator()("P"), 1, GL_FALSE, glm::value_ptr(P));
		glUniform1f(shader->operator()("z_far"), z_far);
		glUniform3fv(shader->operator()("light_position"), 1, glm::value_ptr(light_pos));
		checkCritOpenGLError();
		mesh->render();
	shader->unUse();

	glDisable(GL_CULL_FACE);
	glViewport(0, 0, viewport_size.x, viewport_size.y);
	checkCritOpenGLError();
}

void RenderAlgorithms::translucency(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, std::shared_ptr<Texture2D> lineal_deepth_tex, float z_far, glm::mat4 V_L, glm::mat4 P_L, glm::vec3 light_pos)
{
	//bias matrix
	glm::mat4 B = glm::scale(glm::translate(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(0.5, 0.5, 0.5));
	//glm::mat4 S = B * P_L * V_L;// *M;
	//glm::vec4 aux = V*glm::vec4(light_pos, 1);
	//glm::vec3 eye_light_pos = glm::vec3(aux.x, aux.y, aux.z) / aux.w;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	lineal_deepth_tex->use(GL_TEXTURE0);
	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::TRANSLUCENCY_SHADER);
	fbo->useFrameBuffer();
	shader->use();
	glUniformMatrix4fv(shader->operator()("MVP"), 1, GL_FALSE, glm::value_ptr(P*V*M));
	glUniformMatrix4fv(shader->operator()("MV"), 1, GL_FALSE, glm::value_ptr(V*M));
	glUniformMatrix4fv(shader->operator()("M"), 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(shader->operator()("N"), 1, GL_FALSE, glm::value_ptr(glm::inverseTranspose(M)));
	glUniformMatrix4fv(shader->operator()("BP_L"), 1, GL_FALSE, glm::value_ptr(B * P_L));
	glUniformMatrix4fv(shader->operator()("V_L"), 1, GL_FALSE, glm::value_ptr(V_L));
	glUniform3fv(shader->operator()("light_position"), 1, glm::value_ptr(light_pos));
	glUniform1f(shader->operator()("z_far"), z_far);
	mesh->render();
	shader->unUse();


	glDisable(GL_CULL_FACE);
	checkCritOpenGLError();
}

bool RenderAlgorithms::checkGLEnabled(GLenum param)
{
	return glIsEnabled(param);
}

void RenderAlgorithms::renderDiffuseAndSpecular(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::mat4 prev_VP, glm::vec3 camera_pos, float z_far, glm::vec3 light_pos, 
	std::shared_ptr<Texture2D> shadow_tex, glm::mat4 V_L, glm::mat4 P_L, 
	std::shared_ptr<Texture2D> light_linear_shadow_tex, float light_far_plane, float sss_width, float translucency, bool ssss_enabled)
{
	assert(RenderAlgorithms::checkGLEnabled(GL_DEPTH_TEST));

	glm::mat4 B = glm::scale(glm::translate(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(0.5, 0.5, 0.5));
	glm::mat4 lightViewProjM = B * P_L * V_L;

	shadow_tex->use(GL_TEXTURE0);
	light_linear_shadow_tex->use(GL_TEXTURE1);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::MAIN_RENDER_SHADER);
	fbo->useFrameBuffer(3);

	shader->use();
		//vert
		glUniformMatrix4fv(shader->operator()("curr_WorldViewProjM;"), 1, GL_FALSE, glm::value_ptr(P*V*M));
		glUniformMatrix4fv(shader->operator()("curr_WorldViewProjM;"), 1, GL_FALSE, glm::value_ptr(prev_VP*M));
		glUniformMatrix4fv(shader->operator()("worldInverseTransposeM"), 1, GL_FALSE, glm::value_ptr(glm::inverseTranspose(M)));
		glUniformMatrix4fv(shader->operator()("worldM"), 1, GL_FALSE, glm::value_ptr(M));
		glUniformMatrix4fv(shader->operator()("viewM"), 1, GL_FALSE, glm::value_ptr(V));
		glUniform3fv(shader->operator()("m_camera_pos"), 1, glm::value_ptr(camera_pos));
		glUniform1f(shader->operator()("z_far"), z_far);
		//frag
		glUniform3fv(shader->operator()("light_position"), 1, glm::value_ptr(light_pos));
		glUniform1f(shader->operator()("m_ambientcomp"), 1.0f); 
		glUniformMatrix4fv(shader->operator()("lightViewProjBiasM"), 1, GL_FALSE, glm::value_ptr(B * P_L * V_L));


		glUniformMatrix4fv(shader->operator()("lightViewM"), 1, GL_FALSE, glm::value_ptr(V_L));
		glUniformMatrix4fv(shader->operator()("lightProjBiasM"), 1, GL_FALSE, glm::value_ptr(B * P_L));
		glUniform1f(shader->operator()("light_far_plane"), light_far_plane);
		glUniform1f(shader->operator()("sssWidth"), sss_width);
		glUniform1f(shader->operator()("translucency"), translucency);
		glUniform1i(shader->operator()("sssEnabled"), int(ssss_enabled));
		checkCritOpenGLError();

		mesh->render();
	shader->unUse();

	glDisable(GL_CULL_FACE);
}

//void RenderAlgorithms::mainRenderPas(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::mat4 prev_MVP, glm::vec3 camera_pos, float z_far)
//{
//	assert(RenderAlgorithms::checkGLEnabled(GL_DEPTH_TEST));
//
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//
//	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::MAIN_RENDER_SHADER);
//	fbo->useFrameBuffer();
//
//	shader->use();
//	glUniformMatrix4fv(shader->operator()("MVP"), 1, GL_FALSE, glm::value_ptr(P*V*M));
//	mesh->render();
//	shader->unUse();
//
//	glDisable(GL_CULL_FACE);
//}