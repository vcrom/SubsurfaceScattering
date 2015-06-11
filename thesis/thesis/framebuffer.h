#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL\glew.h>

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(int id_buff, unsigned int color_levels);
	~FrameBuffer();
	void createFrameBuffer();
	void deleteFrameBuffer();
	void useFrameBuffer(unsigned int color_levels = 1) const;

	GLuint getFrameBufferID();
	GLuint getColorLevels();


	void clearColor();
	void clearDepth();
	void clearStencil();
	void clearColorAndDepth();
	void clearColorAndStencil();
	void clearDepthAndStencil();
	void clearColorDepthAndStencil();
	void colorBuffer(GLuint tex, unsigned int attachment, GLint level = 0);
	void depthBuffer(GLuint tex, GLint level = 0);
	void stencilBuffer(GLuint tex, GLint level = 0);
	void depthAndStencilBuffer(GLuint tex, GLint level = 0);




	//raw opengl
	void attachTexture(GLenum target, GLenum attachment, GLuint texture, GLint level);
	void attachRenderBuffer(GLenum attachment, GLuint renderbuffer);

	bool checkStatus();
	//static void useFrameBuffer(GLuint fbo);


private:
	GLuint _id;
	GLuint _color_levels;

	void debugAsserts();
	bool isBinded() const;
};

#endif // FRAMEBUFFER_H
