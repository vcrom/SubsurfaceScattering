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

	static GLint getBindedTexture2d();
	static GLint getBindedTextureCubeMap();

	//uils
	void use(GLenum tex_unit = GL_TEXTURE0) const;
	void setTexParameter(GLenum pname, GLint param);
	void setTexParameter(GLenum pname, GLfloat param);
	void setTexParameter(GLenum pname, const GLint *param);
	void setTexParameter(GLenum pname, const GLfloat *param);
	void generateMipMaps();

protected:
	GLenum _target, _internal_format;
	GLsizei _levels;
	GLuint _id;

	bool isBinded() const;

private:
};

#endif // TEXTURE_H
