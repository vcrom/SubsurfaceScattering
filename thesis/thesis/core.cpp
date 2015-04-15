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

	//GLuint textureID;
	//glGenTextures(1, &textureID);
	//glBindTexture(GL_TEXTURE_2D, textureID);
	//glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT);

	//std::cout << "OpenGL No init" << std::endl;
	//checkCritOpenGLError();


	std::cout << "OpenGL init" << std::endl;
}

void Core::initialize()
{
	initializeGL();

	std::cout << "Init shaders" << std::endl;

	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/screen_space_quad.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.unUse();
	checkCritOpenGLError();
	std::cout << "Core initialized" << std::endl;


	TextureManager tex_man;
	//tex_man.loadTexture("textures/squares.jpg");
	tex_man.loadTexture("textures/squares.png");

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
	ScreenQuad* quad = ScreenQuad::getInstance();
	shader.use();
		quad->render();
	shader.unUse();
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, cosa_);
	//glVertexAttribPointer(
	//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	//	3,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride
	//	(void*)0            // array buffer offset
	//	);

	//// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	//glDisableVertexAttribArray(0);

	checkCritOpenGLError();
}

void Core::loadMesh(const std::string& path)
{
	Mesh m = MeshImporter::importMeshFromFile(path);
	std::cout << "LOALA" << std::endl;
}