#ifndef MESHIMPORTER_H
#define MESHIMPORTER_H

#include <memory>
#include "mesh.h"

class MeshImporter
{
public:
	static std::shared_ptr<Mesh> importMeshFromFile(const std::string& path, bool info = false);
private:
};

#endif // MESHIMPORTER_H
