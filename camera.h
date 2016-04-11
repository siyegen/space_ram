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

const GLfloat RightYaw = -155.0f;;
const GLfloat LeftYaw = -25.0f;

const GLfloat PanAmount = 10.0f;

enum class PanState {
	Center,
	Left,
	Right
};

class Camera {
public:
	// Attributes
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat Radius, ZRadius;
	GLfloat MinZ, MaxZ;

	PanState CurrentPanState = PanState::Center;

	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	GLboolean IsMoving = false;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	glm::mat4 GetViewMatrix();

	void Update(GLfloat dt);
	void RotateRight(); // Refers to Yaw
	void RotateLeft();
	void PanIn();
	void PanOut();
	void Center(glm::vec3 position);

private:
	void updateCameraVectors();
	void Rotate(PanState moveTo);
};
