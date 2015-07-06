#include "mesh.h"

#include "utils.h"
//Mesh::Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices)
//{
//	std::vector<glm::vec3> normals = computeNormals();
//}

Mesh::Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices)
{
	initialize(idx, vertices);
}

//Mesh::Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals)
//	: RenderableObject()
//{
//	initialize(idx, vertices, normals);
//}
//
//Mesh::Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors)
//{
//	initialize(idx, vertices, normals, colors);
//}
//
//
//Mesh::Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors, const std::vector<glm::vec2> &tex_coords)
//{
//
//}
//
//Mesh::Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors, const std::vector<glm::vec2> &tex_coords, const std::vector<glm::vec3> &tangent)
//{
//
//}
//
//Mesh::Mesh(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors, const std::vector<glm::vec2> &tex_coords, const std::vector<glm::vec3> &tangent, const std::vector<glm::vec3> &bi_tangent)
//{
//
//}

Mesh::~Mesh()
{
	destroy();
}

void Mesh::destroy()
{
	vertices_.clear();
	vTable_.clear();
	normals_.clear();
	colors_.clear();
	glDeleteBuffers(1, &vboNormalsId_);
	glDeleteBuffers(1, &vboColorId_);
	glDeleteBuffers(1, &vboTexCoordsId_);
	glDeleteBuffers(1, &vboTangentsId_);
	checkCritOpenGLError();
}

void Mesh::initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices)
{
	assert(idx.size());
	assert(vertices.size());
	vTable_ = idx;
	vertices_ = vertices;
	RenderableObject::initialize();
	bbox_computed_ = false;
}

void Mesh::initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals)
{
	assert(normals.size());
	initialize(idx, vertices);
	normals_ = normals;
	initializeNormalsBuffer();
}

void Mesh::initialize(const std::vector<unsigned int>& idx, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec4> &colors)
{
	assert(colors.size());
	initialize(idx, vertices, normals);
	colors_ = colors;
	initializeColorBuffer();
}

void Mesh::addNormals(const std::vector<glm::vec3> &normals)
{
	assert(normals.size() == vertices_.size());
	normals_ = normals;
	initializeNormalsBuffer();
}

void Mesh::addColors(const std::vector<glm::vec4> &colors)
{
	assert(colors.size() == vertices_.size());
	colors_ = colors;
	initializeColorBuffer();
}

void Mesh::addTexCoords(const std::vector<glm::vec2> &tex_coords)
{
	assert(tex_coords.size() == vertices_.size());
	tex_coords_ = tex_coords;
	initializeTexCoordsBuffer();
}

void Mesh::initializeNormalsBuffer()
{
	glGenBuffers(1, &vboNormalsId_);
	glBindVertexArray(vaoId_);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormalsId_);
	glBufferData(GL_ARRAY_BUFFER, totalVertices_ * sizeOfVertexElement(), 0, GL_STATIC_DRAW);

	GLfloat* pBuffer = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	assert(pBuffer != nullptr);
	fillNormalBuffer(pBuffer);
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	assert(glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, vertexNumberOfComponents(), GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	checkCritOpenGLError();
}

void Mesh::initializeColorBuffer()
{
	glGenBuffers(1, &vboColorId_);
	glBindVertexArray(vaoId_);
	glBindBuffer(GL_ARRAY_BUFFER, vboColorId_);
	glBufferData(GL_ARRAY_BUFFER, totalVertices_ * sizeOfColorlement(), 0, GL_STATIC_DRAW);

	GLfloat* pBuffer = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	assert(pBuffer != nullptr);
	fillColorBuffer(pBuffer);
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	assert(glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, colorNumberOfComponents(), GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	checkCritOpenGLError();
}
void Mesh::initializeTexCoordsBuffer()
{
	glGenBuffers(1, &vboTexCoordsId_);
	glBindVertexArray(vaoId_);
	glBindBuffer(GL_ARRAY_BUFFER, vboTexCoordsId_);
	glBufferData(GL_ARRAY_BUFFER, totalVertices_ * sizeOfTextureCoordslement(), 0, GL_STATIC_DRAW);

	GLfloat* pBuffer = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	assert(pBuffer != nullptr);
	fillColorBuffer(pBuffer);
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	assert(glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, colorNumberOfComponents(), GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	checkCritOpenGLError();
}

void Mesh::fillTexCoordsBuffer(GLfloat* pBuffer)
{

}

void Mesh::fillVertexBuffer(GLfloat* pBuffer)
{
	glm::vec3* vert = (glm::vec3*)(pBuffer);
	for (unsigned int i = 0; i < totalVertices_; ++i)
		vert[i] = vertices_[i];
}

void Mesh::fillColorBuffer(GLfloat* pBuffer)
{
	glm::vec4* vert = (glm::vec4*)(pBuffer);
	for (unsigned int i = 0; i < totalVertices_; ++i)
		vert[i] = colors_[i];
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

unsigned int Mesh::sizeOfVertexElement()
{
	return sizeof(glm::vec3);
}

unsigned int Mesh::vertexNumberOfComponents()
{
	return 3;
}

unsigned int Mesh::sizeOfColorlement()
{
	return sizeof(glm::vec4);
}

unsigned int Mesh::colorNumberOfComponents()
{
	return 4;
}

unsigned int Mesh::sizeOfTextureCoordsElement()
{
	return sizeof(glm::vec2);
}

unsigned int textureCoordsNumberOfComponents()
{
	return 2;
}

BBox Mesh::getBBox()
{
	if (!bbox_computed_)
		computeBBox();
	return bbox_;
}

void Mesh::computeBBox()
{
	glm::vec3 min = vertices_[0];
	glm::vec3 max = vertices_[0];

	for (unsigned int i = 1; i < vertices_.size(); ++i)
	{
		min = glm::min(min, vertices_[i]);
		max = glm::max(max, vertices_[i]);
	}
	bbox_.set(min, max);
	bbox_computed_ = true;
}