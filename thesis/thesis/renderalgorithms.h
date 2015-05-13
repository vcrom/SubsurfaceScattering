#ifndef RENDERALGORITHMS_H
#define RENDERALGORITHMS_H

#include "glslshadermanager.h"
#include "texture2d.h"
#include "framebuffer.h"

class RenderAlgorithms
{
public:
	RenderAlgorithms();
	~RenderAlgorithms();
	static GLuint default_buffer;

	static void renderTexture(FrameBuffer fbo, const Texture2D& tex);
	//void render(std::vector<);
private:
	static GlslShaderManager *shader_manager;
};

#endif // RENDERALGORITHMS_H
