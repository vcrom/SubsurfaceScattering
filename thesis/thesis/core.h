#ifndef CORE_H
#define CORE_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <memory>
#include <chrono>
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
	void onRender();
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

	void reloadShaders();
	void moveLight(glm::vec3 dir);

private:
	//constants
	const float ROT_SPEED = 0.5f;
	const float ZOOM_SPEED = 0.05f;
	const float LIGHT_MOV_SPEED = 0.025f;

	//initialization
	void initializeGL();
	void glewInitialization();
	void initializeCam();

	//Render passes 
	//void renderBackground();
	void renderScene();
	void shadowMapPass();
	void mainRenderPass();// , float time, float elapsedTime);
	void subSurfaceScatteringPass();
	void addSpecularPass();

	
	//cammera
	TargetCamera _cam;

	//Textures
	std::shared_ptr<Texture2D> _background_texture, 
		//shadow mapping(for each light)
		_shadow_map_texture, _lineal_shadow_map_texture, 
		//ssss
		_depth_stencil_texture, _diffuse_color_texture, _specular_texture, _lineal_depth_texture, _aux_ssss_texture;

	void initializeTextures();
	void resizeTextures(unsigned int w, unsigned int h);

	//Buffers
	std::shared_ptr<FrameBuffer> _default_buffer, _generic_buffer;

	std::shared_ptr<Mesh> _sphere;
	std::shared_ptr<Entity> _object, _light;

	//control params
	std::vector<bool> _control_boolean_params;
	glm::vec2 _window_size;

	int _mouse_x, _mouse_y;

	//timmer
	std::chrono::high_resolution_clock _clock;
	std::chrono::high_resolution_clock::time_point _t1, _t2;

	//params
	glm::mat4 _light_view_matrix, _light_projection_matrix;
	void computeLightMatrices();

	glm::mat4 _prev_VP;

	float sss_width, translucency;
		

};

#endif // CORE_H
