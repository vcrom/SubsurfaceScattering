#include "entity.h"
#include "glm/gtc/matrix_transform.hpp"

/// <summary>
/// Initializes a new instance of the <see cref="Entity"/> class.
/// </summary>
/// <param name="mesh"> pointer to a mesh used to represent the entity.</param>
Entity::Entity(std::shared_ptr<Mesh> mesh) : _mesh(mesh)
{
	_transformations = glm::mat4(1);
	_position = glm::vec3(0);
	_scale = glm::vec3(1);
	_rotation = glm::vec3(0);
	_updated = false;
}

Entity::~Entity()
{
	//delete _mesh;
	_mesh.reset();
}

/// <summary>
/// Renders this entity mesh.
/// </summary>
void Entity::render()
{
	assert(_mesh);
	_mesh->render();
}

/// <summary>
/// Scales the entity's mesh as its longest axis(x, y, z) measures 1, alse kepping the proportions.
/// </summary>
void Entity::setUnitary()
{
	BBox box = getMeshBBox();
	glm::vec3 dims = box.getMax() - box.getMin();
	float max_dim = glm::max(dims.x, dims.y);
	max_dim = glm::max(max_dim, dims.z);
	setScale(glm::vec3(1.0f / max_dim));
	_updated = false;
}

/// <summary>
/// Sets the entitys scale.
/// </summary>
/// <param name="scale">The scale in each axis (x, y, z).</param>
void Entity::setScale(glm::vec3 scale)
{
	_scale = scale;
	_updated = false;
}

/// <summary>
/// Sets the entitys position.
/// </summary>
/// <param name="pos">The position.</param>
void Entity::setPosition(glm::vec3 pos)
{
	_position = pos;
	_updated = false;
}

/// <summary>
/// Sets the entity rotation.
/// </summary>
/// <param name="rot">The rotation in radiants.</param>
void Entity::setRotation(glm::vec3 rot)
{
	_rotation = rot;
	_updated = false;
}

/// <summary>
/// Sets the entity rotation.
/// </summary>
/// <param name="rot">The rotatin in degrees</param>
void Entity::setRotationd(glm::vec3 rot)
{
	setRotation(glm::radians(rot));
}

/// <summary>
/// Scales the entity accrding to the param.
/// </summary>
/// <param name="s">The scale factor.</param>
void Entity::scale(glm::vec3 s)
{
	_scale *= s;
	_updated = false;
}

/// <summary>
/// Translates the entity according to the param.
/// </summary>
/// <param name="t">The translation.</param>
void Entity::translate(glm::vec3 t)
{
	_position += t;
	_updated = false;
}

/// <summary>
/// Rotates the entity an amount of radiants defined by the param.
/// </summary>
/// <param name="r">The rotation in radiants.</param>
void Entity::rotate(glm::vec3 r)
{
	_rotation *= r;
	_updated = false;
}

/// <summary>
/// Rotates the entity an amount of degrees defined by the param.
/// </summary>
/// <param name="r">The rotation in degrees.</param>
void Entity::rotated(glm::vec3 r)
{
	rotate(glm::radians(r));
}

/// <summary>
/// Gets the mesh axis aligned bounding box.
/// </summary>
/// <returns>Returns the axis aligned bounding box of the mesh that represents the entity</returns>
BBox Entity::getMeshBBox()
{
	if (!_mesh) return BBox(glm::vec3(-0.5), glm::vec3(0.5));
	return _mesh->getBBox(); 
}

/// <summary>
/// Gets the bouinding box of the entity after being translated, rotated and scaled.
/// </summary>
/// <returns>Returns the mesh bounding box after being transformed with the Model matrix</returns>
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
/// Gets the pointer to teh mesh which represents the entity.
/// </summary>
/// <returns></returns>
std::shared_ptr<Mesh> Entity::getMeshPtr()
{
	return _mesh;
}

/// <summary>
/// Gets the model transformation matrix.
/// </summary>
/// <returns></returns>
glm::mat4 Entity::getTransformations()
{
	if (!_updated) computetransformationMatrix();
	return _transformations;
}

/// <summary>
/// Compute the transformations the applied to the mesh according to the transformations of the entity..
/// </summary>
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