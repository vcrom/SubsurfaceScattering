#include "screenquad.h"

const glm::vec2 ScreenQuad::vertices_[4] = {glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0), glm::vec2(0.0, 1.0)};
const unsigned int ScreenQuad::indices_[6] = {0, 1, 2, 0, 2, 3};

ScreenQuad::ScreenQuad()
	: RenderableObject()
{
}

ScreenQuad::~ScreenQuad()
{
}

unsigned int ScreenQuad::getTotalVertices()
{
	return 4;
}
unsigned int ScreenQuad::getTotalIndices()
{
	return 6;
}
GLenum ScreenQuad::getPrimitiveType()
{
	return GL_TRIANGLES;
}

unsigned int ScreenQuad::sizeOfVertexElement()
{
	return sizeof(glm::vec2);
}

void ScreenQuad::fillVertexBuffer(GLfloat* pBuffer)
{
	glm::vec2* vert = (glm::vec2*)(pBuffer);
	for (unsigned int i = 0; i < totalVertices_; ++i)
		vert[i] = vertices_[i];
}

void ScreenQuad::fillIndexBuffer(GLuint* pBuffer)
{
	int* idx = (int*)(pBuffer);
	for (unsigned int i = 0; i < totalIndices_; ++i)
		idx[i] = indices_[i];
}