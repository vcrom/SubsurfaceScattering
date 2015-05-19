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
	void useFrameBuffer() const;

	GLuint getFrameBufferID();
	GLuint getColorLevels();


	void colorBuffer(GLuint tex, unsigned int attachment, GLint level = 0);
	void depthBuffer(GLuint tex, GLint level = 0);


	//raw opengl
	void attachTexture(GLenum target, GLenum attachment, GLuint texture, GLint level);
	void attachRenderBuffer(GLenum attachment, GLuint renderbuffer);

	bool checkStatus();
	static void useFrameBuffer(GLuint fbo);


private:
	GLuint _id;
	GLuint _color_levels;

	void debugAsserts();
	bool isBinded() const;
};

#endif // FRAMEBUFFER_H
