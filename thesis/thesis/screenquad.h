#ifndef SCREENQUAD_H
#define SCREENQUAD_H

#include "renderableobject.h"
#include <glm\glm.hpp>
#include <vector>

class ScreenQuad : public RenderableObject
{
public:
	static ScreenQuad& getInstance();
	static ScreenQuad* getInstanceP();
	~ScreenQuad();

protected:
	//void destroy();

	unsigned int getTotalVertices();
	unsigned int getTotalIndices();
	GLenum getPrimitiveType();
	unsigned int sizeOfVertexElement();
	unsigned int vertexNumberOfComponents();
	void fillVertexBuffer(GLfloat* pBuffer);
	void fillIndexBuffer(GLuint* pBuffer);

private:
	ScreenQuad();
	ScreenQuad(ScreenQuad const&) = delete;
	void operator=(ScreenQuad const&) = delete;

	static const std::vector<glm::vec2> vertices_;
	static const std::vector<unsigned int> indices_;
	static ScreenQuad* instance_;
};

#endif // SCREENQUAD_H
