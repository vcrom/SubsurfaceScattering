#include "core.h"

#include "utils.h"
#include "meshimporter.h"
#include "renderalgorithms.h"
#include "kxtime.h"

#include <iostream>
#include <chrono>
#include <fstream>
//#include <thread>

Core::Core()
{
	_use_textures = true;
	_mouse_x = _mouse_y = 0;

	_window_size = glm::vec2(0);
	_prev_VP = glm::mat4(0);
	_sss_width = 0.005;//0.0117500005;
	_translucency = 0.95;
	_correction = 1700;
	_pixel_size = glm::vec2(1);
	_exposure = 2;
	_ambientInt = 1;
	_specInt = 1;
	_burnout = std::numeric_limits<float>::infinity();


	_control_boolean_params = std::vector<bool>(10, false);
	_control_boolean_params[2] = true;
	_sss_method = 0;
	_tone_mapping_method = 0;

	_ssss_mod_factor = 0.5f;
	_num_samples = 20;
	_sss_strength = glm::vec3(0.48, 0.41, 0.28);
	_falloff = {1.0f, 0.37f, 0.3f};
	_bumpint = 0.5f;

	std::function<void()> syncOpenGl = [&]{glFinish();};
	kxt::set_sync_function(syncOpenGl);
	kxt::set_sample_frequency(50);
	kxt::init();
	
	_num_frames = 0;
	_timed_regions = std::vector<std::string> {"shadow mapping", "main render", "Subsurface simulation", "Specular", "Tonemapping"};
	_timmings = std::vector<double>(_timed_regions.size(), 0);
	
}

#include "fimage.h"
Core::~Core()
{
	fImage image;
	_object.reset();
	kxt::quit();
}

/// <summary>
/// Initializes OpenGL Extension Wrangler Library.
/// </summary>
void Core::glewInitialization()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	}
	else {
		if (GLEW_VERSION_3_3)
			std::cout << "Driver supports OpenGL 3.3\nDetails:" << std::endl;
		else
			throw_critical("Driver does not support openGL 3.3");
	}
	//err = glGetError(); //this is to ignore INVALID ENUM error 1282
	checkCritOpenGLError();

	//output hardware information
	std::cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "\tVendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "\tRenderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "\tVersion: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	int aux;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &aux);
	std::cout << "\tMax uniform comp: " << aux << std::endl;
	glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &aux);
	std::cout << "\tMax varying comp: " << aux << std::endl;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &aux);
	std::cout << "\tMax drawBuffers comp: " << aux << std::endl;
	checkCritOpenGLError();
}

/// <summary>
/// Initializes the openGl enviroment.
/// </summary>
void Core::initializeGL()
{
	glewInitialization();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearStencil(0);

	std::cout << "OpenGL initialized" << std::endl;
}


/// <summary>
/// Initializes the textures.
/// </summary>
void Core::initializeTextures()
{
	//Shadow map texture
	_shadow_map_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_shadow_map_texture->use();
	_shadow_map_texture->loadEmptyTexture(GL_DEPTH_COMPONENT, 32, 32);
	_shadow_map_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_shadow_map_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLfloat border[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	_shadow_map_texture->setTexParameter(GL_TEXTURE_BORDER_COLOR, border);
	_shadow_map_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	_shadow_map_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// This must be specified in order to allow usage of shadow2DProj function in the shader
	_shadow_map_texture->setTexParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	_shadow_map_texture->setTexParameter(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	checkCritOpenGLError();

	//lineal shadow map
	_lineal_shadow_map_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_lineal_shadow_map_texture->use();
	_lineal_shadow_map_texture->loadEmptyTexture(GL_R16F, 32, 32);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_BORDER_COLOR, border);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	//lineal shadow map
	_aux_blur_tex = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_aux_blur_tex->use();
	_aux_blur_tex->loadEmptyTexture(GL_R16F, 32, 32);
	_aux_blur_tex->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_aux_blur_tex->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_aux_blur_tex->setTexParameter(GL_TEXTURE_BORDER_COLOR, border);
	_aux_blur_tex->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	_aux_blur_tex->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();


	_depth_stencil_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_depth_stencil_texture->use();
	_depth_stencil_texture->loadEmptyTexture(GL_DEPTH24_STENCIL8, 32, 32);
	_depth_stencil_texture->setTexParameter(GL_TEXTURE_BORDER_COLOR, border);
	_depth_stencil_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	_depth_stencil_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	_depth_stencil_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	_depth_stencil_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	_diffuse_color_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_diffuse_color_texture->use();
	_diffuse_color_texture->loadEmptyTexture(GL_RGBA32F, 32, 32);
	_diffuse_color_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_diffuse_color_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_diffuse_color_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_diffuse_color_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	_albedo_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_albedo_texture->use();
	_albedo_texture->loadEmptyTexture(GL_RGBA32F, 32, 32);
	_albedo_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_albedo_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_albedo_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_albedo_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkCritOpenGLError();

	_specular_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_specular_texture->use();
	_specular_texture->loadEmptyTexture(GL_RGBA32F, 32, 32);
	_specular_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_specular_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_specular_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_specular_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	_lineal_depth_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_lineal_depth_texture->use();
	_lineal_depth_texture->loadEmptyTexture(GL_R16F, 32, 32);
	_lineal_depth_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_lineal_depth_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_lineal_depth_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_lineal_depth_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	_aux_ssss_texture1 = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_aux_ssss_texture1->use();
	_aux_ssss_texture1->loadEmptyTexture(GL_RGBA32F, 32, 32);
	_aux_ssss_texture1->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_aux_ssss_texture1->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_aux_ssss_texture1->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_aux_ssss_texture1->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	_aux_ssss_texture2 = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_aux_ssss_texture2->use();
	_aux_ssss_texture2->loadEmptyTexture(GL_RGBA32F, 32, 32);
	_aux_ssss_texture2->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_aux_ssss_texture2->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_aux_ssss_texture2->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_aux_ssss_texture2->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	_aux_ssss_pingpong = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_aux_ssss_pingpong->use();
	_aux_ssss_pingpong->loadEmptyTexture(GL_RGBA32F, 32, 32);
	_aux_ssss_pingpong->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_aux_ssss_pingpong->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_aux_ssss_pingpong->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_aux_ssss_pingpong->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	//CBF factor
	_cross_bilateral_factor = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_cross_bilateral_factor->use();
	_cross_bilateral_factor->loadEmptyTexture(GL_R32F, 32, 32);
	_cross_bilateral_factor->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_cross_bilateral_factor->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_cross_bilateral_factor->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_cross_bilateral_factor->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	checkCritOpenGLError();

	//Curvature texture
	_curvature_tex = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_curvature_tex->use();
	_curvature_tex->loadEmptyTexture(GL_R32F, 32, 32);
	_curvature_tex->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_curvature_tex->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_curvature_tex->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_curvature_tex->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	checkCritOpenGLError();

	
	loadMeshDiffuseTexture("digital_emily/Emily_albedo.jpg");
	loadMeshAOTexture("digital_emily/EmilyAO.jpg");
	loadMeshNormalsTexture("digital_emily/Emily_normals_c.jpg");
	checkCritOpenGLError();

	std::cout << "Textures init" << std::endl;
}

/// <summary>
/// Initializes core objects and openGL context
/// </summary>
void Core::initialize()
{
	initializeGL();
	initializeTextures();

	//init shader manager
	std::shared_ptr<GlslShaderManager> shader_manager = GlslShaderManager::instance();
	shader_manager->initializeShaders();


	//Init default buffer
	_default_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(RenderAlgorithms::default_buffer, 1));

	_generic_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer());
	_generic_buffer->createFrameBuffer();

	//Init cam projection
	_cam.updateProjection(CAM_FOVY, 1.0f);

	//light
	_sphere = MeshImporter::importMeshFromFile("meshes/sphere.ply");
	_light = std::shared_ptr<Entity>(new Entity(_sphere));
	_light->setUnitary();
	_light->scale(glm::vec3(0.19));
	_light->setPosition(glm::vec3(1.5, 0.4, 0.9));

	//Load skybox and pbr envs
	_sky_box = std::shared_ptr<CSkybox>(new CSkybox());
	loadEnviroment("textures/env2");

	//loadFace Maesh
	loadMesh("digital_emily/Emily_Head.ply");

	//load kernel
	loadPreComputedKernel("kernels/Skin1_PreInt_DISCSEP.bn");

	//Compute kernel
	RenderAlgorithms::computeKernels(_num_samples, _sss_strength, _falloff);
	RenderAlgorithms::setSeparableKernels(_sss_method%2);

}

void Core::initializeCam()
{
	if (_object) _cam.initFromBBox(_object.get()->getBBox());
	_cam.update();
}


void Core::resizeTextures(unsigned int w, unsigned int h)
{
	GLsizei shadow_width, shadow_height, width, height;
	width = GLsizei(int(w));
	height = GLsizei(int(h));
	shadow_width = width;
	shadow_height = height;
	//shadow_width = 2048; //w
	//shadow_height = 2048; //h

	_shadow_map_texture->use(); 
	_shadow_map_texture->resize(shadow_width, shadow_height);

	_lineal_shadow_map_texture->use();
	_lineal_shadow_map_texture->resize(shadow_width, shadow_height);

	_aux_blur_tex->use();
	_aux_blur_tex->resize(shadow_width, shadow_height);

	_depth_stencil_texture->use();
	_depth_stencil_texture->resize(width, height);

	_aux_ssss_texture1->use();
	_aux_ssss_texture1->resize(width, height);

	_aux_ssss_texture2->use();
	_aux_ssss_texture2->resize(width, height);

	_diffuse_color_texture->use();
	_diffuse_color_texture->resize(width, height);

	_albedo_texture->use();
	_albedo_texture->resize(width, height);

	_specular_texture->use();
	_specular_texture->resize(width, height);

	_lineal_depth_texture->use();
	_lineal_depth_texture->resize(width, height);

	_aux_ssss_pingpong->use();
	_aux_ssss_pingpong->resize(width, height);

	_cross_bilateral_factor->use();
	_cross_bilateral_factor->resize(width, height);

	_curvature_tex->use();
	_curvature_tex->resize(width, height);
}

void Core::resize(unsigned int w, unsigned int h)
{
	std::cout << "Resize(" << w << ", " << h << ")" << std::endl;
	_window_size = glm::vec2(int(w), int(h));
	_pixel_size = glm::vec2(1.0 / float(w), 1.0 / float(h));
	glViewport(0, 0, GLint(w), GLint(h));
	_cam.updateProjection(CAM_FOVY, float(w) / float(h));
	_cam.update();
	resizeTextures(w, h);
	computeLightMatrices();
	checkCritOpenGLError();
}

//#include "screenquad.h"
#include <glm/gtc/matrix_transform.hpp>
void Core::onRender()
{
	//std::cout << _cam.getZfar() << std::endl;
	if (!_control_boolean_params[0])
	{
		renderScene();
		return;
	}
	_default_buffer->useFrameBuffer();
	_default_buffer->clearColorAndDepth();
	
	RenderAlgorithms::renderMesh(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _light_view_matrix, _light_projection_matrix, glm::vec3(1, 0, 0));
}

void Core::renderScene()
{
	_default_buffer->useFrameBuffer();
	_default_buffer->clearColorAndDepth();
	std::cout << "Rendering scene..." << std::endl;

	kxt::frame_start();
	kxt::region_start(_timed_regions[0]);
	shadowMapPass();
	kxt::region_end(_timed_regions[0]);

	kxt::region_start(_timed_regions[1]);
	mainRenderPass();
	kxt::region_end(_timed_regions[1]);

	kxt::region_start(_timed_regions[2]);
	if(_control_boolean_params[2]) subSurfaceScatteringPass();
	kxt::region_end(_timed_regions[2]);

	kxt::region_start(_timed_regions[3]);
	addSpecularPass();
	kxt::region_end(_timed_regions[3]);
	
	kxt::region_start(_timed_regions[4]);
	toneMap();
	kxt::region_end(_timed_regions[4]);
	
	kxt::frame_end();

	for (unsigned int i = 0; i < _timmings.size(); ++i)
	{
		_timmings[i] = _timmings[i] + (kxt::region_duration(_timed_regions[i])*1000 - _timmings[i]) / double(_num_frames + 1);
		std::cout << _timed_regions[i] << ": " << _timmings[i] << std::endl;
	}
	++_num_frames;
	std::cout << "Frame: " << _num_frames << std::endl;
}

/// <summary>
/// Render pas which renders the scene from the light point of view and gets it's shadowmaps
/// </summary>
void Core::shadowMapPass()
{
	_lineal_shadow_map_texture->use(GL_TEXTURE0);
	_shadow_map_texture->use(GL_TEXTURE1);
	//_lineal_depth_texture->use(GL_TEXTURE2);

	_generic_buffer->useFrameBuffer(2);
	_generic_buffer->colorBuffer(_lineal_shadow_map_texture->getTextureID(), 0);
	_generic_buffer->depthBuffer(_shadow_map_texture->getTextureID());
	_generic_buffer->stencilBuffer(0);
	_generic_buffer->clearColorAndDepth();

	RenderAlgorithms::getLinealShadowMap(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), _light_view_matrix, _light_projection_matrix, _cam.getZfar(), _window_size, glm::vec2(_lineal_shadow_map_texture->getWidth(), _lineal_shadow_map_texture->getHeight()), _light->getPosition());

	//_generic_buffer->useFrameBuffer();
	//_generic_buffer->colorBuffer(_aux_blur_tex->getTextureID(), 0);
	//_generic_buffer->depthBuffer(0);
	//_generic_buffer->clearColor();
	//RenderAlgorithms::blurTexture(_generic_buffer, _lineal_shadow_map_texture, _aux_blur_tex, _pixel_size);
}

void getMinViewZPoint()
{

}

#include<algorithm>
/// <summary>
/// Mains the render pass, which renders the scene(head)
/// </summary>
void Core::mainRenderPass()
{

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);

	_generic_buffer->useFrameBuffer(6);
	_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);//difuse
	_generic_buffer->colorBuffer(_lineal_depth_texture->getTextureID(), 1);//lin depth
	_generic_buffer->colorBuffer(_specular_texture->getTextureID(), 2);//specular
	_generic_buffer->colorBuffer(_cross_bilateral_factor->getTextureID(), 3);//cross-bilateral
	_generic_buffer->colorBuffer(_curvature_tex->getTextureID(), 4);//curvature
	_generic_buffer->colorBuffer(_albedo_texture->getTextureID(), 5);//albedo
	_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());
	_generic_buffer->clearColorDepthAndStencil();

	checkCritOpenGLError();

	RenderAlgorithms::renderDiffuseAndSpecular(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), _prev_VP, 
		_cam.getPosition(), _cam.getZfar(), _light->getPosition(), _cam.getZnear(), _roughness, _bumpint, _diffuse_env_texture,
		_shadow_map_texture, _light_view_matrix, _light_projection_matrix, _lineal_shadow_map_texture, _cam.getZfar(), 
		_sss_width, _translucency, _ambientInt, _specInt, _control_boolean_params[2], 
		_mesh_diffuse_texture, _mesh_ao_texture, _mesh_normals_texture, _use_textures);

	_prev_VP = _cam.getProjectionMatrix()*_cam.getViewMatrix();
	_generic_buffer->useFrameBuffer(3);
	_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 2);//specular

	glDisable(GL_STENCIL_TEST);

	_generic_buffer->useFrameBuffer();
	_generic_buffer->colorBuffer(_aux_blur_tex->getTextureID(), 0);
	_generic_buffer->clearColor();
	_generic_buffer->depthBuffer(0);
	RenderAlgorithms::blurTexture(_generic_buffer, _curvature_tex, _aux_blur_tex, _pixel_size);
}

void Core::subSurfaceScatteringPass()
{
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);
	_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());
	switch (_sss_method)
	{
	case 0: //SSSS
	case 1: //precomputed kernel
		//glDisable(GL_STENCIL_TEST);
		_generic_buffer->useFrameBuffer(3);
		_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 0);
		_generic_buffer->colorBuffer(_aux_ssss_texture2->getTextureID(), 1);
		_generic_buffer->colorBuffer(_aux_ssss_pingpong->getTextureID(), 2);
		_generic_buffer->clearColor();
		//_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());

		RenderAlgorithms::separableSSSSEffect(_generic_buffer, _diffuse_color_texture, _aux_ssss_texture1, _lineal_depth_texture, _cam.getFOV(), _sss_width*_ssss_mod_factor*0.9, _cross_bilateral_factor, _curvature_tex, _albedo_texture);
		_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);
		break;
	case 2: //Perceptual variable #samples
		_generic_buffer->useFrameBuffer(3);
		_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 0);
		_generic_buffer->colorBuffer(_aux_ssss_texture2->getTextureID(), 1);
		_generic_buffer->colorBuffer(_aux_ssss_pingpong->getTextureID(), 2);
		_generic_buffer->clearColor();
		//_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());

		RenderAlgorithms::GaussianSSSEffect(_generic_buffer, _diffuse_color_texture, _aux_ssss_pingpong, _aux_ssss_texture1, _aux_ssss_texture2, _lineal_depth_texture, _pixel_size, _correction, _sss_width*0.2/*_sssStrength*/, _cam.getFOV(), _cross_bilateral_factor, _curvature_tex, _albedo_texture);
		_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);
		break;
	case 3: //Perceptual
		_generic_buffer->useFrameBuffer(3);
		_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 0);
		_generic_buffer->colorBuffer(_aux_ssss_texture2->getTextureID(), 1);
		_generic_buffer->colorBuffer(_aux_ssss_pingpong->getTextureID(), 2);
		_generic_buffer->clearColor();
		//_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());

		RenderAlgorithms::SSSEffect(_generic_buffer, _diffuse_color_texture, _aux_ssss_pingpong, _aux_ssss_texture1, _aux_ssss_texture2, _lineal_depth_texture, _pixel_size, _correction, _sss_width*0.2/*_sssStrength*/, _cam.getFOV(), _cross_bilateral_factor, _curvature_tex, _albedo_texture);
		_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);
		break;
	default:
		break;
	}
	glDisable(GL_STENCIL_TEST);
}

/// <summary>
/// This render step adds the specular component to the sss part
/// </summary>
void Core::addSpecularPass()
{
	//RenderAlgorithms::renderTexture(_default_buffer, _diffuse_color_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	_generic_buffer->useFrameBuffer(1);
	_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);
	RenderAlgorithms::renderTexture(_generic_buffer, _specular_texture);
	glDisable(GL_BLEND);
}

#include <glm/gtc/type_ptr.hpp>
/// <summary>
/// This render step does the tonne mapping of teh renderer scene and also outputs the background
/// </summary>
void Core::toneMap()
{
	_generic_buffer->useFrameBuffer(1);
	_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 0);
	_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());
	_generic_buffer->clearColor();

	RenderAlgorithms::toneMapTexture(_generic_buffer, _diffuse_color_texture, _exposure, _burnout, _tone_mapping_method);

	//render background
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 0, 0xFF);
	glStencilMask(0x00);
	//RenderAlgorithms::renderTexture(_generic_buffer, _background_texture);
	_generic_buffer->useFrameBuffer();
	glm::mat4 P_sky = glm::perspective(_cam.getFOV(), _cam.getAspectRatio(), 50.0f, 200.0f);
	glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(100.0f));
	_sky_box->render(glm::value_ptr(P_sky * _cam.getViewMatrix()* S));
	glDisable(GL_STENCIL_TEST);


	RenderAlgorithms::renderTexture(_default_buffer, _aux_ssss_texture1);

	if (_control_boolean_params[1])
	{
		RenderAlgorithms::renderTexture(_default_buffer, _lineal_shadow_map_texture);
	}
}

/// <summary>
/// Loads the mesh from path. In order to efectivelly load the mesh, teh openGl context must be active
/// </summary>
/// <param name="path">The path of teh mesh.</param>
void Core::loadMesh(const std::string& path)
{
	unloadMesh();
	_object = std::shared_ptr<Entity>(new Entity(MeshImporter::importMeshFromFile(path, true)));
	_object->setUnitary();
	_object->translateToOrigin();
	initializeCam();
	_use_textures = _object->hasTextures();
}

void Core::loadMeshDiffuseTexture(const std::string& path)
{
	_mesh_diffuse_texture.reset();
	_mesh_diffuse_texture = TextureLoader::create2DTexture(path);
	_mesh_diffuse_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_mesh_diffuse_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_mesh_diffuse_texture->use();
}

void Core::loadMeshAOTexture(const std::string& path)
{
	_mesh_ao_texture.reset();
	_mesh_ao_texture = TextureLoader::create2DTexture(path);
	_mesh_ao_texture->use();
}

void Core::loadMeshNormalsTexture(const std::string& path)
{
	_mesh_normals_texture.reset();
	_mesh_normals_texture = TextureLoader::create2DTexture(path);
	_mesh_normals_texture->use();
}

void Core::computeLightMatrices()
{
	_light_view_matrix = glm::lookAt(_light->getPosition(), _object->getBBox().getCenter(), glm::vec3(0, 1, 0));
	_light_projection_matrix = glm::perspective(glm::radians(60.0f), float(_lineal_depth_texture->getWidth())/float(_lineal_depth_texture->getHeight()), 0.01f, _cam.getZfar());
}

void Core::unloadMesh()
{
	if (_object) _object.reset();
}

void Core::setDefaultFBO(GLuint fbo)
{
	RenderAlgorithms::default_buffer = fbo;
	_default_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(fbo, 1));
}

void Core::mouseclick(int x, int y, Input button)
{
	switch (button)
	{
	case Input::MOUSE_LEFT_BUTTON:
		_mouse_x = x;
		_mouse_y = y;
		break;
	case Input::MOUSE_RIGHT_BUTTON:
		_mouse_y = y;
		break;
	default:
		break;
	}
	std::cout << "click: " << _mouse_x << "  " << _mouse_y << std::endl;
	std::cout << "mouse press event" << std::endl;
}

void Core::mouseMoved(int x, int y, Input button)
{
	switch (button)
	{
	case Input::MOUSE_LEFT_BUTTON:
		
		_cam.rotate(-(y - _mouse_y)*ROT_SPEED, (x - _mouse_x)*ROT_SPEED);
		//_cam.update();
		_mouse_x = x;
		_mouse_y = y;
		break;
	case Input::MOUSE_RIGHT_BUTTON:
		_cam.zoom((y - _mouse_y) * ZOOM_SPEED);
		_mouse_y = y;
		break;
	default:
		break;
	}
	std::cout << "zoom: " << y - _mouse_y << "  " << x - _mouse_x << std::endl;
	std::cout << "mouse move event" << std::endl;
}

void Core::mouseReleased(int x, int y, Input button)
{
	switch (button)
	{
	default:
		break;
	}
	std::cout << "mpouse rel event" << std::endl;
}


void Core::toggleControlBool(unsigned int i)
{
	assert(i < _control_boolean_params.size());
	_control_boolean_params[i] = !_control_boolean_params[i];
}

void Core::setSSSMethod(int val)
{
	_sss_method = val;
		//case 0: //SSSS
		//case 1: //precomputed kernel
	if (_sss_method < 2)
	{
		RenderAlgorithms::setSeparableKernels(_sss_method);
		if (_sss_method == 0)_ssss_mod_factor = 0.7f;
		else if (_sss_method == 1) _ssss_mod_factor = 0.2f; //_ssss_mod_factor = 0.07f;
	}
	_num_frames = 0;
	_timmings = std::vector<double>(_timed_regions.size(), 0);
}

void Core::setToneMappingMethod(int val)
{
	_tone_mapping_method = val;
}

void Core::moveLight(glm::vec3 dir)
{
	_light->translate(LIGHT_MOV_SPEED*glm::normalize(dir));
	computeLightMatrices();
	std::cout << "light pos: " << glm::to_string(_light->getPosition()) << std::endl;
}

void Core::reloadShaders()
{
	std::shared_ptr<GlslShaderManager> shader_manager = GlslShaderManager::instance();
	shader_manager->reloadShaders();
	RenderAlgorithms::setSSSSKernels((_sss_method < 2 ? _sss_method : 0));
}


void Core::setTranslucency(float t)
{
	_translucency = t;
}

void Core::setSSWidth(float w)
{
	_sss_width = w * _initial_width;
	float factor = 0.9;
}

void Core::setExposure(float e)
{
	_exposure = e;
}

void Core::setAmbientInt(float a)
{
	_ambientInt = a;
}

void Core::setSpeculartInt(float s)
{
	_specInt = s;
}

void Core::setSSSRedStr(float s)
{
	_sss_strength.r = s;
	//Compute kernel
	RenderAlgorithms::computeKernels(_num_samples, _sss_strength, _falloff);
	RenderAlgorithms::setSSSSKernels((_sss_method < 2 ? _sss_method : 0));
}

void Core::setSSSGreenStr(float s)
{
	_sss_strength.g = s;
	//Compute kernel
	RenderAlgorithms::computeKernels(_num_samples, _sss_strength, _falloff);
	RenderAlgorithms::setSSSSKernels((_sss_method < 2 ? _sss_method : 0));
}

void Core::setSSSBlueStr(float s)
{
	_sss_strength.b = s;
	//Compute kernel
	RenderAlgorithms::computeKernels(_num_samples, _sss_strength, _falloff);
	RenderAlgorithms::setSSSSKernels((_sss_method < 2 ? _sss_method : 0));
}

void Core::setSSSRedFalloff(float s)
{
	_falloff[0] = s;
	//Compute kernel
	RenderAlgorithms::computeKernels(_num_samples, _sss_strength, _falloff);
	RenderAlgorithms::setSSSSKernels((_sss_method < 2 ? _sss_method : 0));
}

void Core::setSSSGreenFalloff(float s)
{
	_falloff[1] = s;
	//Compute kernel
	RenderAlgorithms::computeKernels(_num_samples, _sss_strength, _falloff);
	RenderAlgorithms::setSSSSKernels((_sss_method < 2 ? _sss_method : 0));
}

void Core::setSSSBlueFalloff(float s)
{
	_falloff[2] = s;
	//Compute kernel
	RenderAlgorithms::computeKernels(_num_samples, _sss_strength, _falloff);
	RenderAlgorithms::setSSSSKernels((_sss_method < 2 ? _sss_method : 0));
}

void Core::setSSSNumSamples(int s)
{
	_num_samples = s;
	RenderAlgorithms::computeKernels(_num_samples, _sss_strength, _falloff);
	RenderAlgorithms::setSSSSKernels((_sss_method < 2 ? _sss_method : 0));
}

void Core::setGlossines(float g)
{
	_glossines = g;
	_roughness = std::pow(ALPHA_MAX, g);
}

void Core::setBumpInt(float s)
{
	_bumpint = s;
}

void Core::saveCamtoFile(const std::string &path)
{
	std::ofstream file;
	file.open(path);
	glm::vec3 cam_pos = _cam.getPosition();
	file << cam_pos.x << " " << cam_pos.y << " " << cam_pos.z << " ";
	glm::vec3 cam_angles = _cam.getYawPitchRoll();
	file << cam_angles.x << " " << cam_angles.y << " " << cam_angles.z;
	file.close();
	std::cout << "Save: " << path << std::endl;
}

void Core::loadCamFromFile(const std::string &path)
{
	std::ifstream file;
	file.open(path);
	glm::vec3 cam_pos, cam_angles;
	file >> cam_pos.x >> cam_pos.y >> cam_pos.z;
	_cam.setPosition(cam_pos);
	_cam.zoom(0);
	file >> cam_angles.x >> cam_angles.y >> cam_angles.z;
	_cam.rotater(cam_angles.x, cam_angles.y, cam_angles.z);
	_cam.update();

	file.close();

	std::cout << "Load " << path << std::endl;
}

void Core::loadEnviroment(const std::string &path)
{
	auto sky_tex = TextureLoader::createCubeMap(path + "/skybox");
	_sky_box->setSkyTexture(sky_tex);
	_diffuse_env_texture = TextureLoader::createCubeMap(path + "/diffuse");
}

void Core::loadPreComputedKernel(const std::string &path)
{
	bool binary = false;
	if (path.compare(path.size() - 3, 3, ".bn") == 0)
		binary = true;
	std::ifstream in_file;
	std::ios_base::openmode om;

	if (binary) om = std::ios_base::in | std::ios_base::binary;
	else om = std::ios_base::in;

	in_file.open(path, om);
	assert(in_file.good());
	std::vector<float> kernel;

	if (binary)
	{
		// read float count
		char sv[4];
		in_file.read(sv, 4);
		int fc = (int)(floor(*((float*)sv)));
		kernel.resize(fc);
		in_file.read(reinterpret_cast<char*>(&kernel[0]), fc * 4);
	}
	else
	{
		float v;
		while (in_file >> v)
		{
			kernel.push_back(v);
			char next = in_file.peek();
			switch (next)
			{
				case ',': in_file.ignore(1); break;
				case ' ': in_file.ignore(1); break;
			}
		}
	}

	in_file.close();

	RenderAlgorithms::setPreComputedKernel(kernel);
	RenderAlgorithms::computeKernels(_num_samples, _sss_strength, _falloff);
	RenderAlgorithms::setSSSSKernels((_sss_method < 2 ? _sss_method : 0));
}