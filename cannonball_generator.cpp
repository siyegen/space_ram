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
			ball.Velocity -= glm::vec3(0.0f, 10.0f, 0.0f)*dt;
			ball.CubeObj.Position += ball.Velocity*dt;
			if (ball.CubeObj.Position.y < -2.0f) {
				ball.IsActive = false;
			}
		}
	}
}

void CannonBallGenerator::Draw(glm::mat4 camera, LightSource *lightSource) {
	for (auto &ball : cannonBalls) {
		// update / set color here
		// call draw, go to next
		if (ball.IsActive) {
			ball.CubeObj.Draw(*cannonBallRenderer, camera, lightSource, nullptr, false);
		}
	}
}

void CannonBallGenerator::Fire(GLuint amount, glm::vec3 origin, GLfloat launchRotation, glm::vec3 target) {
	// For now only fire 1
	std::cout << "Firing! ";
	amount = 1;
	CannonBall &current = cannonBalls[getFirstReadyCannonBall()];
	resetCannonBall(current, origin);
	current.CubeObj.Rotation = 75.0f;
	std::cout << "from " << current.CubeObj.Position.x << std::endl;
	std::cout << "active? " << launchRotation << std::endl;

	glm::vec3 vv(0.0f, 20.0f, 20.0f);
	GLfloat g = 10.0f;
	GLfloat v = glm::sqrt(glm::pow(vv.x, 2) + glm::pow(vv.z, 2));
	GLfloat d = target.z - origin.z; // dist out
	GLfloat angle = 0.5f * asin((g*d)/pow(v,2));
	std::cout << "angle " << angle << std::endl;
	angle = glm::radians(angle);
	launchRotation = launchRotation * glm::sign(target.x - origin.x);
	current.Velocity = glm::vec3(1.0f*glm::radians(launchRotation), current.Velocity.y, current.Velocity.z);
	//current.Velocity = glm::vec3(vv.x, glm::sin(angle)*vv.y, -glm::cos(angle)*vv.z);
	//current.Velocity = glm::vec3(0.0f, 20.0f*glm::sin(glm::radians(angle)), -20.0f*glm::cos(glm::radians(angle)));
}

GLuint CannonBallGenerator::getFirstReadyCannonBall() {
	for (GLuint i = currentIndex; i < totalSize; i++) {
		if (!cannonBalls[i].IsActive) {
			currentIndex = i+1;
			return i;
		}
	}
	// Bad, but overwrite first one if we can't find any
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
	cannonball.Velocity = glm::vec3(0.0f, 11.0f, -8.0f);
}

