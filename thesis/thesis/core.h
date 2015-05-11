#ifndef CORE_H
#define CORE_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

#include "targetcamera.h"
#include "glslshader.h"

#include "textureloader.h"

#include "glslshadermanager.h"

class Core
{
public:
	Core();
	~Core();

	void initialize();
	void render();
	void resize(unsigned int w, unsigned int h);
	void loadMesh(const std::string& path);
	void setDefaultFBO(GLuint fbo);

private:
	void initializeGL();
	void glewInitialization();

	TargetCamera cam_;
	GlslShader shader;
	Texture2D *tex;
	GlslShaderManager *shader_manager = GlslShaderManager::instance();
	//GlslShaderManager shader_manager;
	//std::vector<
	
};

#endif // CORE_H
