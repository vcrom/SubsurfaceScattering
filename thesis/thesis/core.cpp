#include "core.h"

#include <iostream>

#include "utils.h"
#include "meshimporter.h"
#include "renderalgorithms.h"


Core::Core()
{
	_mouse_x = _mouse_y = 0;
	_control_boolean_params = std::vector<bool>(10, false);
	_window_size = glm::vec2(0);
}

#include "fimage.h"
Core::~Core()
{
	//fImage image;
	//image.loadImage(_shadow_map_texture->getTextureData(), _shadow_map_texture->getWidth(), _shadow_map_texture->getHeight());
	//image.writeImage("textures/depth_map.jpg");
	_object.reset();
}

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
	checkCritOpenGLError();
}

void Core::initializeGL()
{
	glewInitialization();
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

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


#include <chrono>
#include <thread>
#include "bbox.h"
//GlslShaderManager shader_manager = GlslShaderManager::instance();

/// <summary>
/// Initializes core objects and openGL context
/// </summary>
void Core::initialize()
{
	initializeGL();
	//GlslShaderManager shader_manager = GlslShaderManager::instance();
	//shader_manager = GlslShaderManager::instance();
	std::shared_ptr<GlslShaderManager> shader_manager = GlslShaderManager::instance();
	shader_manager->initializeShaders();

	//TextureManager tex_man;
	//tex_man.loadTexture("textures/squares.jpg");
	//tex_man.loadTexture("textures/squares.png");
	//tex_man.loadTexture("textures/hdr_is.hdr");
	fImage image, image2;
	image.loadImage("textures/flower.jpg");

	_shadow_map_texture = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	_shadow_map_texture->use();
	_shadow_map_texture->loadEmptyTexture(GL_DEPTH_COMPONENT, 32, 32);

	//_background_texture = TextureLoader::Create2DTexture("textures/hills.jpg");
	_background_texture = TextureLoader::Create2DTexture("textures/flower.jpg");
	_background_texture->use();

	//tex = TextureLoader::Create2DTexture("textures/flower.jpg");
	//tex->use(GL_TEXTURE0);
	//checkCritOpenGLError();

	//tex_col = new Texture2D(GL_TEXTURE_2D);
	//tex_col->createTexture();
	//tex_col->use(GL_TEXTURE1);
	//tex_col->loadEmptyTexture(GL_RGBA32F, 32, 32);
	//_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer());
	//_buffer->createFrameBuffer();

	//_buffer->useFrameBuffer();
	//_buffer->colorBuffer(tex_col->getTextureID(), 0);
	//if (_buffer->checkStatus()) std::cout << "Buffer init" << std::endl;
	//else std::cout << "Buffer NO init" << std::endl;
	//checkCritOpenGLError();

	//Init default buffer
	_default_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(RenderAlgorithms::default_buffer, 1));

	_generic_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer());
	_generic_buffer->createFrameBuffer();

	_cam.updateProjection(glm::radians(45.0f), 1.0f);
	//loadMesh("meshes/bunny.ply");
	//loadMesh("meshes/tests.ply");
	//loadMesh("C:/Users/crv/Source/Repos/Master thesis/thesis/thesis/meshes/sphere.ply");

	//_cam.updateProjection(glm::radians(45.0f), 1.0f);
	//_object = std::shared_ptr<Entity> (new Entity(MeshImporter::importMeshFromFile("meshes/tests.ply")));
	//_object->setUnitary();
	//std::cout << "Mesh box: " << _object->getMeshBBox() << std::endl;
	//std::cout << "Obj box: " << _object->getBBox() << std::endl;
	//initializeCam();
	_sphere = MeshImporter::importMeshFromFile("meshes/sphere.ply");
	_light = std::shared_ptr<Entity>(new Entity(_sphere));
	_light->setUnitary();

	loadMesh("meshes/tests.ply");
	_light->scale(glm::vec3(0.19));
	_light->setPosition(glm::vec3(0.5, 1, 0.5));
}

void Core::initializeCam()
{
	if (_object) _cam.initFromBBox(_object.get()->getBBox());
	_cam.update();
}


void Core::resizeTextures(unsigned int w, unsigned int h)
{
	_shadow_map_texture->use();
	_shadow_map_texture->resize(w, h);
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
void Core::render()
{
	_default_buffer->useFrameBuffer();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	RenderAlgorithms::renderTexture(_default_buffer, _background_texture);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (!_object) return;
	//else std::cout << "Obj box: " << _object->getBBox() << std::endl;

	//RenderAlgorithms::renderTexture(*buffer, *tex);
	//RenderAlgorithms::renderTexture(*_default_buffer, *tex_col);
	//glClear(GL_DEPTH_BUFFER_BIT);
	if (!_control_boolean_params[0])
	{

		_generic_buffer->useFrameBuffer();
		_generic_buffer->depthBuffer(_shadow_map_texture->getTextureID());
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 V_L = glm::lookAt(_light->getPosition(), _object->getBBox().getCenter(), glm::vec3(0, 1, 0));
		glm::mat4 P_L = glm::perspective(glm::radians(60.0f), _cam.getAspectRatio(), _cam.getZnear(), _cam.getZfar());
		RenderAlgorithms::getShadowMap(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L, _window_size, glm::vec2(_shadow_map_texture->getWidth(), _shadow_map_texture->getHeight()));


		RenderAlgorithms::renderDiffuseAndShadows(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), _shadow_map_texture, V_L, P_L, _light->getPosition());
		//RenderAlgorithms::renderMesh(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix());
		//RenderAlgorithms::renderMesh(_default_buffer, _light->getMeshPtr(), _light->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix(), glm::vec3(1, 0, 0));
	}

	else
	{
		glm::mat4 V_L = glm::lookAt(_light->getPosition(), _object->getBBox().getCenter(), glm::vec3(0, 1, 0));
		glm::mat4 P_L = glm::perspective(glm::radians(60.0f), _cam.getAspectRatio(), _cam.getZnear(), _cam.getZfar());

		_generic_buffer->useFrameBuffer();
		_generic_buffer->depthBuffer(_shadow_map_texture->getTextureID());
		RenderAlgorithms::getShadowMap(_generic_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L, _window_size, glm::vec2(_shadow_map_texture->getWidth(), _shadow_map_texture->getHeight()));

		//RenderAlgorithms::renderMesh(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L);
		RenderAlgorithms::renderDiffuseAndShadows(_default_buffer, _object->getMeshPtr(), _object->getTransformations(), V_L, P_L, _shadow_map_texture, V_L, P_L, _light->getPosition());

	}


	std::cout << glm::to_string(_object->getBBox().getCenter()) << std::endl;
	std::cout << glm::to_string(_light->getBBox().getCenter()) << std::endl;
}

/// <summary>
/// Loads the mesh from path. In order to efectivelly load the mesh, teh openGl context must be active
/// </summary>
/// <param name="path">The path of teh mesh.</param>
void Core::loadMesh(const std::string& path)
{
	unloadMesh();
	_object = std::shared_ptr<Entity>(new Entity(MeshImporter::importMeshFromFile(path)));
	_object->setUnitary();
	initializeCam();
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