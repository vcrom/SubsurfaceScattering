#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>

//OpenGl context must be active
class Texture
{
public:
	Texture(GLenum target);
	~Texture();


	//getters
	GLuint getTextureID();
	GLenum getTarget();
	GLenum getInternalFormat();
	GLsizei getLevels();

	//uils
	void use(GLenum tex_unit = GL_TEXTURE0);
	void setTexParameter(GLenum pname, GLint param);
	void setTexParameter(GLenum pname, GLfloat param);

protected:
	GLenum _target, _internal_format;
	GLsizei _levels;

	bool isBinded();

private:
	GLuint _id;
};

#endif // TEXTURE_H
