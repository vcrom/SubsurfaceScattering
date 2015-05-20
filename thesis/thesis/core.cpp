#include "core.h"

#include <iostream>

#include "utils.h"
#include "meshimporter.h"
#include "renderalgorithms.h"

Core::Core()
{
	_mouse_x = _mouse_y = 0;
}

Core::~Core()
{

}

void Core::glewInitialization()
{
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
	shader_manager->initializeShaders();

	//std::cout << "Init shaders" << std::endl;

	//shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	//shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/screen_space_quad.frag");
	//shader.createAndLinkProgram();
	//shader.use();
	//shader.addAttribute("vVertex");
	//shader.addUniform("color_texture");
	//glUniform1i(shader("color_texture"), 0);
	//shader.unUse();
	//checkCritOpenGLError();
	//std::cout << "Core initialized" << std::endl;


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
	buffer = new FrameBuffer();
	buffer->createFrameBuffer();
	//glBindFramebuffer(GL_FRAMEBUFFER, buffer->getFrameBufferID());
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_col->getTextureID(), 0);
	//GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//if (status == GL_FRAMEBUFFER_COMPLETE) std::cout << "FBO horizontal blur successful." << std::endl;
	//else std::cout << "Error in FBO horizontal blur. " << status << std::endl;

	buffer->useFrameBuffer();
	buffer->colorBuffer(tex_col->getTextureID(), 0);
	if (buffer->checkStatus()) std::cout << "Buffer init" << std::endl;
	else std::cout << "Buffer NO init" << std::endl;
	checkCritOpenGLError();

	_qt_buffer = new FrameBuffer(RenderAlgorithms::default_buffer, 1);
	//mesh = MeshImporter::importMeshFromFile("meshes/sphere.ply");
	//mesh = MeshImporter::importMeshFromFile("meshes/bunny.ply");
	mesh = MeshImporter::importMeshFromFile("meshes/tests.ply");
	
	_cam.updateProjection(glm::radians(45.0f), 1.0f);
	_cam.initFromBBox(mesh->getBBox());
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
	//RenderAlgorithms::renderTexture(*buffer, *tex);
	//RenderAlgorithms::renderTexture(*_qt_buffer, *tex_col);
	//glClear(GL_DEPTH_BUFFER_BIT);
	RenderAlgorithms::renderMesh(*_qt_buffer, mesh, _cam.getViewMatrix(), _cam.getProjectionMatrix());

	//checkCritOpenGLError();
	
}

void Core::loadMesh(const std::string& path)
{
	if (mesh != nullptr){
		delete mesh;
		mesh = nullptr;
	}
	mesh = MeshImporter::importMeshFromFile(path);
	_cam.initFromBBox(mesh->getBBox()); 
	_cam.update();
}

void Core::setDefaultFBO(GLuint fbo)
{
	RenderAlgorithms::default_buffer = fbo;
	_qt_buffer = new FrameBuffer(fbo, 1);
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