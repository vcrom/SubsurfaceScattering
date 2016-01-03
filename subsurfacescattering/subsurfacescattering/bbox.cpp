#include "bbox.h"

BBox::BBox()
{
	_limits = std::vector<glm::vec3>(2);
}

BBox::BBox(const glm::vec3 &minPoint, const glm::vec3 &maxPoint)
{
	_limits = {minPoint, maxPoint};
}

BBox::~BBox()
{
}

void BBox::set(const glm::vec3 &minPoint, const glm::vec3 &maxPoint)
{
	_limits = { minPoint, maxPoint };
}

std::vector<glm::vec3> BBox::getAllBoxPoints() const
{
	glm::vec3 min = _limits[0];
	glm::vec3 max = _limits[1];
	std::vector<glm::vec3> points{
		glm::vec3(min.x, min.y, min.z),
		glm::vec3(min.x, min.y, max.z),
		glm::vec3(min.x, max.y, min.z),
		glm::vec3(min.x, max.y, max.z),
		glm::vec3(max.x, min.y, min.z),
		glm::vec3(max.x, min.y, max.z),
		glm::vec3(max.x, max.y, min.z),
		glm::vec3(max.x, max.y, max.z)
	};
	return points;
}

glm::vec3 BBox::getCenter() const
{
	glm::vec3 center = glm::vec3(
		(_limits[0].x + _limits[1].x) / 2.f,
		(_limits[0].y + _limits[1].y) / 2.f,
		(_limits[0].z + _limits[1].z) / 2.f);

	return center;
}

float BBox::getCircumRadius() const
{
	glm::vec3 dist = _limits[1] - _limits[0];
	return glm::length(dist) / 2.f;
}

glm::vec3 BBox::getMin() const
{
	return _limits[0];
}

glm::vec3 BBox::getMax() const
{
	return _limits[1];
}

std::vector<glm::vec3> BBox::getMinMax() const
{
	return _limits;
}