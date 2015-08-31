#ifndef TEXTURECUBE_H
#define TEXTURECUBE_H

#include "texture.h"

class TextureCube : public Texture
{
public:
	TextureCube();
	TextureCube(GLenum target);
	~TextureCube();
	
	//initialization
	//void 

	GLsizei getWidth();
	GLsizei getHeight();

	//unsigned char* getTextureData(Gluint face);

	//loaders
	void loadBufferToFace(GLuint face, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
	void loadBufferToFace(GLuint face, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);

private:
	GLsizei _width, _height;
	GLint _border;
};

#endif // TEXTURECUBE_H
