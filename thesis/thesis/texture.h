#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>

//OpenGl context must be active
class Texture
{
public:
	Texture(GLenum target);
	~Texture();

	void createTexture();
	void deleteTexture();

	//getters
	GLuint getTextureID();
	GLenum getTarget();
	GLenum getInternalFormat();
	GLsizei getLevels();

	virtual unsigned char* getTextureData(GLint level, GLenum format, GLenum type, unsigned int channels) = 0;

	static GLint getBindedTextured();

	//uils
	void use(GLenum tex_unit = GL_TEXTURE0);
	void setTexParameter(GLenum pname, GLint param);
	void setTexParameter(GLenum pname, GLfloat param);

protected:
	GLenum _target, _internal_format;
	GLsizei _levels;
	GLuint _id;

	bool isBinded();

private:
};

#endif // TEXTURE_H
