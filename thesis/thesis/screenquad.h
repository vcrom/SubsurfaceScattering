#ifndef SCREENQUAD_H
#define SCREENQUAD_H

#include "renderableobject.h"
#include <glm\glm.hpp>

class ScreenQuad : public RenderableObject
{
public:
	ScreenQuad();
	~ScreenQuad();

protected:
	//void destroy();

	unsigned int getTotalVertices();
	unsigned int getTotalIndices();
	GLenum getPrimitiveType();
	unsigned int sizeOfVertexElement();
	void fillVertexBuffer(GLfloat* pBuffer);
	void fillIndexBuffer(GLuint* pBuffer);

private:
	static const glm::vec2 vertices_[4];
	static const unsigned int indices_[6];
};

#endif // SCREENQUAD_H
