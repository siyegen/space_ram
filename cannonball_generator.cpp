#include "cannonball_generator.h"

#include <iostream>

CannonBallGenerator::CannonBallGenerator(Renderer *renderer, GLuint amount) {
	cannonBallRenderer = renderer;
	totalSize = amount;
	cannonBalls.reserve(amount);

	srand(static_cast <unsigned> (time(0)));
	for (GLuint i = 0; i < totalSize; i++) {
		cannonBalls.push_back(CannonBall());
	}
	std::cout << "size of balls " << cannonBalls.size() << std::endl;
}

void CannonBallGenerator::Update(GLfloat dt) {
	for (auto &ball : cannonBalls) {
		if (ball.IsActive) {
			ball.Velocity -= ball.Grav*dt;
			ball.CubeObj.Position += ball.Velocity*dt;
			if (ball.CubeObj.Position.y < -0.1f) {
				ball.IsActive = false;
				std::cout << "killed ball" << std::endl;
			}
		}
	}
}

void CannonBallGenerator::Draw(glm::mat4 camera, LightSource *lightSource) {
	for (auto &ball : cannonBalls) {
		if (ball.IsActive) {
			ball.CubeObj.Draw(*cannonBallRenderer, camera, lightSource, nullptr, false);
		}
	}
}

void CannonBallGenerator::Fire(GLuint amount, glm::vec3 origin, GLfloat launchRotation, glm::vec3 target) {
	std::cout << "Firing! ";
	amount = 1;
	CannonBall &current = cannonBalls[getFirstReadyCannonBall()];
	resetCannonBall(current, origin);
	// Update "ball" to match direction of the turret
	current.CubeObj.Rotation = launchRotation;

	target = glm::vec3(target.x - 0.5f, target.y, target.z + 0.5f);

	glm::vec3 fireVector = target - origin;
	GLfloat dist = glm::sqrt(fireVector.x * fireVector.x + fireVector.z * fireVector.z);
	fireVector = glm::normalize(fireVector);
	fireVector.y = glm::radians(78.0f);
	fireVector *= 7.0f; // Speed

	GLfloat vx = glm::sqrt(fireVector.x*fireVector.x + fireVector.z *fireVector.z);
	GLfloat vy = fireVector.y;

	GLfloat t = dist / vx;
	GLfloat y = target.y - origin.y;
	GLfloat g = (2.0f * (y - vy *t)) / (t*t);

	current.Velocity = fireVector;
	current.Grav = glm::vec3(0.0f, -g, 0.0f);
}

bool CannonBallGenerator::CheckCollision(const Cube &target) {
	for (auto &ball : cannonBalls) {
		glm::vec3 fireLine = target.CubeObj.Position - ball.CubeObj.Position;
		GLfloat dist = glm::abs(glm::sqrt(fireLine.x * fireLine.x + fireLine.z * fireLine.z));
		if (ball.IsActive && dist <= 2.0f) { // small filter
			std::cout << "checking collision" << std::endl;
			glm::vec3 tPos = target.CubeObj.Position;
			glm::vec3 bPos = ball.CubeObj.Position;
			if (glm::abs(tPos.x - bPos.x) < 0.5f + 0.25f) { // x half size
				if (glm::abs(tPos.y - bPos.y) < 0.5f + 0.25f) { // y
					if (glm::abs(tPos.z - bPos.z) < 0.5f + 0.25f) { // z
						ball.IsActive = false;
						return true;
					}
				}
			}
		}
	}
	return false;
}

GLuint CannonBallGenerator::getFirstReadyCannonBall() {
	for (GLuint i = currentIndex; i < totalSize; i++) {
		if (!cannonBalls[i].IsActive) {
			currentIndex = i+1;
			return i;
		}
	}
	// Bad, but overwrite first one if we can't find any
	std::cout << "WARN::OVERWRITING_ACTIVE_CANNON_BALL" << std::endl;
	currentIndex = 0;
	return 0;
}

void CannonBallGenerator::resetCannonBall(CannonBall &cannonball, glm::vec3 position) {
	cannonball.IsActive = true;
	GLfloat red = 0.6f + (rand() / (RAND_MAX / (1.0f - 0.6f)));
	red = ((int)(red *100.0f)) / 100.0f; // 2 dec places
	GLfloat green = 0.1f + (rand() / (RAND_MAX / (0.5f - 0.1f)));
	green = ((int)(green *100.0f)) / 100.0f; // 2 dec places
	cannonball.CubeObj.Color = glm::vec3(red, green, 0.1f);
	cannonball.CubeObj.Position = position;
	cannonball.CubeObj.Scale = 0.5f;
}
