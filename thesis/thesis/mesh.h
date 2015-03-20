#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm\glm.hpp>

#include "renderableobject.h"

class Mesh : public RenderableObject
{
public:
	//Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices);
	Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals);
	~Mesh();

protected:
	void destroy();

	unsigned int getTotalVertices();
	unsigned int getTotalIndices();
	GLenum getPrimitiveType();
	unsigned int sizeOfVertexElement();
	unsigned int vertexNumberOfComponents();
	void fillVertexBuffer(GLfloat* pBuffer);
	void fillIndexBuffer(GLuint* pBuffer);
	void fillNormalBuffer(GLfloat* pBuffer);

private:
	void initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals);
	void initializeNormalsBuffer();

	GLuint vboNormalsId_;
	std::vector<unsigned int> vTable_;
	std::vector<glm::vec3> vertices_, normals_;	
};

#endif // MESH_H
