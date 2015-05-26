#ifndef CORE_H
#define CORE_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <memory>
//#include <vld.h>

#include "targetcamera.h"
#include "glslshader.h"

#include "textureloader.h"

#include "glslshadermanager.h"
#include "framebuffer.h"
#include "meshimporter.h"
#include "entity.h"
	
class Core
{
public:
	using uint = unsigned int;

	Core();
	~Core();

	void initialize();
	void render();
	void resize(uint w, uint h);
	//void loadMesh(const std::string& path);
	//void queueToLoadMesh(const std::string& path);
	void loadMesh(const std::string& path);
	void unloadMesh();
	void setDefaultFBO(GLuint fbo);

	enum Input { NOTHING, MOUSE_LEFT_BUTTON, MOUSE_RIGHT_BUTTON };
	void mouseclick(int x, int y, Input button);
	void mouseMoved(int x, int y, Input button);
	void mouseReleased(int x, int y, Input button);

private:
	void initializeGL();
	void glewInitialization();
	void initializeCam();

	//void processMeshLoadingEvents();
	//void loadMesh(const std::string& path);

	TargetCamera _cam;
	Texture2D *tex, *tex_col;
	std::shared_ptr<FrameBuffer> _qt_buffer, _buffer;

	std::shared_ptr<Mesh> _sphere;
	std::shared_ptr<Entity> _object, _light;

	//constants
	const float ROT_SPEED = 0.5f;
	const float ZOOM_SPEED = 0.05f;
	int _mouse_x, _mouse_y;	
};

#endif // CORE_H
