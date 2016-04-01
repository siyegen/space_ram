#pragma once

#pragma once

#include <vector>

// GL Includes

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_Movement {
	ROTATE_LEFT, // Rotate while still facing a certain point, so still yaw
	ROTATE_RIGHT,
	MOVE_OUT,
	MOVE_IN
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = -15.0f;
const GLfloat SPEED = 7.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera {
public:
	// Attributes
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	glm::mat4 GetViewMatrix();

	// These probably don't belong here
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(GLfloat yoffset);

private:
	void updateCameraVectors();
};