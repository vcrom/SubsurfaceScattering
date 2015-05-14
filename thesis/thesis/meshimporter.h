#ifndef MESHIMPORTER_H
#define MESHIMPORTER_H

#include "mesh.h"
class MeshImporter
{
public:
	static Mesh* importMeshFromFile(const std::string& path);
private:
	//static void getVertices();
};

#endif // MESHIMPORTER_H
