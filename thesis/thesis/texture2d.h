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
	unsigned char* getTextureData();
	unsigned char* getTextureData(GLint level, GLenum format, GLenum type, unsigned int channels = 3);

	//utils
	void resize(GLsizei width, GLsizei height);

	//loaders
	void loadBufferToTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
	void loadBufferToTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
	void loadEmptyTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border);
	void loadEmptyTexture(GLint level, GLint internalformat, GLsizei width, GLsizei height);
	void loadEmptyTexture(GLint internalformat, GLsizei width, GLsizei height);

private:
	GLsizei _width, _height;
	GLint _border;
	
};

#endif // TEXTURE2D_H
