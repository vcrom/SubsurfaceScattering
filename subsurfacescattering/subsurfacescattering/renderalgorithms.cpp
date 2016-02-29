#include "renderalgorithms.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


#include "screenquad.h"
#include "utils.h"
#include <GL/glew.h>

RenderAlgorithms::RenderAlgorithms()
{

}

RenderAlgorithms::~RenderAlgorithms()
{

}

std::shared_ptr<GlslShaderManager> RenderAlgorithms::_shader_manager = GlslShaderManager::instance();
//const GLenum render_buff[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
GLuint RenderAlgorithms::default_buffer = 0;

std::vector<float> RenderAlgorithms::_ssss_kernel = std::vector<float>(); 
std::vector<float> RenderAlgorithms::_gaussian_weights = std::vector<float>(); 
std::vector<float> RenderAlgorithms::_ssss_precomputed_kernel = std::vector<float>();
std::vector<float> RenderAlgorithms::_ssss_precomputed_kernel_sampled = std::vector<float>();

int RenderAlgorithms::_num_sss_samples = 0;
float RenderAlgorithms::_pre_int_range = 1;


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


void RenderAlgorithms::blurTexture(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> tex, std::shared_ptr<Texture2D> aux_blur_tex, glm::vec2 pixel_size)
{
	
	ScreenQuad* quad = ScreenQuad::getInstanceP();
	std::shared_ptr<GlslShader> vert = _shader_manager->getShader(GlslShaderManager::Shaders::IMAGE_BLUR_SHADER_VERT);
	vert->use();
	glUniform2fv(vert->operator()("pixel_size"), 1, glm::value_ptr(pixel_size));

	std::shared_ptr<GlslShader> horiz = _shader_manager->getShader(GlslShaderManager::Shaders::IMAGE_BLUR_SHADER_HORI);
	horiz->use();
	glUniform2fv(horiz->operator()("pixel_size"), 1, glm::value_ptr(pixel_size));

	fbo->useFrameBuffer();
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_STENCIL_TEST);
	glDepthMask(GL_FALSE);

	tex->use(GL_TEXTURE0);
	horiz->use();
	quad->render();

	aux_blur_tex->use(GL_TEXTURE0);
	fbo->useFrameBuffer();
	fbo->colorBuffer(tex->getTextureID(), 0);
	
	vert->use();
	quad->render();
	vert->unUse();

	checkCritOpenGLError();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

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
	glEnable(GL_DEPTH_TEST);

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
	glEnable(GL_DEPTH_TEST);
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
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	glViewport(0, 0, GLint(shadow_buffer_size.x), GLint(shadow_buffer_size.y));

	fbo->useFrameBuffer(1);
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

void RenderAlgorithms::renderDiffuseAndSpecular(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::mat4 prev_VP, 
	glm::vec3 camera_pos, float z_far, glm::vec3 light_pos, float z_near, float roughness, float bump_int, std::shared_ptr<Texture2D> diffuse_env_map,
	std::shared_ptr<Texture2D> shadow_tex, glm::mat4 V_L, glm::mat4 P_L, 
	std::shared_ptr<Texture2D> light_linear_shadow_tex, float light_far_plane, float sss_width, 
	float translucency, float ambient_int, float specular_int, bool ssss_enabled, 
	std::shared_ptr<Texture2D> diffuse_texture, std::shared_ptr<Texture2D> ao_texture, std::shared_ptr<Texture2D> normals_texture, bool use_texture)
{
	glEnable(GL_DEPTH_TEST);

	glm::mat4 B = glm::scale(glm::translate(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(0.5, 0.5, 0.5));
	glm::mat4 lightViewProjM = B * P_L * V_L;

	shadow_tex->use(GL_TEXTURE0);
	light_linear_shadow_tex->use(GL_TEXTURE1);

	if (use_texture)
	{
		diffuse_texture->use(GL_TEXTURE2);
		ao_texture->use(GL_TEXTURE3);
		normals_texture->use(GL_TEXTURE4);
	}
	diffuse_env_map->use(GL_TEXTURE5);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::MAIN_RENDER_SHADER);
	fbo->useFrameBuffer(fbo->getColorLevels());

	//shader.addUniform("min_z");
	shader->use();
		//vert
		glUniformMatrix4fv(shader->operator()("curr_WorldViewProjM"), 1, GL_FALSE, glm::value_ptr(P*V*M));
		//glUniformMatrix4fv(shader->operator()("prev_WorldViewProjM"), 1, GL_FALSE, glm::value_ptr(prev_VP*M));
		glUniformMatrix4fv(shader->operator()("worldInverseTransposeM"), 1, GL_FALSE, glm::value_ptr(glm::inverseTranspose(M)));
		glUniformMatrix4fv(shader->operator()("viewInverseTransposeM"), 1, GL_FALSE, glm::value_ptr(glm::inverseTranspose(V)));
		glUniformMatrix4fv(shader->operator()("worldM"), 1, GL_FALSE, glm::value_ptr(M));
		glUniformMatrix4fv(shader->operator()("viewM"), 1, GL_FALSE, glm::value_ptr(V));
		glUniform3fv(shader->operator()("m_camera_pos"), 1, glm::value_ptr(camera_pos));
		glUniform1f(shader->operator()("z_far"), z_far);
		//frag
		glUniform3fv(shader->operator()("light_position"), 1, glm::value_ptr(light_pos)); 
		glUniform1f(shader->operator()("m_ambientcomp"), ambient_int);
		glUniform1f(shader->operator()("spec_int"), specular_int);
		
		glUniformMatrix4fv(shader->operator()("lightViewProjBiasM"), 1, GL_FALSE, glm::value_ptr(lightViewProjM));
		glUniformMatrix4fv(shader->operator()("lightViewM"), 1, GL_FALSE, glm::value_ptr(V_L));
		glUniformMatrix4fv(shader->operator()("lightProjBiasM"), 1, GL_FALSE, glm::value_ptr(B * P_L));
		glUniform1f(shader->operator()("light_far_plane"), light_far_plane);
		glUniform1f(shader->operator()("sssWidth"), sss_width);
		glUniform1f(shader->operator()("translucency"), translucency);
		glUniform1i(shader->operator()("sssEnabled"), int(ssss_enabled));
		glUniform1f(shader->operator()("roughness"), roughness);

		glUniform1i(shader->operator()("texture_enabled"), int(use_texture)); 
		glUniform1f(shader->operator()("z_near"), z_near);
		glUniform1f(shader->operator()("bumpiness"), bump_int);
		checkCritOpenGLError();

		mesh->render();
	shader->unUse();

	glDisable(GL_CULL_FACE);
}

std::vector<glm::vec4> initGaussians()
{
	auto gaussians = std::vector<glm::vec4>(0);
	gaussians.push_back(glm::vec4(0.0064, 1, 1, 1));
	gaussians.push_back(glm::vec4(0.0484, 0.3251, 0.424, 0.346));
	gaussians.push_back(glm::vec4(0.187, 0.261, 0.2, 0.0));
	gaussians.push_back(glm::vec4(0.567, 0.2, 0.007, 0.007));
	gaussians.push_back(glm::vec4(1.99, 0.388, 0.004, 0));
	gaussians.push_back(glm::vec4(7.41, 0.078, 0.0, 0));
	for (unsigned int  i = 1; i < gaussians.size(); ++i) gaussians[i].x = gaussians[i].x - gaussians[i - 1].x;
	return gaussians;
}

std::vector<glm::vec4> RenderAlgorithms::_gaussians = initGaussians();


void RenderAlgorithms::SSSEffect(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> sss_tex, std::shared_ptr<Texture2D> sss_tex_pingpong, std::shared_ptr<Texture2D> rt1_tex, std::shared_ptr<Texture2D> rt2_tex, std::shared_ptr<Texture2D> lineal_depth, glm::vec2 pixel_size, float correction, float sssWidth, float cam_fovy, std::shared_ptr<Texture2D> cross_bilateral_factor, std::shared_ptr<Texture2D> curvature_tex, std::shared_ptr<Texture2D> albedo_tex)
{
	std::vector<std::shared_ptr<Texture2D> >pingpong_tex = { sss_tex, sss_tex_pingpong };

	std::shared_ptr<GlslShader> horizontal = _shader_manager->getShader(GlslShaderManager::Shaders::SSSS_HORIZONTAL_BLUR);
	std::shared_ptr<GlslShader> vertical = _shader_manager->getShader(GlslShaderManager::Shaders::SSSS_VERTICAL_BLUR);

	horizontal->use();
	glUniform1f(horizontal->operator()("cam_fovy"), cam_fovy);
	glUniform2fv(horizontal->operator()("pixel_size"), 1, glm::value_ptr(pixel_size));
	glUniform1f(horizontal->operator()("correction"), correction);
	glUniform1f(horizontal->operator()("sssWidth"), sssWidth);
	vertical->use();
	glUniform1f(vertical->operator()("cam_fovy"), cam_fovy);
	glUniform2fv(vertical->operator()("pixel_size"), 1, glm::value_ptr(pixel_size));
	glUniform1f(vertical->operator()("correction"), correction);
	glUniform1f(vertical->operator()("sssWidth"), sssWidth);

	ScreenQuad* quad = ScreenQuad::getInstanceP();


	sss_tex->use(GL_TEXTURE0);
	lineal_depth->use(GL_TEXTURE1);
	cross_bilateral_factor->use(GL_TEXTURE3);
	curvature_tex->use(GL_TEXTURE4);
	albedo_tex->use(GL_TEXTURE5);

	fbo->useFrameBuffer(1);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	int idx = 0;
	for (unsigned int i = 0; i < _gaussians.size(); ++i)
	{
		idx = i % 2;
		fbo->useFrameBuffer(1);
		fbo->colorBuffer(rt2_tex->getTextureID(), 0);
		horizontal->use();
		glUniform4fv(horizontal->operator()("gaussian"), 1, glm::value_ptr(_gaussians[i]));
		quad->render();

		rt2_tex->use(GL_TEXTURE0);
		fbo->useFrameBuffer(2);
		fbo->colorBuffer(rt1_tex->getTextureID(), 0);
		fbo->colorBuffer(pingpong_tex[idx]->getTextureID(), 1);
		pingpong_tex[(i + 1) % 2]->use(GL_TEXTURE2);
		vertical->use();
		glUniform4fv(vertical->operator()("gaussian"), 1, glm::value_ptr(_gaussians[i]));
		quad->render();
		rt1_tex->use(GL_TEXTURE0);
	}

	//if (idx != 0)
	//{
		fbo->useFrameBuffer(1);
		fbo->colorBuffer(sss_tex->getTextureID(), 0);
		renderTexture(fbo, pingpong_tex[idx]);
	//}

	checkCritOpenGLError();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void RenderAlgorithms::separableSSSSEffect(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> sss_tex, std::shared_ptr<Texture2D> rt1_tex, std::shared_ptr<Texture2D> lineal_depth, float cam_fovy, float sssWidth, std::shared_ptr<Texture2D> cross_bilateral_factor, std::shared_ptr<Texture2D> curvature_tex, std::shared_ptr<Texture2D> albedo_tex)
{
	//std::cout << "WIDTH: " << sssWidth << std::endl;
	std::shared_ptr<GlslShader> horizontal = _shader_manager->getShader(GlslShaderManager::Shaders::SEPARABLE_SSSS_HORIZONTAL_BLUR);
	std::shared_ptr<GlslShader> vertical = _shader_manager->getShader(GlslShaderManager::Shaders::SEPARABLE_SSSS_VERTICAL_BLUR);
	horizontal->use();
	glUniform1f(horizontal->operator()("cam_fovy"), cam_fovy);
	glUniform1f(horizontal->operator()("sssWidth"), sssWidth);
	vertical->use();
	glUniform1f(vertical->operator()("cam_fovy"), cam_fovy);
	glUniform1f(vertical->operator()("sssWidth"), sssWidth);
	ScreenQuad* quad = ScreenQuad::getInstanceP();

	lineal_depth->use(GL_TEXTURE1);
	cross_bilateral_factor->use(GL_TEXTURE2);
	curvature_tex->use(GL_TEXTURE4);
	albedo_tex->use(GL_TEXTURE5);

	fbo->useFrameBuffer();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	sss_tex->use(GL_TEXTURE0);
	fbo->colorBuffer(rt1_tex->getTextureID(), 0);
	horizontal->use();
	//vertical->use();
	quad->render();

	rt1_tex->use(GL_TEXTURE0);
	fbo->colorBuffer(sss_tex->getTextureID(), 0);
	vertical->use();
	//horizontal->use();
	quad->render();

	checkCritOpenGLError();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}


void RenderAlgorithms::GaussianSSSEffect(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> sss_tex, std::shared_ptr<Texture2D> sss_tex_pingpong, std::shared_ptr<Texture2D> rt1_tex, std::shared_ptr<Texture2D> rt2_tex, std::shared_ptr<Texture2D> lineal_depth, glm::vec2 pixel_size, float correction, float sssWidth, float cam_fovy, std::shared_ptr<Texture2D> cross_bilateral_factor, std::shared_ptr<Texture2D> curvature_tex, std::shared_ptr<Texture2D> albedo_tex)
{
	std::vector<std::shared_ptr<Texture2D> >pingpong_tex = { sss_tex, sss_tex_pingpong };

	std::shared_ptr<GlslShader> horizontal = _shader_manager->getShader(GlslShaderManager::Shaders::GAUSSIAN_SSSS_HORIZONTAL_BLUR);
	std::shared_ptr<GlslShader> vertical = _shader_manager->getShader(GlslShaderManager::Shaders::GAUSSIAN_SSSS_VERTICAL_BLUR);

	horizontal->use();
	glUniform1f(horizontal->operator()("cam_fovy"), cam_fovy);
	glUniform2fv(horizontal->operator()("pixel_size"), 1, glm::value_ptr(pixel_size));
	glUniform1f(horizontal->operator()("correction"), correction);
	glUniform1f(horizontal->operator()("sssWidth"), sssWidth);
	vertical->use();
	glUniform1f(vertical->operator()("cam_fovy"), cam_fovy);
	glUniform2fv(vertical->operator()("pixel_size"), 1, glm::value_ptr(pixel_size));
	glUniform1f(vertical->operator()("correction"), correction);
	glUniform1f(vertical->operator()("sssWidth"), sssWidth);

	ScreenQuad* quad = ScreenQuad::getInstanceP();


	sss_tex->use(GL_TEXTURE0);
	lineal_depth->use(GL_TEXTURE1);
	cross_bilateral_factor->use(GL_TEXTURE3);
	curvature_tex->use(GL_TEXTURE4);
	albedo_tex->use(GL_TEXTURE5);


	fbo->useFrameBuffer(1);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	int idx = 0;
	for (unsigned int i = 0; i < _gaussians.size(); ++i)
	{
		idx = i % 2;
		fbo->useFrameBuffer(1);
		fbo->colorBuffer(rt2_tex->getTextureID(), 0);
		horizontal->use();
		glUniform4fv(horizontal->operator()("gaussian"), 1, glm::value_ptr(_gaussians[i]));
		quad->render();

		rt2_tex->use(GL_TEXTURE0);
		fbo->useFrameBuffer(2);
		fbo->colorBuffer(rt1_tex->getTextureID(), 0);
		fbo->colorBuffer(pingpong_tex[idx]->getTextureID(), 1);
		pingpong_tex[(i + 1) % 2]->use(GL_TEXTURE2);
		vertical->use();
		glUniform4fv(vertical->operator()("gaussian"), 1, glm::value_ptr(_gaussians[i]));
		quad->render();
		rt1_tex->use(GL_TEXTURE0);
	}

	//if (idx != 0)
	//{
	fbo->useFrameBuffer(1);
	fbo->colorBuffer(sss_tex->getTextureID(), 0);
	renderTexture(fbo, pingpong_tex[idx]);
	//}

	checkCritOpenGLError();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void RenderAlgorithms::toneMapTexture(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> tex, float exposure, float burnout, int method)
{
	fbo->useFrameBuffer();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	tex->use(GL_TEXTURE0);
	ScreenQuad* quad = ScreenQuad::getInstanceP();
	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::TONE_MAP);
	shader->use();
	glUniform1f(shader->operator()("exposure"), exposure);
	//glUniform1f(shader->operator()("m_burnout"), 50); 
	glUniform1i(shader->operator()("method"), method);
	std::cout << "Method " << float(method) << std::endl;
	checkCritOpenGLError();
	quad->render();
	shader->unUse();
	checkCritOpenGLError();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);


	//glBindFramebuffer(GL_FRAMEBUFFER, RenderAlgorithms::default_buffer);
}

#define M_PIl          3.141592653589793238462643383279502884L
#define M_PI           3.14159265358979323846

glm::vec3 gaussian(float variance, float r, const std::vector<float> &falloff) {
	/**
	* We use a falloff to modulate the shape of the profile. Big falloffs
	* spreads the shape making it wider, while small falloffs make it
	* narrower.
	*/
	glm::vec3 g;
	for (int i = 0; i < 3; i++) {
		float rr = r / (0.001f + falloff[i]);
		g[i] = exp((-(rr * rr)) / (2.0f * variance)) / (2.0f * M_PI * variance);
	}
	return g;
}


glm::vec3 profile(float r, const std::vector<float> &falloff) {
	/**
	* We used the red channel of the original skin profile defined in
	* [d'Eon07] for all three channels. We noticed it can be used for green
	* and blue channels (scaled using the falloff parameter) without
	* introducing noticeable differences and allowing for total control over
	* the profile. For example, it allows to create blue SSS gradients, which
	* could be useful in case of rendering blue creatures.
	*/
	return
		0.100f * gaussian(0.0484f, r, falloff) +
		0.118f * gaussian(0.187f, r, falloff) +
		0.113f * gaussian(0.567f, r, falloff) +
		0.358f * gaussian(1.99f, r, falloff) +
		0.078f * gaussian(7.41f, r, falloff);
}

#include <chrono>
std::chrono::high_resolution_clock hi_clock;
using time_unit = std::chrono::nanoseconds;
void RenderAlgorithms::computeSeparableKernel(int num_samples, const glm::vec3 &sss_strength, std::vector<float> &falloff)
{
		//std::vector<glm::vec4> kernel(num_samples);

		std::chrono::high_resolution_clock::time_point t1 = hi_clock.now();
		const float RANGE = 2.0f;
		const float EXPONENT = 2.0f;

		_ssss_kernel = std::vector<float> (num_samples * 4);

		// Calculate the offsets:
		//float step = 2.0f * RANGE / (num_samples - 1);
		float step = RANGE / (num_samples / 2);
		for (int i = 0; i < num_samples; i++) {
			float o = -RANGE + float(i) * step;
			float sign = o < 0.0f ? -1.0f : 1.0f;
			//_ssss_kernel[i].w = RANGE * sign * abs(pow(o, EXPONENT)) / pow(RANGE, EXPONENT);
			_ssss_kernel[i * 4 + 3] = RANGE * sign * abs(pow(o, EXPONENT)) / pow(RANGE, EXPONENT);
		}

		// Calculate the weights:
		for (int i = 0; i < num_samples; i++) {
			//float w0 = i > 0 ? abs(_ssss_kernel[i].w - _ssss_kernel[i - 1].w) : 0.0f;
			//float w1 = i < num_samples - 1 ? abs(_ssss_kernel[i].w - _ssss_kernel[i + 1].w) : 0.0f;
			float w0 = i > 0 ? abs(_ssss_kernel[i*4+3] - _ssss_kernel[4*(i - 1) +3]) : 0.0f;
			float w1 = i < num_samples - 1 ? abs(_ssss_kernel[i * 4 + 3] - _ssss_kernel[4*(i + 1)+3]) : 0.0f;
			float area = (w0 + w1) / 2.0f;
			//glm::vec3 t = area * profile(_ssss_kernel[i].w, falloff);
			glm::vec3 t = area * profile(_ssss_kernel[i*4+3], falloff);
			_ssss_kernel[i*4] = t.x;
			_ssss_kernel[i*4 + 1] = t.y;
			_ssss_kernel[i*4 + 2] = t.z;
		}

		// We want the offset 0.0 to come first:
		//glm::vec4 t = _ssss_kernel[num_samples / 2];
		glm::vec4 t = glm::vec4(_ssss_kernel[(num_samples / 2) * 4], _ssss_kernel[(num_samples / 2) * 4 + 1], _ssss_kernel[(num_samples / 2) * 4 + 2], _ssss_kernel[(num_samples / 2) * 4 + 3]);
		for (int i = num_samples / 2; i > 0; i--)
		{
			_ssss_kernel[i*4] = _ssss_kernel[(i - 1)*4];
			_ssss_kernel[i * 4+1] = _ssss_kernel[(i - 1) * 4+1];
			_ssss_kernel[i * 4+2] = _ssss_kernel[(i - 1) * 4+2];
			_ssss_kernel[i * 4+3] = _ssss_kernel[(i - 1) * 4+3];
		}
		//_ssss_kernel[0] = t;
		_ssss_kernel[0] = t.x;
		_ssss_kernel[1] = t.y;
		_ssss_kernel[2] = t.z;
		_ssss_kernel[3] = t.w;

		// Calculate the sum of the weights, we will need to normalize them below:
		glm::vec3 sum = glm::vec3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < num_samples; i++)
			//sum += glm::vec3(_ssss_kernel[i]);
			sum += glm::vec3(_ssss_kernel[i * 4], _ssss_kernel[i * 4+1], _ssss_kernel[i * 4+2]);


		// Normalize the weights:
		for (int i = 0; i < num_samples; i++) {
			//_ssss_kernel[i].x /= sum.x;
			//_ssss_kernel[i].y /= sum.y;
			//_ssss_kernel[i].z /= sum.z;
			_ssss_kernel[i*4] /= sum.x;
			_ssss_kernel[i*4+1] /= sum.y;
			_ssss_kernel[i*4+2] /= sum.z;
		}

		// Tweak them using the desired strength. The first one is:
		//     lerp(1.0, _ssss_kernel[0].rgb, strength)
		//_ssss_kernel[0].x = (1.0f - sss_strength.x) * 1.0f + sss_strength.x * _ssss_kernel[0].x;
		//_ssss_kernel[0].y = (1.0f - sss_strength.y) * 1.0f + sss_strength.y * _ssss_kernel[0].y;
		//_ssss_kernel[0].z = (1.0f - sss_strength.z) * 1.0f + sss_strength.z * _ssss_kernel[0].z;
		_ssss_kernel[0] = (1.0f - sss_strength.x) * 1.0f + sss_strength.x * _ssss_kernel[1];
		_ssss_kernel[1] = (1.0f - sss_strength.y) * 1.0f + sss_strength.y * _ssss_kernel[2];
		_ssss_kernel[2] = (1.0f - sss_strength.z) * 1.0f + sss_strength.z * _ssss_kernel[3];

		// The others:
		//     lerp(0.0, _ssss_kernel[0].rgb, strength)
		for (int i = 1; i < num_samples; i++) {
			//_ssss_kernel[i].x *= sss_strength.x;
			//_ssss_kernel[i].y *= sss_strength.y;
			//_ssss_kernel[i].z *= sss_strength.z;
			_ssss_kernel[i*4] *= sss_strength.x;
			_ssss_kernel[i*4+1] *= sss_strength.y;
			_ssss_kernel[i*4+2] *= sss_strength.z;
		}

		std::chrono::high_resolution_clock::time_point t2 = hi_clock.now();
		std::cout << "\tSeparableKernelComputation time: " << std::chrono::duration_cast<time_unit>(t2 - t1).count() << std::endl;
}

void RenderAlgorithms::computeGaussianKernel(int num_samples)
{
	std::cout << "Num samples: " << num_samples << std::endl;
	_gaussian_weights = std::vector<float>(num_samples*2);

	//get gaussian weights
	const float RANGE = 3.0f;
	float incr = RANGE / float(num_samples / RANGE);
	float dist = -RANGE;
	float sum = 0;
	const float dev = 1.04037900598585464717179319651756539235055628923279085812682966;
	
	//compute gaussian taps
	for (int i = 0; i < num_samples*2; i+=2)
	{
		//std::cout << dist << " ";
		_gaussian_weights[i] = exp(-(dist*dist) / (2.0f * dev*dev)) / (sqrt(2.0f * M_PI) * dev);
		_gaussian_weights[i + 1] = dist / RANGE;
		dist += incr;
		sum += _gaussian_weights[i];
	}
	//std::cout << std::endl;

	//for (float b : weights)
	//{
	//	std::cout << b << " ";
	//}
	//std::cout << std::endl;

	//normalize weights
	for (int i = 0; i < num_samples * 2; i += 2)
		_gaussian_weights[i] /= sum;

	for (int i = 0; i < num_samples * 2; i += 2)
		std::cout << _gaussian_weights[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < num_samples * 2; i += 2)
		std::cout << _gaussian_weights[i+1] << " ";
	std::cout << std::endl;
}

void RenderAlgorithms::setSeparableKernels(int kernel)
{
	assert(kernel == 0 || kernel == 1);
	std::shared_ptr<GlslShader> horizontal = _shader_manager->getShader(GlslShaderManager::Shaders::SEPARABLE_SSSS_HORIZONTAL_BLUR);

	horizontal->use();
	glUniform1i(horizontal->operator()("ssss_n_samples"), _num_sss_samples);
	if (kernel == 0)
	{
		glUniform4fv(horizontal->operator()("kernel"), _num_sss_samples * 4, _ssss_kernel.data());
		glUniform1f(horizontal->operator()("kernel_range"), 3.0f);
	}
	else if (kernel == 1)
	{
		glUniform4fv(horizontal->operator()("kernel"), _num_sss_samples * 4, _ssss_precomputed_kernel_sampled.data());
		glUniform1f(horizontal->operator()("kernel_range"), _pre_int_range);
	}
	horizontal->unUse();

	std::shared_ptr<GlslShader> vertical = _shader_manager->getShader(GlslShaderManager::Shaders::SEPARABLE_SSSS_VERTICAL_BLUR);

	vertical->use();
	glUniform1i(vertical->operator()("ssss_n_samples"), _num_sss_samples);
	if (kernel == 0)
	{
		glUniform4fv(vertical->operator()("kernel"), _num_sss_samples * 4, _ssss_kernel.data());
		glUniform1f(vertical->operator()("kernel_range"), 3.0f);
	}
	else if (kernel == 1)
	{
		glUniform4fv(vertical->operator()("kernel"), _num_sss_samples * 4, _ssss_precomputed_kernel_sampled.data());
		glUniform1f(vertical->operator()("kernel_range"), _pre_int_range);
	}
	vertical->unUse();
	checkCritOpenGLError();
}

void RenderAlgorithms::setGaussianKernels()
{
	std::shared_ptr<GlslShader> horizontal = _shader_manager->getShader(GlslShaderManager::Shaders::GAUSSIAN_SSSS_HORIZONTAL_BLUR);
	horizontal->use();
	glUniform1i(horizontal->operator()("ssss_n_samples"), _num_sss_samples);
	glUniform2fv(horizontal->operator()("kernel"), _num_sss_samples * 2, &_gaussian_weights[0]);
	horizontal->unUse();
	checkCritOpenGLError();

	std::shared_ptr<GlslShader> vertical = _shader_manager->getShader(GlslShaderManager::Shaders::GAUSSIAN_SSSS_VERTICAL_BLUR);
	vertical->use();
	glUniform1i(vertical->operator()("ssss_n_samples"), _num_sss_samples);
	glUniform2fv(vertical->operator()("kernel"), _num_sss_samples * 2, &_gaussian_weights[0]);
	vertical->unUse();
	checkCritOpenGLError();
}

void RenderAlgorithms::computeKernels(int num_samples, const glm::vec3 &sss_strength, std::vector<float> &falloff)
{
	_num_sss_samples = num_samples;
	computeSeparableKernel(num_samples, sss_strength, falloff);
	computeGaussianKernel(num_samples);
	computeSampledPreIntegratedKernel(num_samples);
}

void RenderAlgorithms::setSSSSKernels(int separable_mode)
{
	setSeparableKernels(separable_mode);
	setGaussianKernels();
}

void RenderAlgorithms::setPreComputedKernel(const std::vector<float> &kernel)
{
	_ssss_precomputed_kernel = kernel;
}

void calculateOffsets(float _range, float _exponent, int _offsetCount, std::vector<float> & _offsets)
{
	// Calculate the offsets:
	float step = 2.0f * _range / (_offsetCount - 1);
	for (int i = 0; i < _offsetCount; i++) {
		float o = -_range + float(i) * step;
		float sign = o < 0.0f ? -1.0f : 1.0f;
		float ofs = _range * sign * abs(pow(o, _exponent)) / pow(_range, _exponent);
		_offsets.push_back(ofs);
	}
}

void calculateAreas(std::vector<float> & _offsets, std::vector<float> & _areas)
{
	int size = _offsets.size();

	for (int i = 0; i < size; i++) {
		float w0 = i > 0 ? abs(_offsets[i] - _offsets[i - 1]) : 0.0f;
		float w1 = i < size - 1 ? abs(_offsets[i] - _offsets[i + 1]) : 0.0f;
		float area = (w0 + w1) / 2.0f;
		_areas.push_back(area);
	}
}

//r, g, b, x, y
glm::vec3 linInterpol1D(const std::vector<float> &kernelData, float _x, unsigned int element_size)
{
	// naive, a lot to improve here
	unsigned int num_elements = kernelData.size() / 4;
	if (num_elements < 1) throw "_kernelData empty";

	unsigned int i = 0;
	while (i < num_elements)
	{
		if (_x > kernelData[i*element_size + 3]) i++;
		else break;
	}

	glm::vec3 v;

	if (i<1)
	{
		v.x = kernelData[0];
		v.y = kernelData[1];
		v.z = kernelData[2];
	}
	else if (i > num_elements - 1)
	{
		v.x = kernelData[(num_elements - 1)*element_size];
		v.y = kernelData[(num_elements - 1)*element_size + 1];
		v.z = kernelData[(num_elements - 1)*element_size + 2];
	}
	else
	{
		float br = kernelData[i*element_size];
		float bg = kernelData[i*element_size + 1];
		float bb = kernelData[i*element_size + 2];
		float bx = kernelData[i*element_size + 3];

		float ar = kernelData[(i - 1)*element_size];
		float ag = kernelData[(i - 1)*element_size + 1];
		float ab = kernelData[(i - 1)*element_size + 2];
		float ax = kernelData[(i - 1)*element_size + 3];

		float d = bx - ax;
		float dx = _x - ax;

		float t = dx / d;

		v.x = ar * (1 - t) + br * t;
		v.y = ag * (1 - t) + bg * t;
		v.z = ab * (1 - t) + bb * t;
	}

	return v;
}

void RenderAlgorithms::computeSampledPreIntegratedKernel(int num_samples)
{
	//int counter = 0;
	std::cout << "Computing Sampled preintegrated" << std::endl;
	std::chrono::high_resolution_clock::time_point t1 = hi_clock.now();

 	const float EXPONENT = 2.0f; // used for impartance sampling
	//std::cout << ++counter << std::endl;
	float RANGE = _ssss_precomputed_kernel[_ssss_precomputed_kernel.size()-1];
	_pre_int_range = RANGE;
	//std::cout << ++counter << std::endl;
	// calculate offsets
	std::vector<float> offsets;
	calculateOffsets(RANGE, EXPONENT, num_samples, offsets);
	//std::cout << ++counter << std::endl;

	// calculate areas (using importance-sampling) 
	std::vector<float> areas;
	calculateAreas(offsets, areas);
	//std::cout << ++counter << std::endl;

	_ssss_precomputed_kernel_sampled = std::vector<float>(num_samples * 4);
	//std::cout << ++counter << std::endl;

	glm::vec3 sum = glm::vec3(0.0f, 0.0f, 0.0f);

	// compute interpolated weights
	for (int i = 0; i<num_samples; i++)
	{
		float sx = offsets[i];

		glm::vec3 v = linInterpol1D(_ssss_precomputed_kernel, sx, 4);
		_ssss_precomputed_kernel_sampled[i*4] = v.x * areas[i];
		_ssss_precomputed_kernel_sampled[i*4 + 1] = v.y * areas[i];
		_ssss_precomputed_kernel_sampled[i*4 + 2] = v.z * areas[i];
		_ssss_precomputed_kernel_sampled[i*4 + 3] = sx;

		sum.x += _ssss_precomputed_kernel_sampled[i*4];
		sum.y += _ssss_precomputed_kernel_sampled[i*4 + 1];
		sum.z += _ssss_precomputed_kernel_sampled[i*4 + 2];
	}
	//std::cout << ++counter << std::endl;
	// Normalize
	for (int i = 0; i < num_samples; i++) {
		_ssss_precomputed_kernel_sampled[i*4] /= sum.x;
		_ssss_precomputed_kernel_sampled[i*4 + 1] /= sum.y;
		_ssss_precomputed_kernel_sampled[i*4 + 2] /= sum.z;
	}
	//std::cout << ++counter << std::endl;
	// TEMP put center at first
	glm::vec4 t = glm::vec4(_ssss_precomputed_kernel_sampled[(num_samples / 2) * 4], _ssss_precomputed_kernel_sampled[(num_samples / 2) * 4 + 1], _ssss_precomputed_kernel_sampled[(num_samples / 2) * 4 + 2], _ssss_precomputed_kernel_sampled[(num_samples / 2) * 4 + 3]);
	for (int i = num_samples / 2; i > 0; i--)
	{
		_ssss_precomputed_kernel_sampled[i * 4] = _ssss_precomputed_kernel_sampled[(i - 1) * 4];
		_ssss_precomputed_kernel_sampled[i * 4 + 1] = _ssss_precomputed_kernel_sampled[(i - 1) * 4 + 1];
		_ssss_precomputed_kernel_sampled[i * 4 + 2] = _ssss_precomputed_kernel_sampled[(i - 1) * 4 + 2];
		_ssss_precomputed_kernel_sampled[i * 4 + 3] = _ssss_precomputed_kernel_sampled[(i - 1) * 4 + 3];
	}
	//std::cout << ++counter << std::endl;
	//_ssss_kernel[0] = t;
	_ssss_precomputed_kernel_sampled[0] = t.x;
	_ssss_precomputed_kernel_sampled[1] = t.y;
	_ssss_precomputed_kernel_sampled[2] = t.z;
	_ssss_precomputed_kernel_sampled[3] = t.w;
	//std::cout << ++counter << std::endl;
}