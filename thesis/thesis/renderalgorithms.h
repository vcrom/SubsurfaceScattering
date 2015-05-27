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
	static void initialize(unsigned int num_tex);
	static void resizeTextures(unsigned int w, unsigned int h);
	static void resizeTexture(unsigned int id, unsigned int w, unsigned int h);

	static void renderTexture(const std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<Texture2D> tex);
	static void renderMesh(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 col = glm::vec3(1));
	static void getShadowMap(const std::shared_ptr<FrameBuffer> fbo, const std::shared_ptr<Mesh> mesh, glm::mat4 M, glm::mat4 V, glm::mat4 P);

	//void render(std::vector<);
private:
	static std::shared_ptr<GlslShaderManager> _shader_manager;
	static std::vector<Texture2D> _aux_textures;
	static FrameBuffer buffer;

	static bool checkGLEnabled(GLenum param);
};

#endif // RENDERALGORITHMS_H
