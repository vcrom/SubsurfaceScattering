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
int RenderAlgorithms::_num_sss_samples = 0;

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

	fbo->useFrameBuffer(1);
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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	std::shared_ptr<GlslShader> shader = _shader_manager->getShader(GlslShaderManager::Shaders::MAIN_RENDER_SHADER);
	fbo->useFrameBuffer(4);

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
		glUniformMatrix4fv(shader->operator()("lightViewProjBiasM"), 1, GL_FALSE, glm::value_ptr(B * P_L * V_L));


		glUniformMatrix4fv(shader->operator()("lightViewM"), 1, GL_FALSE, glm::value_ptr(V_L));
		glUniformMatrix4fv(shader->operator()("lightProjBiasM"), 1, GL_FALSE, glm::value_ptr(B * P_L));
		glUniform1f(shader->operator()("light_far_plane"), light_far_plane);
		glUniform1f(shader->operator()("sssWidth"), sss_width);
		glUniform1f(shader->operator()("translucency"), translucency);
		glUniform1i(shader->operator()("sssEnabled"), int(ssss_enabled));

		glUniform1i(shader->operator()("texture_enabled"), int(use_texture));
		checkCritOpenGLError();

		mesh->render();
	shader->unUse();

	glDisable(GL_CULL_FACE);
}

std::vector<glm::vec4> initGaussians()
{
	auto gaussians = std::vector<glm::vec4>(0);
	gaussians.push_back(glm::vec4(0.0064, 1, 1, 1));
	gaussians.push_back(glm::vec4(0.0516, 0.3251, 0.45, 0.3583));
	gaussians.push_back(glm::vec4(0.2719, 0.34, 0.1864, 0.0));
	gaussians.push_back(glm::vec4(2.0062, 0.46, 0.0, 0.0402));
	for (unsigned int  i = 1; i < gaussians.size(); ++i) gaussians[i].x = gaussians[i].x - gaussians[i - 1].x;
	return gaussians;
}

std::vector<glm::vec4> RenderAlgorithms::_gaussians = initGaussians();


void RenderAlgorithms::SSSEffect(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> sss_tex, std::shared_ptr<Texture2D> sss_tex_pingpong, std::shared_ptr<Texture2D> rt1_tex, std::shared_ptr<Texture2D> rt2_tex, std::shared_ptr<Texture2D> lineal_depth, glm::vec2 pixel_size, float correction, float sssStrenth, std::shared_ptr<Texture2D> cross_bilateral_factor)
{
	std::vector<std::shared_ptr<Texture2D> >pingpong_tex = { sss_tex, sss_tex_pingpong };

	std::shared_ptr<GlslShader> horizontal = _shader_manager->getShader(GlslShaderManager::Shaders::SSSS_HORIZONTAL_BLUR);
	std::shared_ptr<GlslShader> vertical = _shader_manager->getShader(GlslShaderManager::Shaders::SSSS_VERTICAL_BLUR);

	horizontal->use();
	glUniform2fv(horizontal->operator()("pixel_size"), 1, glm::value_ptr(pixel_size));
	glUniform1f(horizontal->operator()("correction"), correction);
	glUniform1f(horizontal->operator()("sssStrength"), sssStrenth);
	vertical->use();
	glUniform2fv(vertical->operator()("pixel_size"), 1, glm::value_ptr(pixel_size));
	glUniform1f(vertical->operator()("correction"), correction);
	glUniform1f(vertical->operator()("sssStrength"), sssStrenth);

	ScreenQuad* quad = ScreenQuad::getInstanceP();


	sss_tex->use(GL_TEXTURE0);
	lineal_depth->use(GL_TEXTURE1);
	cross_bilateral_factor->use(GL_TEXTURE3);

	fbo->useFrameBuffer(1);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	int idx = 0;
	for (unsigned int i = 0; i < _gaussians.size(); ++i)
	{
		idx = i % 2;
		fbo->useFrameBuffer(1);
		fbo->colorBuffer(rt1_tex->getTextureID(), 0);
		horizontal->use();
		glUniform4fv(horizontal->operator()("gaussian"), 1, glm::value_ptr(_gaussians[i]));
		quad->render();

		rt1_tex->use(GL_TEXTURE0);
		fbo->useFrameBuffer(2);
		fbo->colorBuffer(rt2_tex->getTextureID(), 0);
		fbo->colorBuffer(pingpong_tex[idx]->getTextureID(), 1);
		pingpong_tex[(i + 1) % 2]->use(GL_TEXTURE2);
		vertical->use();
		glUniform4fv(vertical->operator()("gaussian"), 1, glm::value_ptr(_gaussians[i]));
		quad->render();
		rt2_tex->use(GL_TEXTURE0);
	}

	if (idx != 0)
	{
		fbo->useFrameBuffer(1);
		fbo->colorBuffer(sss_tex->getTextureID(), 0);
		renderTexture(fbo, pingpong_tex[idx]);
	}

	checkCritOpenGLError();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void RenderAlgorithms::separableSSSSEffect(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> sss_tex, std::shared_ptr<Texture2D> rt1_tex, std::shared_ptr<Texture2D> lineal_depth, float cam_fovy, float sssWidth, std::shared_ptr<Texture2D> cross_bilateral_factor)
{

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
	fbo->useFrameBuffer();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	sss_tex->use(GL_TEXTURE0);
	fbo->colorBuffer(rt1_tex->getTextureID(), 0);
	horizontal->use();
	quad->render();

	rt1_tex->use(GL_TEXTURE0);
	fbo->colorBuffer(sss_tex->getTextureID(), 0);
	vertical->use();
	quad->render();

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
	glUniform1f(shader->operator()("m_burnout"), burnout); 
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


glm::vec3 gaussian(float variance, float r, const std::vector<float> &falloff) {
	/**
	* We use a falloff to modulate the shape of the profile. Big falloffs
	* spreads the shape making it wider, while small falloffs make it
	* narrower.
	*/
	glm::vec3 g;
	for (int i = 0; i < 3; i++) {
		float rr = r / (0.001f + falloff[i]);
		g[i] = exp((-(rr * rr)) / (2.0f * variance)) / (2.0f * 3.14f * variance);
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
	return  // 0.233f * gaussian(0.0064f, r) + /* We consider this one to be directly bounced light, accounted by the strength parameter (see @STRENGTH) */
		0.100f * gaussian(0.0484f, r, falloff) +
		0.118f * gaussian(0.187f, r, falloff) +
		0.113f * gaussian(0.567f, r, falloff) +
		0.358f * gaussian(1.99f, r, falloff) +
		0.078f * gaussian(7.41f, r, falloff);
}

void RenderAlgorithms::computeSeparableKernel(int num_samples, const glm::vec3 &sss_strength, std::vector<float> &falloff)
{
		//std::vector<glm::vec4> kernel(num_samples);

		const float RANGE = num_samples > 20 ? 3.0f : 2.0f;
		const float EXPONENT = 2.0f;

		_ssss_kernel = std::vector<float> (num_samples * 4);
		_num_sss_samples = num_samples;

		// Calculate the offsets:
		float step = 2.0f * RANGE / (num_samples - 1);
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

		// Finally, set 'em!
		//V(kernelVariable->SetFloatVectorArray((float *)&_ssss_kernel.front(), 0, num_samples));
		setSeparableKernels();
}

void RenderAlgorithms::setSeparableKernels()
{
	std::shared_ptr<GlslShader> horizontal = _shader_manager->getShader(GlslShaderManager::Shaders::SEPARABLE_SSSS_HORIZONTAL_BLUR);

	horizontal->use();
	glUniform1i(horizontal->operator()("ssss_n_samples"), _num_sss_samples);
	glUniform4fv(horizontal->operator()("kernel"), _num_sss_samples, &_ssss_kernel[0]);
	horizontal->unUse();

	std::shared_ptr<GlslShader> vertical = _shader_manager->getShader(GlslShaderManager::Shaders::SEPARABLE_SSSS_VERTICAL_BLUR);

	vertical->use();
	glUniform1i(vertical->operator()("ssss_n_samples"), _num_sss_samples);
	glUniform4fv(vertical->operator()("kernel"), _num_sss_samples, &_ssss_kernel[0]);
	vertical->unUse();
	checkCritOpenGLError();
}