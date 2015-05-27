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
	void loadMesh(const std::string& path);
	void unloadMesh();
	void setDefaultFBO(GLuint fbo);

	//control
	enum Input { NOTHING, MOUSE_LEFT_BUTTON, MOUSE_RIGHT_BUTTON };
	void mouseclick(int x, int y, Input button);
	void mouseMoved(int x, int y, Input button);
	void mouseReleased(int x, int y, Input button);

	void toggleControlBool(unsigned int i);


private:
	void initializeGL();
	void glewInitialization();
	void initializeCam();

	TargetCamera _cam;

	//Texture Managment
	//Texture2D *tex, *tex_col;
	std::shared_ptr<Texture2D> _background_texture, _shadow_map_texture;
	std::shared_ptr<FrameBuffer> _default_buffer, _buffer;

	std::shared_ptr<Mesh> _sphere;
	std::shared_ptr<Entity> _object, _light;

	//control params
	std::vector<bool> _control_boolean_params;

	int _mouse_x, _mouse_y;

	//constants
	const float ROT_SPEED = 0.5f;
	const float ZOOM_SPEED = 0.05f;
};

#endif // CORE_H
