#ifndef MESHIMPORTER_H
#define MESHIMPORTER_H

#include <memory>
#include "mesh.h"

class MeshImporter
{
public:
	static std::shared_ptr<Mesh> importMeshFromFile(const std::string& path);
private:
	//static void getVertices();
};

#endif // MESHIMPORTER_H
