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

Assimp::Importer importer;

std::unique_ptr<aiMesh> loadMeshFromFile(const std::string& path)
{
	//Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_ValidateDataStructure |
		aiProcess_ImproveCacheLocality |
		aiProcess_GenSmoothNormals
		);

	if (!scene)
	{
		std::cerr << importer.GetErrorString() << std::endl;
		throw_non_critical(importer.GetErrorString());
	}
	return std::unique_ptr<aiMesh> (scene->mMeshes[0]);
}

Mesh MeshImporter::importMeshFromFile(const std::string& path)
{
	std::cout << "Loading: " << path << std::endl;
	std::unique_ptr<aiMesh> mesh = loadMeshFromFile(path);
	std::vector<unsigned int> faces = getAiFaces(mesh);
	std::vector<glm::vec3> vertices = getAiMeshVertices(mesh);
	std::vector<glm::vec3> normals = getAiMeshNormals(mesh);
	std::cout << "...Loaded." <<std::endl;
	mesh.release();
	return Mesh(faces, vertices, normals);
}