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
	//Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals);
	//Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors);
	//Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors, const std::vector<glm::vec2> &tex_coords);
	//Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors, const std::vector<glm::vec2> &tex_coords, const std::vector<glm::vec3> &tangent);
	//Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors, const std::vector<glm::vec2> &tex_coords, const std::vector<glm::vec3> &tangent, const std::vector<glm::vec3> &bi_tangent);

	void addNormals(const std::vector<glm::vec3> &normals);
	void addColors(const std::vector<glm::vec4> &colors);
	void addTexCoords(const std::vector<glm::vec2> &tex_coords);
	void addTangents(const std::vector<glm::vec3> &tangents);
	~Mesh();

	BBox getBBox();
	bool hasTexCoords();

protected:
	void destroy();

	unsigned int getTotalVertices();
	unsigned int getTotalIndices();
	GLenum getPrimitiveType();
	unsigned int sizeOfVertexElement();
	unsigned int vertexNumberOfComponents();
	unsigned int sizeOfColorlement();
	unsigned int colorNumberOfComponents();
	unsigned int sizeOfTextureCoordsElement();
	unsigned int textureCoordsNumberOfComponents();
	void fillVertexBuffer(GLfloat* pBuffer);
	void fillIndexBuffer(GLuint* pBuffer);
	void fillNormalBuffer(GLfloat* pBuffer);
	void fillColorBuffer(GLfloat* pBuffer);
	void fillTexCoordsBuffer(GLfloat* pBuffer);
	void fillTangentBuffer(GLfloat* pBuffer);


private:
	void initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices);
	void initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals);
	void initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors);


	void initializeNormalsBuffer();
	void initializeColorBuffer();
	void initializeTexCoordsBuffer();
	void initializeTangentsBuffer();
	void computeBBox();

	GLuint vboNormalsId_, vboColorId_, vboTexCoordsId_, vboTangentsId_;
	std::vector<unsigned int> vTable_;
	std::vector<glm::vec2> tex_coords_;
	std::vector<glm::vec3> vertices_, normals_, tangents_;
	std::vector<glm::vec4> colors_;
	BBox bbox_;
	bool bbox_computed_;
};

#endif // MESH_H
