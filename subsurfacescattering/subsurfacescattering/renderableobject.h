#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class RenderableObject
{
public:
	RenderableObject();
	~RenderableObject();
	void render() const;

protected:
	void initialize();
	void destroy();

	GLuint vaoId_;
	GLuint vboVerticesId_;
	GLuint vboIndicesId_;

	GLenum primType_;
	int totalVertices_, totalIndices_;

	virtual unsigned int getTotalVertices() = 0;
	virtual unsigned int getTotalIndices() = 0;
	virtual GLenum getPrimitiveType() = 0;
	virtual unsigned int sizeOfVertexElement() = 0;
	virtual unsigned int vertexNumberOfComponents() = 0;
	virtual void fillVertexBuffer(GLfloat* pBuffer) = 0;
	virtual void fillIndexBuffer(GLuint* pBuffer) = 0;	
};

#endif // RENDERABLEOBJECT_H
