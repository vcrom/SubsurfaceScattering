#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "texture.h"

class Texture2D : public Texture
{
public:
	Texture2D();
	Texture2D(GLenum target);
	~Texture2D();

	//getters
	GLsizei getWidth();
	GLsizei getHeight();

	//utils
	void allocateTextureStorage(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
	void loadImageToTexture(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);

private:
	GLsizei _width, _height;
	
};

#endif // TEXTURE2D_H
