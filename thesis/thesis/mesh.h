#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm\glm.hpp>

#include "renderableobject.h"
#include "bbox.h"

class Mesh : public RenderableObject
{
public:
	Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices);
	Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals);
	Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors);
	~Mesh();
	BBox getBBox();

protected:
	void destroy();

	unsigned int getTotalVertices();
	unsigned int getTotalIndices();
	GLenum getPrimitiveType();
	unsigned int sizeOfVertexElement();
	unsigned int vertexNumberOfComponents();
	unsigned int sizeOfColorlement();
	unsigned int colorNumberOfComponents();
	void fillVertexBuffer(GLfloat* pBuffer);
	void fillIndexBuffer(GLuint* pBuffer);
	void fillNormalBuffer(GLfloat* pBuffer);
	void fillColorBuffer(GLfloat* pBuffer);

private:
	void initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices);
	void initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals);
	void initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors);


	void initializeNormalsBuffer();
	void initializeColorBuffer();
	void computeBBox();

	GLuint vboNormalsId_, vboColorId_;
	std::vector<unsigned int> vTable_;
	std::vector<glm::vec3> vertices_, normals_;
	std::vector<glm::vec4> colors_;
	BBox bbox_;
	bool bbox_computed_;
};

#endif // MESH_H
