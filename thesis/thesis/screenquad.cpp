#include "screenquad.h"

const std::vector<glm::vec2> ScreenQuad::vertices_ = { glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0), glm::vec2(0.0, 1.0) };
const std::vector<unsigned int> ScreenQuad::indices_ = { 0, 1, 2, 0, 2, 3 };

ScreenQuad* ScreenQuad::instance_ = nullptr;

ScreenQuad* ScreenQuad::getInstance()
{
	return instance_ != nullptr ? instance_ : (instance_ = new ScreenQuad);
}

ScreenQuad::ScreenQuad()
	: RenderableObject()
{
	initialize();
}

ScreenQuad::~ScreenQuad()
{
	delete instance_;
	instance_ = nullptr;
}

unsigned int ScreenQuad::getTotalVertices()
{
	return vertices_.size();
}

unsigned int ScreenQuad::getTotalIndices()
{
	return indices_.size();
}

GLenum ScreenQuad::getPrimitiveType()
{
	return GL_TRIANGLES;
}

unsigned int ScreenQuad::sizeOfVertexElement()
{
	return sizeof(glm::vec2);
}

unsigned int ScreenQuad::vertexNumberOfComponents()
{
	return 2;
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