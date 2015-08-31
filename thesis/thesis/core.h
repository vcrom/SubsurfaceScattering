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
#include "skybox.h"
	

class Core
{
public:
	using uint = unsigned int;

	Core();
	~Core();

	void initialize();
	void onRender();
	void resize(uint w, uint h);
	void setDefaultFBO(GLuint fbo);

	//loading unloading
	void loadMesh(const std::string& path);
	void unloadMesh();
	void loadMeshDiffuseTexture(const std::string& path);
	void loadMeshAOTexture(const std::string& path);
	void loadMeshNormalsTexture(const std::string& path);


	//control
	enum Input { NOTHING, MOUSE_LEFT_BUTTON, MOUSE_RIGHT_BUTTON };
	void mouseclick(int x, int y, Input button);
	void mouseMoved(int x, int y, Input button);
	void mouseReleased(int x, int y, Input button);

	void toggleControlBool(unsigned int i);
	void setSSSMethod(int val);
	void setToneMappingMethod(int val);


	void reloadShaders();
	void moveLight(glm::vec3 dir);
	void setTranslucency(float t);
	void setSSWidth(float w);
	void setExposure(float e);
	void setAmbientInt(float a);
	void setSpeculartInt(float s);

	//s4 params
	void setSSSRedStr(float s);
	void setSSSGreenStr(float s);
	void setSSSBlueStr(float s);
	void setSSSNumSamples(int s);
	void setGlossines(float g);

	//float _sss_width, _translucency, _correction, _sssStrength, _exposure, _burnout;


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
	void toneMap();

	
	//cammera
	TargetCamera _cam;

	//Textures
	std::shared_ptr<Texture2D> _background_texture, _mesh_diffuse_texture, _mesh_ao_texture, _mesh_normals_texture,
		//shadow mapping(for each light)
		_shadow_map_texture, _lineal_shadow_map_texture,

		//ssss
		_depth_stencil_texture, _diffuse_color_texture, _specular_texture, _lineal_depth_texture, _aux_ssss_texture1, _aux_ssss_texture2, _aux_ssss_pingpong,

		//Filter aux
		_cross_bilateral_factor, _curvature_tex,
		
		//pbr
		_diffuse_env_texture;

	std::string _enviroment_path;

	void initializeTextures();
	void resizeTextures(unsigned int w, unsigned int h);

	//Buffers
	std::shared_ptr<FrameBuffer> _default_buffer, _generic_buffer;

	std::shared_ptr<Mesh> _sphere;
	std::shared_ptr<Entity> _object, _light;
	std::shared_ptr<CSkybox> _sky_box;

	//control params
	std::vector<bool> _control_boolean_params;
	int _sss_method, _tone_mapping_method;
	glm::vec2 _window_size;

	int _mouse_x, _mouse_y;

	//timmer
	std::chrono::high_resolution_clock _clock;
	std::chrono::high_resolution_clock::time_point _t1, _t2;

	//params
	glm::mat4 _light_view_matrix, _light_projection_matrix;
	void computeLightMatrices();

	glm::mat4 _prev_VP;

	const float ALPHA_MAX = 8192.0f;
	float _sss_width, _translucency, _correction, _sssStrength, _exposure, _burnout, _ambientInt, _specInt, _glossines, _roughness;
	glm::vec2 _pixel_size;
	int _num_samples;
	glm::vec3 _sss_strength;
	std::vector<float> _falloff;
		

};

#endif // CORE_H
