#include "mesh.h"

//Mesh::Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices)
//{
//	std::vector<glm::vec3> normals = computeNormals();
//}

Mesh::Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals)
{
	initialize(idx, vertices, normals);
}

Mesh::~Mesh()
{
	destroy();
}

void Mesh::destroy()
{
	vertices_.clear();
	vTable_.clear();
	normals_.clear();
	glDeleteBuffers(1, &vboNormalsId_);
}

void Mesh::initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals)
{
	vTable_ = idx;
	vertices_ = vertices;
	normals_ = normals;
	RenderableObject::initialize();
	initializeNormalsBuffer();
}

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);
void Mesh::initializeNormalsBuffer()
{
	glGenBuffers(1, &vboNormalsId_);
	glBindVertexArray(vaoId_);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormalsId_);
	glBufferData(GL_ARRAY_BUFFER, totalVertices_ * sizeof(glm::vec3), 0, GL_STATIC_DRAW);

	GLfloat* pBuffer = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	fillNormalBuffer(pBuffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	GL_CHECK_ERRORS
}

unsigned int Mesh::getTotalVertices()
{
	return vertices_.size();
}

unsigned int Mesh::getTotalIndices()
{
	return vTable_.size();
}

GLenum Mesh::getPrimitiveType()
{
	return GL_TRIANGLES;
}

void Mesh::fillVertexBuffer(GLfloat* pBuffer)
{
	glm::vec3* vert = (glm::vec3*)(pBuffer);
	for (unsigned int i = 0; i < totalVertices_; ++i)
		vert[i] = vertices_[i];
}

void Mesh::fillIndexBuffer(GLuint* pBuffer)
{
	int* idx = (int*)(pBuffer);
	for (unsigned int i = 0; i < totalIndices_; ++i)
		idx[i] = vTable_[i];
}

void Mesh::fillNormalBuffer(GLfloat* pBuffer)
{
	glm::vec3* norm = (glm::vec3*)(pBuffer);
	for (glm::uint i = 0; i < totalVertices_; ++i)
		norm[i] = normals_[i];
}