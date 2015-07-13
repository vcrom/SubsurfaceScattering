#include "core.h"

#include <iostream>
#include <chrono>
//#include <thread>

#include "utils.h"
#include "meshimporter.h"
#include "renderalgorithms.h"


Core::Core()
{
	_mouse_x = _mouse_y = 0;

	_window_size = glm::vec2(0);
	_prev_VP = glm::mat4(0);
	_sss_width = 0.005;//0.0117500005;
	_translucency = 0.95;
	_correction = 1700;
	_sssStrength =  12.75;
	_pixel_size = glm::vec2(1);
	_exposure = 2;
	_ambientInt = 0.66;
	_specInt = 0.2;
	_burnout = std::numeric_limits<float>::infinity();


	_control_boolean_params = std::vector<bool>(10, false);
	_control_boolean_params[2] = true;
	_sss_method = 0;
	_tone_mapping_method = 0;

	_num_samples = 25;
	_sss_strength = glm::vec3(0.48, 0.41, 0.28);
	_falloff = {1.0f, 0.37f, 0.3f};

}

#include "fimage.h"
Core::~Core()
{
	fImage image;
	//image.loadImage(_shadow_map_texture->getTextureData(), _shadow_map_texture->getWidth(), _shadow_map_texture->getHeight());
	//image.writeImage("textures/depth_map.jpg");
	_object.reset();
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


	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);
	//GLuint textureID;
	//glGenTextures(1, &textureID);
	//glBindTexture(GL_TEXTURE_2D, textureID);
	//glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT);

	//std::cout << "OpenGL No init" << std::endl;
	//checkCritOpenGLError();


	std::cout << "OpenGL init" << std::endl;
}


/// <summary>
/// Initializes the textures.
/// </summary>
void Core::initializeTextures()
{
	//Shadow map texture
	_shadow_map_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_shadow_map_texture->use();
	_shadow_map_texture->loadEmptyTexture(GL_DEPTH_COMPONENT/*GL_DEPTH_COMPONENT32F*/, 32, 32);
	_shadow_map_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	_shadow_map_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
	_lineal_shadow_map_texture->loadEmptyTexture(GL_R32F, 32, 32);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_BORDER_COLOR, border);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	_lineal_shadow_map_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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

	//lineal shadow map
	_cross_bilateral_factor = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_cross_bilateral_factor->use();
	_cross_bilateral_factor->loadEmptyTexture(GL_R32F, 32, 32);
	_cross_bilateral_factor->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_cross_bilateral_factor->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_cross_bilateral_factor->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_cross_bilateral_factor->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	checkCritOpenGLError();

	//_background_texture = TextureLoader::Create2DTexture("textures/hills.jpg");bokeh.jpg
	//_background_texture = TextureLoader::Create2DTexture("textures/flower.jpg");
	_background_texture = TextureLoader::Create2DTexture("textures/bokeh.jpg");

	//loadMeshDiffuseTexture("textures/flower.jpg"); 
	loadMeshDiffuseTexture("textures/tests.png"); 
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

	std::shared_ptr<GlslShaderManager> shader_manager = GlslShaderManager::instance();
	shader_manager->initializeShaders();


	//Init default buffer
	_default_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(RenderAlgorithms::default_buffer, 1));

	_generic_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer());
	_generic_buffer->createFrameBuffer();

	_cam.updateProjection(glm::radians(60.0f), 1.0f);

	_sphere = MeshImporter::importMeshFromFile("meshes/sphere.ply");
	_light = std::shared_ptr<Entity>(new Entity(_sphere));
	_light->setUnitary();

	loadMesh("meshes/tests.ply");
	_light->scale(glm::vec3(0.19));
	_light->setPosition(glm::vec3(2)*glm::vec3(0.5, 1, 0.5));
	moveLight(glm::vec3(0.00001, 0, 0));
	computeLightMatrices();

	//Compute kernel
	RenderAlgorithms::computeSeparableKernel(_num_samples, _sss_strength, _falloff);

}

void Core::initializeCam()
{
	if (_object) _cam.initFromBBox(_object.get()->getBBox());
	_cam.update();
}


void Core::resizeTextures(unsigned int w, unsigned int h)
{
	unsigned int shadow_width, shadow_height;
	//shadow_width = 2048; //w
	//shadow_height = 2048; //h
	shadow_width = w;
	shadow_height = h;

	_shadow_map_texture->use(); 
	_shadow_map_texture->resize(shadow_width, shadow_height);

	_lineal_shadow_map_texture->use();
	_lineal_shadow_map_texture->resize(shadow_width, shadow_height);

	_depth_stencil_texture->use();
	_depth_stencil_texture->resize(shadow_width, shadow_height);

	_aux_ssss_texture1->use();
	_aux_ssss_texture1->resize(w, h);

	_aux_ssss_texture2->use();
	_aux_ssss_texture2->resize(w, h);

	_diffuse_color_texture->use();
	_diffuse_color_texture->resize(w, h);

	_specular_texture->use();
	_specular_texture->resize(w, h);

	_lineal_depth_texture->use();
	_lineal_depth_texture->resize(w, h);

	_aux_ssss_pingpong->use();
	_aux_ssss_pingpong->resize(w, h);

	_cross_bilateral_factor->use();
	_cross_bilateral_factor->resize(w, h);
}

void Core::resize(unsigned int w, unsigned int h)
{
	std::cout << "Resize(" << w << ", " << h << ")" << std::endl;
	_window_size = glm::vec2(w, h);
	_pixel_size = glm::vec2(1.0 / float(w), 1.0 / float(h));
	glViewport(0, 0, w, h);
	_cam.updateProjection(glm::radians(60.0f), float(w) / float(h));
	_cam.update();
	resizeTextures(w, h);
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
		//_default_buffer->useFrameBuffer();
		//_default_buffer->clearColorAndDepth();
		//RenderAlgorithms::renderTexture(_default_buffer, _cross_bilateral_factor);
		return;
	}
	_default_buffer->useFrameBuffer();
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	_default_buffer->clearColorAndDepth();
	
	RenderAlgorithms::renderMesh(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _light_view_matrix, _light_projection_matrix, glm::vec3(1, 0, 0));
}

//using time_unit = std::chrono::milliseconds;
//using time_unit = std::chrono::microseconds;
using time_unit = std::chrono::nanoseconds;


void Core::renderScene()
{
	_default_buffer->useFrameBuffer();
	_default_buffer->clearColorAndDepth();
	std::cout << "Rendering scene..." << std::endl;

	_t1 = _clock.now();
	shadowMapPass();
	glFinish();
	_t2 = _clock.now();
	std::cout << "\tShadow mapping time: " << std::chrono::duration_cast<time_unit>(_t2 - _t1).count() << std::endl;

	_t1 = _clock.now();
	mainRenderPass();
	glFinish();
	_t2 = _clock.now();
	std::cout << "\tMain pas time: " << std::chrono::duration_cast<time_unit>(_t2 - _t1).count() << std::endl;

	_t1 = _clock.now();
	if(_control_boolean_params[2]) subSurfaceScatteringPass();
	glFinish();
	_t2 = _clock.now();
	std::cout << "\tSubsurface scattering pas time: " << std::chrono::duration_cast<std::chrono::milliseconds>(_t2 - _t1).count() << std::endl;
	//RenderAlgorithms::renderMesh(_default_buffer, _light->getMeshPtr(), _light->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), glm::vec3(1, 0, 0));

	_t1 = _clock.now();
	addSpecularPass();
	glFinish();
	_t2 = _clock.now();
	std::cout << "\tAdd Specular pas time: " << std::chrono::duration_cast<std::chrono::milliseconds>(_t2 - _t1).count() << std::endl;

	_t1 = _clock.now();
	toneMap();
	glFinish();
	_t2 = _clock.now();
	std::cout << "\tAdd Tone map pas time: " << std::chrono::duration_cast<std::chrono::milliseconds>(_t2 - _t1).count() << std::endl;
	
	
	//RenderAlgorithms::renderMesh(_default_buffer, _light->getMeshPtr(), _light->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), glm::vec3(1, 0, 0));

}

/// <summary>
/// Render pas which renders the scene from the light point of view and gets it's shadowmaps
/// </summary>
void Core::shadowMapPass()
{
	_generic_buffer->useFrameBuffer();
	_generic_buffer->colorBuffer(_lineal_shadow_map_texture->getTextureID(), 0);
	_generic_buffer->depthBuffer(_shadow_map_texture->getTextureID());
	_generic_buffer->stencilBuffer(0);
	//_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());
	_generic_buffer->clearColorAndDepth();
	RenderAlgorithms::getLinealShadowMap(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), _light_view_matrix, _light_projection_matrix, _cam.getZfar(), _window_size, glm::vec2(_lineal_shadow_map_texture->getWidth(), _lineal_shadow_map_texture->getHeight()), _light->getPosition());
}

/// <summary>
/// Mains the render pass, which renders the scene(head)
/// </summary>
void Core::mainRenderPass()
{

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);

	_default_buffer->useFrameBuffer();
	_default_buffer->clearColorDepthAndStencil();

	_generic_buffer->useFrameBuffer(4);
	_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);//difuse
	_generic_buffer->colorBuffer(_lineal_depth_texture->getTextureID(), 1);//lin depth
	_generic_buffer->colorBuffer(_specular_texture->getTextureID(), 2);//specular
	_generic_buffer->colorBuffer(_cross_bilateral_factor->getTextureID(), 3);//cross-bilateral
	_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());
	_generic_buffer->clearColorDepthAndStencil();

	//render background
	//RenderAlgorithms::renderTexture(_generic_buffer, _background_texture);

	checkCritOpenGLError();

	RenderAlgorithms::renderDiffuseAndSpecular(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), _prev_VP, _cam.getPosition(), _cam.getZfar(), _light->getPosition(), 
		_shadow_map_texture, _light_view_matrix, _light_projection_matrix, _lineal_shadow_map_texture, _cam.getZfar(), 
		_sss_width, _translucency, _ambientInt, _specInt, _control_boolean_params[2], 
		_mesh_diffuse_texture, true);

	_prev_VP = _cam.getProjectionMatrix()*_cam.getViewMatrix();
	_generic_buffer->useFrameBuffer(3);
	_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 2);//specular
	////render background
	//_generic_buffer->useFrameBuffer(1);
	//glStencilFunc(GL_EQUAL, 0, 0xFF);
	//glStencilMask(0x00);
	//RenderAlgorithms::renderTexture(_generic_buffer, _background_texture);
	glDisable(GL_STENCIL_TEST);
}

void Core::subSurfaceScatteringPass()
{
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);
	switch (_sss_method)
	{
	case 0: //SSSS
		_generic_buffer->useFrameBuffer(3);
		_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 0);
		_generic_buffer->colorBuffer(_aux_ssss_texture2->getTextureID(), 1);
		_generic_buffer->colorBuffer(_aux_ssss_pingpong->getTextureID(), 2);
		_generic_buffer->clearColor();
		_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());

		RenderAlgorithms::separableSSSSEffect(_generic_buffer, _diffuse_color_texture, _aux_ssss_texture1, _lineal_depth_texture, _cam.getFOV(), _sss_width, _cross_bilateral_factor);
		_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);

		//RenderAlgorithms::renderTexture(_generic_buffer, _background_texture);

		break;
	case 1: //Perceptual
		_generic_buffer->useFrameBuffer(3);
		_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 0);
		_generic_buffer->colorBuffer(_aux_ssss_texture2->getTextureID(), 1);
		_generic_buffer->colorBuffer(_aux_ssss_pingpong->getTextureID(), 2);
		_generic_buffer->clearColor();

		RenderAlgorithms::SSSEffect(_generic_buffer, _diffuse_color_texture, _aux_ssss_pingpong, _aux_ssss_texture1, _aux_ssss_texture2, _lineal_depth_texture, _pixel_size, _correction, _sssStrength, _cross_bilateral_factor);
		_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);

		//RenderAlgorithms::renderTexture(_generic_buffer, _background_texture);
		//RenderAlgorithms::renderTexture(_generic_buffer, _specular_texture);
		//_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);
		//RenderAlgorithms::renderTexture(_generic_buffer, _background_texture);
		break;
	default:
		break;
	}
	glDisable(GL_STENCIL_TEST);
	//if (_sss_method)
	//{
	//	_generic_buffer->useFrameBuffer(1);
	//	_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 0); 
	//	_generic_buffer->colorBuffer(_aux_ssss_texture2->getTextureID(), 1);
	//	_generic_buffer->colorBuffer(_aux_ssss_pingpong->getTextureID(), 2);
	//	_generic_buffer->clearColor();
	//	glEnable(GL_STENCIL_TEST);
	//	glStencilFunc(GL_EQUAL, 1, 0xFF);
	//	glStencilMask(0x00);
	//	RenderAlgorithms::SSSEffect(_generic_buffer, _diffuse_color_texture, _aux_ssss_pingpong, _aux_ssss_texture1, _aux_ssss_texture2, _lineal_depth_texture, _pixel_size, _correction, _sssStrength);
	//	//_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);
	//	//RenderAlgorithms::renderTexture(_generic_buffer, _background_texture);
	//	glDisable(GL_STENCIL_TEST);
	//}

	//else
	//{
	//	_generic_buffer->useFrameBuffer(1);
	//	_generic_buffer->colorBuffer(_aux_ssss_texture1->getTextureID(), 0);//diffuse
	//	_generic_buffer->clearColor();
	//	glEnable(GL_STENCIL_TEST);
	//	glStencilFunc(GL_EQUAL, 1, 0xFF);
	//	glStencilMask(0x00);

	//	RenderAlgorithms::separableSSSSEffect(_generic_buffer, _diffuse_color_texture, _aux_ssss_texture1, _lineal_depth_texture, _cam.getFOV(), _sss_width);

	//	glDisable(GL_STENCIL_TEST);
	//}
}

void Core::addSpecularPass()
{
	//RenderAlgorithms::renderTexture(_default_buffer, _diffuse_color_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	RenderAlgorithms::renderTexture(_generic_buffer, _specular_texture);
	glDisable(GL_BLEND);

	//RenderAlgorithms::renderTexture(_default_buffer, _diffuse_color_texture);
}


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
	RenderAlgorithms::renderTexture(_generic_buffer, _background_texture);
	glDisable(GL_STENCIL_TEST);


	RenderAlgorithms::renderTexture(_default_buffer, _aux_ssss_texture1);

	//RenderAlgorithms::renderTexture(_default_buffer, _diffuse_color_texture);
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
	initializeCam();
}

void Core::loadMeshDiffuseTexture(const std::string& path)
{
	_mesh_diffuse_texture.reset();
	_mesh_diffuse_texture = TextureLoader::Create2DTexture(path);
	_mesh_diffuse_texture->use();
}

void Core::loadMeshAOTexture(const std::string& path)
{
	_mesh_ao_texture.reset();
	_mesh_ao_texture = TextureLoader::Create2DTexture(path);
	_mesh_ao_texture->use();
}

void Core::computeLightMatrices()
{
	_light_view_matrix = glm::lookAt(_light->getPosition(), _object->getBBox().getCenter(), glm::vec3(0, 1, 0));
	_light_projection_matrix = glm::perspective(glm::radians(60.0f), _cam.getAspectRatio(), _cam.getZnear(), _cam.getZfar());
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
}

void Core::setToneMappingMethod(int val)
{
	_tone_mapping_method = val;
}

//void Core::setControlInt(unsigned int i, int val)
//{
//	assert(i < _control_int_params.size());
//	_control_int_params[i] = val;
//}
//
//void Core::incrControlInt(unsigned int i)
//{
//	assert(i < _control_int_params.size());
//	++_control_int_params[i];
//}
//
//void Core::decrControlInt(unsigned int i)
//{
//	assert(i < _control_int_params.size());
//	--_control_int_params[i];
//}

void Core::moveLight(glm::vec3 dir)
{
	_light->translate(LIGHT_MOV_SPEED*glm::normalize(dir));
	computeLightMatrices();
}

void Core::reloadShaders()
{
	std::shared_ptr<GlslShaderManager> shader_manager = GlslShaderManager::instance();
	shader_manager->reloadShaders();
	RenderAlgorithms::setSeparableKernels();
}


void Core::setTranslucency(float t)
{
	_translucency = t;
}

void Core::setSSWidth(float w)
{
	_sss_width = w;
	float factor = 12.75 / 0.005;
	_sssStrength = factor*_sss_width;
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
	RenderAlgorithms::computeSeparableKernel(_num_samples, _sss_strength, _falloff);
}

void Core::setSSSGreenStr(float s)
{
	_sss_strength.g = s;
	//Compute kernel
	RenderAlgorithms::computeSeparableKernel(_num_samples, _sss_strength, _falloff);
}

void Core::setSSSBlueStr(float s)
{
	_sss_strength.b = s;
	//Compute kernel
	RenderAlgorithms::computeSeparableKernel(_num_samples, _sss_strength, _falloff);
}

void Core::setSSSNumSamples(int s)
{
	_num_samples = s;
	RenderAlgorithms::computeSeparableKernel(_num_samples, _sss_strength, _falloff);
}