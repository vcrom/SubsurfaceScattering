#ifndef BBOX_H
#define BBOX_H

#include <vector>
#include "glm\glm.hpp"
#include "glm/gtx/string_cast.hpp"

class BBox
{

public:
	BBox();
	BBox(const glm::vec3 &minPoint, const glm::vec3 &maxPoint);
	~BBox();

	friend std::ostream& operator<<(std::ostream& out, const BBox& box)
	{
		std::vector<glm::vec3> lim = box.getMinMax();
		out << "BBox ( min: " << glm::to_string(lim[0]) << ", max: " << glm::to_string(lim[1]) << ")";
		return out;
	}

	void set(const glm::vec3 &minPoint, const glm::vec3 &maxPoint);

	std::vector<glm::vec3> getAllBoxPoints() const;
	glm::vec3 getCenter() const;
	float getCircumRadius() const;
	glm::vec3 getMin() const;
	glm::vec3 getMax() const;
	std::vector<glm::vec3> getMinMax() const;

private:
	std::vector<glm::vec3> _limits;
	
};

#endif // BBOX_H
