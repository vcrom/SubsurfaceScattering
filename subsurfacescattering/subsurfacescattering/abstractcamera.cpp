#include "abstractcamera.h"

//glm mastrix transformations
#include "glm/gtc/matrix_transform.hpp"


glm::vec3 AbstractCamera::global_up_vector_ = glm::vec3(0, 1, 0);

AbstractCamera::AbstractCamera(void)
{
	z_near_ = 0.1f;
	z_far_ = 1000;
}


AbstractCamera::~AbstractCamera(void)
{
}

//#include <iostream>
void AbstractCamera::setupProjection(const float fovy, const float aspRatio, const float nr, const float fr)
{
	//std::cout << "Near: " << nr <<" Far:" << fr << std::endl;
	projection_matrix_ = glm::perspective(fovy, aspRatio, nr, fr);
	z_near_ = nr;
	z_far_ = fr;
	fov_ = fovy;
	aspect_ratio_ = aspRatio;
}

const glm::mat4 AbstractCamera::getViewMatrix() const {
	return view_matrix_;
}

const glm::mat4 AbstractCamera::getProjectionMatrix() const
{
	return projection_matrix_;
}

const glm::vec3 AbstractCamera::getPosition() const
{
	return position_;
}

void AbstractCamera::setPosition(const glm::vec3& p)
{
	position_ = p;
}

float AbstractCamera::getFOV() const
{
	return fov_;
}

void AbstractCamera::setFOV(const float fovInRadians)
{
	fov_ = fovInRadians;
	projection_matrix_ = glm::perspective(fovInRadians, aspect_ratio_, z_near_, z_far_);
}

float AbstractCamera::getAspectRatio() const
{
	return aspect_ratio_;
}

void AbstractCamera::setAspectRatio(const float aspRatio)
{
	aspect_ratio_ = aspRatio;
}

float AbstractCamera::getZfar()
{
	return z_far_;
}

float AbstractCamera::getZnear()
{
	return z_near_;
}

void AbstractCamera::rotated(const float y, const float p, const float r)
{
	yaw_ = glm::radians(y);
	pitch_ = glm::radians(p);
	roll_ = glm::radians(r);
	update();
}

void AbstractCamera::rotater(const float y, const float p, const float r)
{
	yaw_ = y;
	pitch_ = p;
	roll_ = r;
	update();
}


glm::vec3 AbstractCamera::getYawPitchRoll()
{
	return glm::vec3(yaw_, pitch_, roll_);
}