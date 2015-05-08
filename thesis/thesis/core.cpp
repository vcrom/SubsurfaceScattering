#include "core.h"

#include <iostream>

#include "utils.h"
#include "meshimporter.h"

Core::Core()
{

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
#include "glslshadermanager.h"

void Core::initialize()
{
	initializeGL();
	GlslShaderManager shader_manager = GlslShaderManager::instance();
	shader_manager.initializeShaders();

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

	//unsigned int n_pixels = tex->getWidth()*tex->getHeight()*3;
	//unsigned char *pixels = new unsigned char[n_pixels];
	//std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	//start = std::chrono::high_resolution_clock::now();
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	//end = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double, std::milli> elapsed_mseconds = end - start;
	//std::cout << "time obtaining the texture: " << elapsed_mseconds.count() << std::endl;
	//checkCritOpenGLError();

	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();
	image2.loadImage(tex->getTextureData(), tex->getWidth(), tex->getHeight());
	image2.writeImage("textures/blep.jpg");
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed_mseconds = end - start;
	std::cout << "MAIN time writing the image: " << elapsed_mseconds.count() << std::endl;
}

void Core::resize(unsigned int w, unsigned int h)
{
	std::cout << "Resize(" << w << ", " << h << ")" << std::endl;
	glViewport(0, 0, w, h);
}

#include "screenquad.h"
void Core::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	std::cout << "Render" << std::endl;
	tex->use(GL_TEXTURE0);
	//FIXME obtain shader
	ScreenQuad* quad = ScreenQuad::getInstance();
	//shader 
	shader.use();
		quad->render();
	shader.unUse();
	checkCritOpenGLError();
}

void Core::loadMesh(const std::string& path)
{
	Mesh m = MeshImporter::importMeshFromFile(path);
	std::cout << "LOALA" << std::endl;
}