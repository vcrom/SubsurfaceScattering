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
	void fillVertexBuffer(GLfloat* pBuffer);
	void fillIndexBuffer(GLuint* pBuffer);
	void fillNormalBuffer(GLfloat* pBuffer);

private:
	const glm::vec2 vertices[4] = { glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0), glm::vec2(0.0, 1.0)};
	const GLushort indices[] = { 0, 1, 2, 0, 2, 3 };


};

#endif // SCREENQUAD_H
