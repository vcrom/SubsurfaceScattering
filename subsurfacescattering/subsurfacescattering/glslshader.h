#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include <GL/glew.h>
#include <map>
#include <string>

class GlslShader
{
public:
	GlslShader(void);
	~GlslShader(void);
	void loadFromString(GLenum whichShader, const std::string& source);
	void loadFromFile(GLenum whichShader, const std::string& filename);
	void createAndLinkProgram();
	void use();
	void unUse();
	void addAttribute(const std::string& attribute);
	void addUniform(const std::string& uniform);

	//An indexer that returns the location of the attribute/uniform
	GLuint operator[](const std::string& attribute);
	GLuint operator()(const std::string& uniform);
	void deleteShaderProgram();

private:
	enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER };
	GLuint _program;
	int _totalShaders;
	GLuint _shaders[3];//0->vertexshader, 1->fragmentshader, 2->geometryshader
	std::map<std::string, GLuint> _attributeList;
	std::map<std::string, GLuint> _uniformLocationList;
};

#endif // GLSLSHADER_H
