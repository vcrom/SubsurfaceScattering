#ifndef TARGETCAMERA_H
#define TARGETCAMERA_H

#include "abstractcamera.h"

class TargetCamera : public AbstractCamera
{
public:
	TargetCamera();
	~TargetCamera();

	void update();
	void rotate(const float yaw, const float pitch);
	//void initFromBBox(CBBox bbox);
	void updateProjection(const float fovy, const float aspRatio);

	const glm::vec3 getTarget() const;

	void pan(const float dx, const float dy);
	void zoom(const float amount);
	void move(const float dx, const float dz);
	void setTarget(const glm::vec3 tgt);

protected:

	glm::vec3 target_;

	float min_y_angle_, max_y_angle_, x_axis_angle_, y_axis_angle_;
	float distance_to_target_;
	float min_distance_to_target_, max_distance_to_target_;
};

#endif // TARGETCAMERA_H
