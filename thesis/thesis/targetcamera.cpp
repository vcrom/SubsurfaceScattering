#include "targetcamera.h"

//#include <iostream>
#include <algorithm>

//#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>


TargetCamera::TargetCamera()
	: AbstractCamera()
{
	initialize();
}

TargetCamera::~TargetCamera()
{

}

void TargetCamera::initialize()
{
	right_vector_ = glm::vec3(1, 0, 0);
	up_vector_ = glm::vec3(0, 1, 0);
	look_vector_ = glm::vec3(0, 0, -1);
	min_distance_to_target_ = 1;
	max_distance_to_target_ = 10;
	x_axis_angle_ = 0;
	y_axis_angle_ = 0;
	distance_to_target_ = 0;
}

void TargetCamera::initFromBBox(BBox bbox)
{
	initialize();
	target_ = bbox.getCenter();
	float rad = bbox.getCircumRadius();
	min_distance_to_target_ = 0.7f*rad;
	max_distance_to_target_ = 4 * rad;
	if (distance_to_target_ == 0) distance_to_target_ = 2 * rad;
	//rX = 0;
	//rY = 0;
	z_near_ = 0.1f*rad;
	z_far_ = 15.0f * rad;
	//CAbstractCamera::Rotate(0, 0, 0);
	setupProjection(fov_, aspect_ratio_, z_near_, z_far_);
	rotate(0, 0);
	update();
}

void TargetCamera::updateProjection(const float fovy, const float aspRatio)
{
	fov_ = fovy;
	aspect_ratio_ = aspRatio;
	setupProjection(fovy, aspRatio, z_near_, z_far_);
}


void TargetCamera::update() {
	glm::mat4 rotation_matrix_transform = glm::yawPitchRoll(yaw_, pitch_, 0.0f);
	glm::vec3 vector_from_target = glm::vec3(0, 0, distance_to_target_);
	vector_from_target = glm::vec3(rotation_matrix_transform*glm::vec4(vector_from_target, 0.0f));
	position_ = target_ + vector_from_target;
	look_vector_ = glm::normalize(target_ - position_);
	up_vector_ = glm::vec3(rotation_matrix_transform*glm::vec4(global_up_vector_, 0.0f));
	right_vector_ = glm::cross(look_vector_, up_vector_);
	view_matrix_ = glm::lookAt(position_, target_, up_vector_);
}

void TargetCamera::setTarget(const glm::vec3 tgt) {
	target_ = tgt;
	distance_to_target_ = glm::distance(position_, target_);
	distance_to_target_ = std::max(min_distance_to_target_, std::min(distance_to_target_, max_distance_to_target_));
}

const glm::vec3 TargetCamera::getTarget() const {
	return target_;
}

void TargetCamera::rotate(const float yaw, const float pitch) {
	y_axis_angle_ += yaw;
	x_axis_angle_ += pitch;
	y_axis_angle_ = std::min(std::max(y_axis_angle_, min_y_angle_), max_y_angle_);

	AbstractCamera::rotated(x_axis_angle_, y_axis_angle_, roll_);
}

void TargetCamera::pan(const float dx, const float dy) {
	glm::vec3 X = right_vector_*dx;
	glm::vec3 Y = up_vector_*dy;
	position_ += X + Y;
	target_ += X + Y;
	update();
}


void TargetCamera::zoom(const float amount) {
	position_ += look_vector_*amount;
	distance_to_target_ = glm::distance(position_, target_);
	distance_to_target_ = std::max(min_distance_to_target_, std::min(distance_to_target_, max_distance_to_target_));
	//std::cout << "Dist: " << distance << std::endl;
	update();
}

void TargetCamera::move(const float dx, const float dy) {
	glm::vec3 X = right_vector_*dx;
	glm::vec3 Y = look_vector_*dy;
	position_ += X + Y;
	target_ += X + Y;
	update();
}
