#ifndef RENDERALGORITHMS_H
#define RENDERALGORITHMS_H

#include <memory>

#include "glslshadermanager.h"
#include "texture2d.h"
#include "framebuffer.h"
#include "mesh.h"

#include "glm\glm.hpp"

class RenderAlgorithms
{
public:
	RenderAlgorithms();
	~RenderAlgorithms();
	static GLuint default_buffer;
	//static void initialize(unsigned int num_tex);
	//static void resizeTextures(unsigned int w, unsigned int h);
	//static void resizeTexture(unsigned int id, unsigned int w, unsigned int h);

	static void renderTexture(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> tex);
	static void renderMesh(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 col = glm::vec3(1));
	static void getShadowMap(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, const glm::vec2 &viewport_size, const glm::vec2 &shadow_buffer_size);
	static void getLinealShadowMap(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, float z_far, const glm::vec2 &viewport_size, const glm::vec2 &shadow_buffer_size, const glm::vec3 &light_pos);
	static void renderShadows(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, std::shared_ptr<Texture2D> shadow_tex, glm::mat4 V_L, glm::mat4 P_L, glm::vec3 light_pos);
	//static void renderThickness(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, float z_far);//, const glm::vec2 &viewport_size, const glm::vec2 &shadow_buffer_size);

	static void translucency(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, std::shared_ptr<Texture2D> lineal_deepth_tex, float z_far, glm::mat4 V_L, glm::mat4 P_L, glm::vec3 light_pos);
	static void renderDiffuseAndSpecular(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, 
		glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::mat4 prev_VP, 
		glm::vec3 camera_pos, float z_far, 
		glm::vec3 light_pos, std::shared_ptr<Texture2D> shadow_tex, glm::mat4 V_L, glm::mat4 P_L, 
		std::shared_ptr<Texture2D> light_linear_shadow_tex, float light_far_plane, 
		float sss_width, float translucency, float ambient_int, float specular_int, bool ssss_enabled = true, 
		std::shared_ptr<Texture2D> diffuse_texture = nullptr, bool use_texture = false);


	static void SSSEffect(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> sss_tex, std::shared_ptr<Texture2D> sss_tex_pingpong, std::shared_ptr<Texture2D> rt1_tex, std::shared_ptr<Texture2D> rt2_tex, std::shared_ptr<Texture2D> lineal_depth, glm::vec2 pixel_size, float correction, float sssStrenth);
	static void separableSSSSEffect(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> sss_tex, std::shared_ptr<Texture2D> rt1_tex, std::shared_ptr<Texture2D> lineal_depth, float cam_fovy, float sssWidth);
	static void computeSeparableKernel(int num_samples, const glm::vec3 &sss_strength, std::vector<float> &falloff);

	static void toneMapTexture(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> tex, float exposure, float burnout, int method = 0);
	//void render(std::vector<);
private:
	static std::shared_ptr<GlslShaderManager> _shader_manager;
	//static std::vector<Texture2D> _aux_textures;
	//static FrameBuffer buffer;

	static bool checkGLEnabled(GLenum param);

	static std::vector<glm::vec4> _gaussians;
};

#endif // RENDERALGORITHMS_H
