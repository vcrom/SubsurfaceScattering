#ifndef ABSTRACTCAMERA_H
#define ABSTRACTCAMERA_H

#include "glm/glm.hpp"

class AbstractCamera
{
public:
	AbstractCamera(void);
	virtual ~AbstractCamera(void);

	void setupProjection(const float fovy, const float aspectRatio, const float near = 0.1f, const float far = 1000.0f);

	virtual void update() = 0;
	virtual void rotated(const float yaw, const float pitch, const float roll);
	virtual void rotater(const float yaw, const float pitch, const float roll);

	const glm::mat4 getViewMatrix() const;
	const glm::mat4 getProjectionMatrix() const;

	void setPosition(const glm::vec3& v);
	const glm::vec3 getPosition() const;


	void setFOV(const float fovInRadians);
	float getFOV() const;
	float getAspectRatio() const;
	void setAspectRatio(const float aspRatio);
	float getZfar();
	float getZnear();
	glm::vec3 getYawPitchRoll();


protected:
	float yaw_, pitch_, roll_, fov_, aspect_ratio_, z_near_, z_far_;
	static glm::vec3 global_up_vector_;
	glm::vec3 look_vector_;
	glm::vec3 up_vector_;
	glm::vec3 right_vector_;
	glm::vec3 position_;
	glm::mat4 view_matrix_;
	glm::mat4 projection_matrix_;
};

#endif // ABSTRACTCAMERA_H
