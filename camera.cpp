#include "Camera.h"

#include <iostream>

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM) {
	Position = position;
	Radius = position.z + 5.0f;
	ZRadius = Radius / 2.0f;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Update(GLfloat dt) {
}

void Camera::RotateRight() {
	std::cout << "Right" << std::endl;
	Rotate(PanState::Right);
}

void Camera::RotateLeft() {
	std::cout << "Left" << std::endl;
	Rotate(PanState::Left);
}

void Camera::Rotate(PanState moveTo) {
	switch (CurrentPanState) {
	case PanState::Center:
		if (moveTo == PanState::Left) {
			Position = Position + glm::vec3(-Radius, 0.0f, -ZRadius);
			Yaw = LeftYaw;
			CurrentPanState = PanState::Left;
		} else {
			Position = Position + glm::vec3(Radius, 0.0f, -ZRadius);
			Yaw = RightYaw;
			CurrentPanState = PanState::Right;
		}
		break;
	case PanState::Left:
		if (moveTo == PanState::Right) {
			Position = Position + glm::vec3(Radius, 0.0f, ZRadius);
			Yaw = YAW;
			CurrentPanState = PanState::Center;
		}
		break;
	case PanState::Right:
		if (moveTo == PanState::Left) {
			Position = Position + glm::vec3(-Radius, 0.0f, ZRadius);
			Yaw = YAW;
			CurrentPanState = PanState::Center;
		}
		break;
	default:
		break;
	}
	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}


