#ifndef ENTITY_H
#define ENTITY_H

#include "mesh.h"
#include "glm\glm.hpp"
#include <memory>

class Entity
{
public:
	Entity(std::shared_ptr<Mesh> mesh);
	~Entity();

	void render();
	glm::mat4 getTransformations();

	void setUnitary();
	void translateToOrigin();
	
	//Setters
	void setScale(glm::vec3 scale);
	void setPosition(glm::vec3 pos);
	void setRotation(glm::vec3 rot);
	void setRotationd(glm::vec3 rot);

	//Getters
	glm::vec3 getScale();
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getRotationd();

	//Funcs
	void scale(glm::vec3 s);
	void translate(glm::vec3 t);
	void rotated(glm::vec3 r);
	void rotate(glm::vec3 r);

	BBox getMeshBBox();
	BBox getBBox();
	std::shared_ptr<Mesh> getMeshPtr();


private:
	std::shared_ptr<Mesh> _mesh;
	//Mesh* _mesh;
	glm::mat4 _transformations;
	glm::vec3 _position, _scale, _rotation;
	bool _updated;

	void computetransformationMatrix();
};

#endif // ENTITY_H
