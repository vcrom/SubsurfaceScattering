#ifndef RENDERALGORITHMS_H
#define RENDERALGORITHMS_H

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

	static void renderTexture(const FrameBuffer& fbo, const Texture2D& tex);
	static void renderMesh(const FrameBuffer& fbo, const Mesh *mesh, glm::mat4 V, glm::mat4 P);
	//void render(std::vector<);
private:
	static GlslShaderManager *shader_manager;
};

#endif // RENDERALGORITHMS_H
