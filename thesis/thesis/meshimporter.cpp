#include "meshimporter.h"

#include <iostream>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#undef AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE
#define AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE 80.0f

#include "utils.h"

inline std::vector<glm::vec3> getAiMeshVertices(std::unique_ptr<aiMesh>& mesh)
{
	std::vector<glm::vec3> vertices(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		vertices[i] = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
	return vertices;
}

inline std::vector<unsigned int> getAiFaces(std::unique_ptr<aiMesh>& mesh)
{
	std::vector<unsigned int> faces(mesh->mNumFaces*3);
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		for (int j = 0; j < 3; ++j)
			faces[i * 3 + j] = face.mIndices[j];
	}
	return faces;
}

inline std::vector<glm::vec3> getAiMeshNormals(std::unique_ptr<aiMesh>& mesh)
{
	std::vector<glm::vec3> normals(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		aiVector3D norm = mesh->mNormals[i].Normalize();
		normals[i] = glm::vec3(norm.x, norm.y, norm.z);
	}
	return normals;
}

inline std::vector<glm::vec4> getAiMeshColors(std::unique_ptr<aiMesh>& mesh)
{
	std::vector<glm::vec4> colors(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		aiColor4D color = mesh->mColors[0][i];
		colors[i] = glm::vec4(color.r, color.g, color.b, color.a);
	}
	return colors;
}

inline std::vector<glm::vec2> getAiMeshUVcoords(std::unique_ptr<aiMesh>& mesh)
{
	std::vector<glm::vec2> tex_cooords(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		aiVector3D uv = mesh->mTextureCoords[0][i];
		tex_cooords[i] = glm::vec2(uv.x, uv.y);
	}
	return tex_cooords;
}

Assimp::Importer importer;

std::unique_ptr<aiMesh> loadMeshFromFile(const std::string& path)
{
	//Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenSmoothNormals |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_ValidateDataStructure |
		aiProcess_ImproveCacheLocality
		);

	if (!scene)
	{
		std::cerr << importer.GetErrorString() << std::endl;
		throw_non_critical(importer.GetErrorString());
	}
	return std::unique_ptr<aiMesh> (scene->mMeshes[0]);
}

inline void meshInfo(const std::unique_ptr<aiMesh>& mesh)
{
	std::cout << "\t #faces: " << mesh->mNumFaces << std::endl;
	std::cout << "\t #vertices: " << mesh->mNumVertices << std::endl;
	if (mesh->HasVertexColors(0)) std::cout << "\t Has color per vertex" << std::endl;
	if (mesh->HasNormals()) std::cout << "\t Has normals" << std::endl;
	std::cout << "\t #UV Channels " << mesh->GetNumUVChannels() << std::endl;
	if (mesh->HasTextureCoords(0)) std::cout << "\t Has texcoords" << std::endl;
	if (mesh->HasTangentsAndBitangents()) std::cout << "\t Has Tangents and bitangents" << std::endl;
}

#include <iostream>
#include<chrono>
using time_unit = std::chrono::milliseconds;
//using time_unit = std::chrono::microseconds;
//using time_unit = std::chrono::nanoseconds;

std::shared_ptr<Mesh> MeshImporter::importMeshFromFile(const std::string& path, bool info)
{
	std::chrono::high_resolution_clock timmer;
	std::chrono::high_resolution_clock::time_point t1, t2;
	if(info) t1 = timmer.now();
	std::cout << "Loading... " << path << std::endl;
	std::unique_ptr<aiMesh> mesh = loadMeshFromFile(path);
	//std::cout << "mesh  " << mesh.get() << std::endl;
	/*if(info)*/ meshInfo(mesh);
	//std::vector<glm::vec3> vertices = getAiMeshVertices(mesh);
	//std::vector<unsigned int> faces = getAiFaces(mesh);
	//std::vector<glm::vec3> normals;
	//std::vector<glm::vec4> colors;

	//if(mesh->HasNormals()) normals = getAiMeshNormals(mesh);
	//if (mesh->HasVertexColors(0)) colors = getAiMeshColors(mesh);

	if (info)
	{
		t2 = timmer.now();
		std::cout << "...Loaded(" << std::chrono::duration_cast<time_unit>(t2 - t1).count() << "ms)." << std::endl;
	}
	else std::cout << "...Loaded." << std::endl;


	std::shared_ptr<Mesh> return_mesh = std::shared_ptr<Mesh>(new Mesh(getAiFaces(mesh), getAiMeshVertices(mesh)));
	if (mesh->HasNormals())
	{
		return_mesh->addNormals(getAiMeshNormals(mesh));
	}
	if (mesh->HasVertexColors(0))
	{
		return_mesh->addColors(getAiMeshColors(mesh));
	}
	if (mesh->HasTextureCoords(0))
	{
		return_mesh->addTexCoords(getAiMeshUVcoords(mesh));
	}
	if (mesh->HasTangentsAndBitangents())
	{

	}

	mesh.release();

	return return_mesh;
}
