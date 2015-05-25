#include "core.h"

#include <iostream>

#include "utils.h"
#include "meshimporter.h"
#include "renderalgorithms.h"


Core::Core()
{
	_mouse_x = _mouse_y = 0;
	_object = nullptr;
	_load_mesh_event = false;
}

Core::~Core()
{
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

#include "fimage.h"
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
	GlslShaderManager *shader_manager = GlslShaderManager::instance();
	shader_manager->initializeShaders();

	//TextureManager tex_man;
	//tex_man.loadTexture("textures/squares.jpg");
	//tex_man.loadTexture("textures/squares.png");
	//tex_man.loadTexture("textures/hdr_is.hdr");
	fImage image, image2;
	image.loadImage("textures/flower.jpg");

	tex = TextureLoader::Create2DTexture("textures/flower.jpg");
	tex->use(GL_TEXTURE0);
	checkCritOpenGLError();

	tex_col = new Texture2D(GL_TEXTURE_2D);
	tex_col->createTexture();
	tex_col->use(GL_TEXTURE1);
	tex_col->loadEmptyTexture(GL_RGBA32F, 32, 32);
	_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer());
	_buffer->createFrameBuffer();

	_buffer->useFrameBuffer();
	_buffer->colorBuffer(tex_col->getTextureID(), 0);
	if (_buffer->checkStatus()) std::cout << "Buffer init" << std::endl;
	else std::cout << "Buffer NO init" << std::endl;
	checkCritOpenGLError();

	_qt_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(RenderAlgorithms::default_buffer, 1));

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
}

void Core::initializeCam()
{
	if (_object) _cam.initFromBBox(_object.get()->getBBox());
	_cam.update();
}

void Core::resize(unsigned int w, unsigned int h)
{
	std::cout << "Resize(" << w << ", " << h << ")" << std::endl;
	glViewport(0, 0, w, h);
	_cam.updateProjection(glm::radians(45.0f), float(w) / float(h));
	_cam.update();
	tex_col->use(GL_TEXTURE1);
	tex_col->resize(w, h);
	checkCritOpenGLError();
}

#include "screenquad.h"
void Core::render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!_object) return;
	//else std::cout << "Obj box: " << _object->getBBox() << std::endl;

	//RenderAlgorithms::renderTexture(*buffer, *tex);
	//RenderAlgorithms::renderTexture(*_qt_buffer, *tex_col);
	//glClear(GL_DEPTH_BUFFER_BIT);
	RenderAlgorithms::renderMesh(_qt_buffer, _object->getMeshPtr(), _object->getTransformations(), _cam.getViewMatrix(), _cam.getProjectionMatrix());
}

//void Core::processMeshLoadingEvents()
//{
//	if (_load_mesh_event)
//	{
//		loadMesh(_path_to_load);
//		_load_mesh_event = false;
//	}
//}
//
//void Core::queueToLoadMesh(const std::string& path)
//{
//	_load_mesh_event = true;
//	_path_to_load = path;
//}

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
	_qt_buffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(fbo, 1));
}

void Core::mouseclick(int x, int y, Input button)
{
	switch (button)
	{
	case Input::MOUSE_LEFT_BUTTON:
		_mouse_x = x;
		_mouse_y = y;
		std::cout << "click: " << _mouse_x << "  " << _mouse_y << std::endl;
		break;
	default:
		break;
	}
	std::cout << "mouse press event" << std::endl;
}

void Core::mouseMoved(int x, int y, Input button)
{
	switch (button)
	{
	case Input::MOUSE_LEFT_BUTTON:
		std::cout << "move: " << y - _mouse_y << "  " << x - _mouse_x << std::endl;
		_cam.rotate(-(y - _mouse_y)*ROT_SPEED, (x - _mouse_x)*ROT_SPEED);
		//_cam.update();
		_mouse_x = x;
		_mouse_y = y;
		break;
	default:
		break;
	}
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