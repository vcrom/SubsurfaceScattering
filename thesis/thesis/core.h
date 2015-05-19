#ifndef CORE_H
#define CORE_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

#include "targetcamera.h"
#include "glslshader.h"

#include "textureloader.h"

#include "glslshadermanager.h"
#include "framebuffer.h"
#include "meshimporter.h"
	
class Core
{
public:
	using uint = unsigned int;

	Core();
	~Core();

	void initialize();
	void render();
	void resize(uint w, uint h);
	void loadMesh(const std::string& path);
	void setDefaultFBO(GLuint fbo);

	enum Input { NOTHING, MOUSE_LEFT_BUTTON, MOUSE_RIGHT_BUTTON };
	void mouseclick(int x, int y, Input button);
	void mouseMoved(int x, int y, Input button);
	void mouseReleased(int x, int y, Input button);

private:
	void initializeGL();
	void glewInitialization();

	TargetCamera _cam;
	Texture2D *tex, *tex_col;
	GlslShaderManager *shader_manager = GlslShaderManager::instance();
	FrameBuffer *buffer, *_qt_buffer;

	Mesh *mesh;

	//constants
	const float ROT_SPEED = 0.5f;
	int _mouse_x, _mouse_y;
	//GlslShaderManager shader_manager;
	//std::vector<
	
};

#endif // CORE_H
