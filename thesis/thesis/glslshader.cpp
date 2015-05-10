#include "glslshader.h"
#include <iostream>


GlslShader::GlslShader(void)
{
	_totalShaders = 0;
	_program = 0;
	_shaders[VERTEX_SHADER] = 0;
	_shaders[FRAGMENT_SHADER] = 0;
	_shaders[GEOMETRY_SHADER] = 0;
	_attributeList.clear();
	_uniformLocationList.clear();
}

GlslShader::~GlslShader(void)
{
}

void GlslShader::deleteShaderProgram() {
	if (_program != 0) glDeleteProgram(_program);
	_attributeList.clear();
	_uniformLocationList.clear();
}

#include "utils.h"
#include <assert.h>
void GlslShader::loadFromString(GLenum type, const std::string& source)
{
	GLuint shader = glCreateShader(type);

	const char * ptmp = source.c_str();
	glShaderSource(shader, 1, &ptmp, NULL);

	//check whether the shader loads fine
	GLint status;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		std::string info(infoLog);
		delete[] infoLog;
		std::cerr << "Compile log: " << info << std::endl;
		//assert(false);
		throw_non_critical("Compile log: " + info);
	}

	switch (type)
	{
		case GL_VERTEX_SHADER:
			_shaders[VERTEX_SHADER] = shader;
			break;
		case GL_FRAGMENT_SHADER:
			_shaders[FRAGMENT_SHADER] = shader;
			break;
		case GL_GEOMETRY_SHADER:
			_shaders[GEOMETRY_SHADER] = shader;
			break;
		default:
			assert(false);
			break;
	}
}


void GlslShader::createAndLinkProgram()
{
	_program = glCreateProgram();
	if (_shaders[VERTEX_SHADER] != 0) {
		glAttachShader(_program, _shaders[VERTEX_SHADER]);
	}
	if (_shaders[FRAGMENT_SHADER] != 0) {
		glAttachShader(_program, _shaders[FRAGMENT_SHADER]);
	}
	if (_shaders[GEOMETRY_SHADER] != 0) {
		glAttachShader(_program, _shaders[GEOMETRY_SHADER]);
	}

	//link and check whether the program links fine
	GLint status;
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;

		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(_program, infoLogLength, NULL, infoLog);
		std::string info(infoLog);
		delete[] infoLog;
		std::cerr << "Link log: " << info << std::endl;
		throw_non_critical("Link log: " + info);
	}

	glDeleteShader(_shaders[VERTEX_SHADER]);
	glDeleteShader(_shaders[FRAGMENT_SHADER]);
	glDeleteShader(_shaders[GEOMETRY_SHADER]);
}

void GlslShader::use()
{
	glUseProgram(_program);
}

void GlslShader::unUse()
{
	glUseProgram(0);
}

void GlslShader::addAttribute(const  std::string& attribute)
{
	_attributeList[attribute] = glGetAttribLocation(_program, attribute.c_str());
}

//An indexer that returns the location of the attribute
GLuint GlslShader::operator [](const  std::string& attribute)
{
	return _attributeList[attribute];
}

void GlslShader::addUniform(const  std::string& uniform)
{
	_uniformLocationList[uniform] = glGetUniformLocation(_program, uniform.c_str());
}

GLuint GlslShader::operator()(const  std::string& uniform)
{
	return _uniformLocationList[uniform];
}

#include <fstream>
void GlslShader::loadFromFile(GLenum whichShader, const  std::string& filename){
	std::ifstream fp;
	fp.open(filename.c_str(), std::ios_base::in);
	if (fp) {
		std::string line, buffer;
		while (std::getline(fp, line)) {
			buffer.append(line);
			buffer.append("\r\n");
		}
		//copy to source
		loadFromString(whichShader, buffer);
	}
	else {
		std::cerr << "Error opening shader: " << filename << std::endl;
		throw_non_critical("Error opening shader: " + filename);
	}
}
