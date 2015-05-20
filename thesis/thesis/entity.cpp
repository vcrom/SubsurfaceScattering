#include "entity.h"
#include "glm/gtc/matrix_transform.hpp"

Entity::Entity(Mesh* mesh) : _mesh(mesh)
{
	_transformations = glm::mat4(1);
	_position = glm::vec3(0);
	_scale = glm::vec3(1);
	_rotation = glm::vec3(0);
	_updated = false;
}

Entity::~Entity()
{

}

void Entity::render()
{
	_mesh->render();
}

void Entity::setUnitary()
{
	BBox box = getMeshBBox();
	glm::vec3 dims = box.getMax() - box.getMin();
	float max_dim = glm::max(dims.x, dims.y);
	max_dim = glm::max(max_dim, dims.z);
	setScale(glm::vec3(1.0f / max_dim));
	_updated = false;
}

void Entity::setScale(glm::vec3 scale)
{
	_scale = scale;
	_updated = false;
}

void Entity::setPosition(glm::vec3 pos)
{
	_position = pos;
	_updated = false;
}

void Entity::setRotation(glm::vec3 rot)
{
	_rotation = rot;
	_updated = false;
}

void Entity::setRotationd(glm::vec3 rot)
{
	setRotation(glm::radians(rot));
}

void Entity::scale(glm::vec3 s)
{
	_scale *= s;
	_updated = false;
}

void Entity::translate(glm::vec3 t)
{
	_position += t;
	_updated = false;
}

void Entity::rotate(glm::vec3 r)
{
	_rotation *= r;
	_updated = false;
}

void Entity::rotated(glm::vec3 r)
{
	rotate(glm::radians(r));
}

BBox Entity::getMeshBBox()
{
	return _mesh->getBBox(); 
}

BBox Entity::getBBox()
{
	BBox box = getMeshBBox();
	glm::vec4 p1 = glm::vec4(box.getMin(), 1);
	glm::vec4 p2 = glm::vec4(box.getMax(), 1);
	glm::mat4 trans = getTransformations();
	p1 = trans*p1;
	p2 = trans*p2;
	return BBox(glm::vec3(p1.x, p1.y, p1.z), glm::vec3(p2.x, p2.y, p2.z));
}

/// <summary>
/// Gets the mocdel transformation matrix.
/// </summary>
/// <returns></returns>
glm::mat4 Entity::getTransformations()
{
	if (!_updated) computetransformationMatrix();
	return _transformations;
}

void Entity::computetransformationMatrix()
{
	_transformations = glm::mat4(1);
	glm::vec3 box_center = _mesh->getBBox().getCenter();

	_transformations = glm::translate(_transformations, _position);
	_transformations = glm::rotate(_transformations, _rotation.x, glm::vec3(1,0,0));
	_transformations = glm::rotate(_transformations, _rotation.y, glm::vec3(0, 1, 0));
	_transformations = glm::rotate(_transformations, _rotation.z, glm::vec3(0, 0, 1));
	_transformations = glm::scale(_transformations, _scale);
	_transformations = glm::translate(_transformations, -box_center);

	_updated = true;
}