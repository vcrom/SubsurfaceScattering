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
	_control_boolean_params = std::vector<bool>(10, false);
	_window_size = glm::vec2(0);
	_prev_VP = glm::mat4(0);
	sss_width = 0.012;
	translucency = 0.95;
}

#include "fimage.h"
Core::~Core()
{
	fImage image;
	image.loadImage(_shadow_map_texture->getTextureData(), _shadow_map_texture->getWidth(), _shadow_map_texture->getHeight());
	image.writeImage("textures/depth_map.jpg");
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearStencil(0);


	glEnable(GL_DEPTH_TEST);
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
	_diffuse_color_texture->loadEmptyTexture(GL_RGBA, 32, 32);
	_diffuse_color_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_diffuse_color_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_diffuse_color_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_diffuse_color_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	_specular_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_specular_texture->use();
	_specular_texture->loadEmptyTexture(GL_RGB, 32, 32);
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

	_aux_ssss_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_aux_ssss_texture->use();
	_aux_ssss_texture->loadEmptyTexture(GL_RGBA, 32, 32);
	_aux_ssss_texture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_aux_ssss_texture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_aux_ssss_texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_aux_ssss_texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, window_size.x, window_size.y, 0, GL_RED, GL_FLOAT, NULL);
	checkCritOpenGLError();

	//_background_texture = TextureLoader::Create2DTexture("textures/hills.jpg");
	_background_texture = TextureLoader::Create2DTexture("textures/flower.jpg");
	_background_texture->use();
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

	_cam.updateProjection(glm::radians(45.0f), 1.0f);

	_sphere = MeshImporter::importMeshFromFile("meshes/sphere.ply");
	_light = std::shared_ptr<Entity>(new Entity(_sphere));
	_light->setUnitary();

	loadMesh("meshes/tests.ply");
	_light->scale(glm::vec3(0.19));
	_light->setPosition(glm::vec3(0.5, 1, 0.5));
	computeLightMatrices();
}

void Core::reloadShaders()
{
	std::shared_ptr<GlslShaderManager> shader_manager = GlslShaderManager::instance();
	shader_manager->reloadShaders();
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

	_aux_ssss_texture->use();
	_aux_ssss_texture->resize(w, h);
	_diffuse_color_texture->use();
	_diffuse_color_texture->resize(w, h);
	_specular_texture->use();
	_specular_texture->resize(w, h);
	_lineal_depth_texture->use();
	_lineal_depth_texture->resize(w, h);



}

void Core::resize(unsigned int w, unsigned int h)
{
	std::cout << "Resize(" << w << ", " << h << ")" << std::endl;
	_window_size = glm::vec2(w, h);
	glViewport(0, 0, w, h);
	_cam.updateProjection(glm::radians(45.0f), float(w) / float(h));
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
		return;
	}
	_default_buffer->useFrameBuffer();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	


	RenderAlgorithms::renderMesh(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _light_view_matrix, _light_projection_matrix, glm::vec3(1, 0, 0));


	//return;
	//RenderAlgorithms::renderTexture(_default_buffer, _background_texture);


	//if (!_object) return;

	////else std::cout << "Obj box: " << _object->getBBox() << std::endl;

	////RenderAlgorithms::renderTexture(*buffer, *tex);
	////RenderAlgorithms::renderTexture(*_default_buffer, *tex_col);
	////glClear(GL_DEPTH_BUFFER_BIT);
	//if (!_control_boolean_params[0])
	//{

	//	_generic_buffer->useFrameBuffer();
	//	_generic_buffer->colorBuffer(_lineal_shadow_map_texture->getTextureID(), 0);
	//	_generic_buffer->depthBuffer(_shadow_map_texture->getTextureID());
	//	_generic_buffer->clearColorAndDepth();

	//	glm::mat4 V_L = glm::lookAt(_light->getPosition(), _object->getBBox().getCenter(), glm::vec3(0, 1, 0));
	//	glm::mat4 P_L = glm::perspective(glm::radians(60.0f), _cam.getAspectRatio(), _cam.getZnear(), _cam.getZfar());
	//	//RenderAlgorithms::getShadowMap(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L, _window_size, glm::vec2(_shadow_map_texture->getWidth(), _shadow_map_texture->getHeight()));
	//	RenderAlgorithms::getLinealShadowMap(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L, _cam.getZfar(), _window_size, glm::vec2(_shadow_map_texture->getWidth(), _shadow_map_texture->getHeight()), _light->getPosition());
	//	RenderAlgorithms::renderDiffuseAndShadows(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), _shadow_map_texture, V_L, P_L, _light->getPosition());
	//	
	//	
	//	
	//	
	//	
	//	//RenderAlgorithms::renderMesh(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix());
	//	RenderAlgorithms::renderMesh(_default_buffer, _light->getMeshPtr(), _light->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), glm::vec3(1, 0, 0));
	//}

	//else
	//{
	//	glm::mat4 V_L = glm::lookAt(_light->getPosition(), _object->getBBox().getCenter(), glm::vec3(0, 1, 0));
	//	glm::mat4 P_L = glm::perspective(glm::radians(60.0f), _cam.getAspectRatio(), _cam.getZnear(), _cam.getZfar());

	//	//_generic_buffer->useFrameBuffer();
	//	//_generic_buffer->depthBuffer(_shadow_map_texture->getTextureID());
	//	//glClear(GL_DEPTH_BUFFER_BIT);

	//	//RenderAlgorithms::getShadowMap(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L, _window_size, glm::vec2(_shadow_map_texture->getWidth(), _shadow_map_texture->getHeight()));

	//	////RenderAlgorithms::renderMesh(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L);
	//	//RenderAlgorithms::renderDiffuseAndShadows(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L, _shadow_map_texture, V_L, P_L, _light->getPosition());
	//	
	//	//_default_buffer->useFrameBuffer();
	//	//_default_buffer->clearDepthAndColor();
	//	//RenderAlgorithms::getLinealShadowMap(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L, _cam.getZfar(), _window_size, _window_size);// glm::vec2(_shadow_map_texture->getWidth(), _shadow_map_texture->getHeight()));

	//	//RenderAlgorithms::renderThickness(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L, _cam.getZfar())//, const glm::vec2 &viewport_size, const glm::vec2 &shadow_buffer_size)

	//	//RenderAlgorithms::renderTexture(_default_buffer, _lineal_shadow_map_texture);
	//	RenderAlgorithms::translucency(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), _lineal_shadow_map_texture, _cam.getZfar(), V_L, P_L, _light->getPosition());


	//}


	//std::cout << glm::to_string(_object->getBBox().getCenter()) << std::endl;
	//std::cout << glm::to_string(_light->getBBox().getCenter()) << std::endl;
}

using time_unit = std::chrono::milliseconds;
//using time_unit = std::chrono::microseconds;
//using time_unit = std::chrono::nanoseconds;


void Core::renderScene()
{
	_default_buffer->useFrameBuffer();
	_default_buffer->clearColorAndDepth();
	std::cout << "Rendering scene..." << std::endl;
	_t1 = _clock.now();
	RenderAlgorithms::renderTexture(_default_buffer, _background_texture);
	_t2 = _clock.now();
	std::cout << "\tRender background time: " << std::chrono::duration_cast<time_unit>(_t2 - _t1).count() << std::endl;

	_t1 = _clock.now();
	shadowMapPass();
	_t2 = _clock.now();
	std::cout << "\tShadow mapping time: " << std::chrono::duration_cast<time_unit>(_t2 - _t1).count() << std::endl;

	_t1 = _clock.now();
	mainRenderPass();
	_t2 = _clock.now();
	std::cout << "\tMain pas time: " << std::chrono::duration_cast<time_unit>(_t2 - _t1).count() << std::endl;

	_t1 = _clock.now();
	subSurfaceScatteringPass();
	_t2 = _clock.now();
	std::cout << "\tSubsurface scattering pas time: " << std::chrono::duration_cast<std::chrono::milliseconds>(_t1 - _t2).count() << std::endl;
	//RenderAlgorithms::renderMesh(_default_buffer, _light->getMeshPtr(), _light->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), glm::vec3(1, 0, 0));

	_t1 = _clock.now();
	addSpecularPass();
	_t2 = _clock.now();
	std::cout << "\tAdd Specular pas time: " << std::chrono::duration_cast<std::chrono::milliseconds>(_t1 - _t2).count() << std::endl;
	
	
	//RenderAlgorithms::renderMesh(_default_buffer, _light->getMeshPtr(), _light->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), glm::vec3(1, 0, 0));

}

/// <summary>
/// Render pas which renders the scene from the light point of view and gets it's shadowmaps
/// </summary>
void Core::shadowMapPass()
{
	_generic_buffer->useFrameBuffer(3);
	_generic_buffer->colorBuffer(_lineal_shadow_map_texture->getTextureID(), 0);
	_generic_buffer->stencilBuffer(0);
	_generic_buffer->depthBuffer(_shadow_map_texture->getTextureID());
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
	//_default_buffer->clearStencil();
	_default_buffer->clearColorDepthAndStencil();

	_generic_buffer->useFrameBuffer(3);
	_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);//difuse
	_generic_buffer->colorBuffer(_lineal_depth_texture->getTextureID(), 1);//lin depth
	_generic_buffer->colorBuffer(_specular_texture->getTextureID(), 2);//specular
	_generic_buffer->depthAndStencilBuffer(_depth_stencil_texture->getTextureID());
	_generic_buffer->clearColorDepthAndStencil();
	checkCritOpenGLError();

	//_generic_buffer->clearColorDepthAndStencil();
	//_default_buffer->useFrameBuffer();

	//RenderAlgorithms::renderShadows(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), _shadow_map_texture, _light_view_matrix, _light_projection_matrix, _light->getPosition());

	RenderAlgorithms::renderDiffuseAndSpecular(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), _prev_VP, _cam.getPosition(), _cam.getZfar(), _light->getPosition(), _shadow_map_texture,
		_light_view_matrix, _light_projection_matrix, _lineal_shadow_map_texture, _cam.getZfar(), sss_width, translucency, true);

	_prev_VP = _cam.getProjectionMatrix()*_cam.getViewMatrix();

	RenderAlgorithms::renderTexture(_default_buffer, _diffuse_color_texture);

	//glStencilFunc(GL_EQUAL, 0, 0xFF);
	//glStencilMask(0xFF);
	//RenderAlgorithms::renderTexture(_default_buffer, _background_texture);
	glDisable(GL_STENCIL_TEST);

	//_default_buffer.use_count();
	//_default_buffer->clearColorDepthAndStencil();
	//RenderAlgorithms::getLinealShadowMap(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _light_view_matrix, _light_projection_matrix, _cam.getZfar(), _window_size, glm::vec2(_shadow_map_texture->getWidth(), _shadow_map_texture->getHeight()));

}

void Core::subSurfaceScatteringPass()
{
	if (!_control_boolean_params[1])
	{

	}

	else
	{


		_generic_buffer->useFrameBuffer(1);
		_generic_buffer->colorBuffer(_aux_ssss_texture->getTextureID(), 0);//diffuse
		_generic_buffer->clearColor();
		//RenderAlgorithms::renderTexture(_generic_buffer, _background_texture);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilMask(0xFF);

		RenderAlgorithms::renderTexture(_generic_buffer, _diffuse_color_texture);

		_generic_buffer->colorBuffer(_diffuse_color_texture->getTextureID(), 0);//diffuse
		_generic_buffer->clearColor();

		RenderAlgorithms::renderTexture(_generic_buffer, _aux_ssss_texture);

		glDisable(GL_STENCIL_TEST);
	}
}

void Core::addSpecularPass()
{
	RenderAlgorithms::renderTexture(_default_buffer, _diffuse_color_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	RenderAlgorithms::renderTexture(_default_buffer, _specular_texture);
	glDisable(GL_BLEND);

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

void Core::computeLightMatrices()
{
	_light_view_matrix = glm::lookAt(_light->getPosition(), _object->getBBox().getCenter(), glm::vec3(0, 1, 0));
	_light_projection_matrix = glm::perspective(glm::radians(45.0f), _cam.getAspectRatio(), _cam.getZnear(), _cam.getZfar());
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

void Core::moveLight(glm::vec3 dir)
{
	_light->translate(LIGHT_MOV_SPEED*glm::normalize(dir));
	computeLightMatrices();
}