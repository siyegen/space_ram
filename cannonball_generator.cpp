#include "cannonball_generator.h"

CannonBallGenerator::CannonBallGenerator(Renderer *renderer, GLuint amount) {
	cannonBallRenderer = renderer;
	totalSize = amount;
	cannonBalls.reserve(amount);
}

void CannonBallGenerator::Update(GLfloat dt) {
}

void CannonBallGenerator::Draw(glm::mat4 camera) {
	for (auto &ball : cannonBalls) {
		// update / set color here
		// call draw, go to next
		if (ball.IsActive) {
			ball.CubeObj.Draw(*cannonBallRenderer, camera);
		}
	}
}

void CannonBallGenerator::Fire(GLuint amount, glm::vec3 origin) {
}

GLuint CannonBallGenerator::getFirstReadyCannonBall() {
	return GLuint();
}

void CannonBallGenerator::resetCannonBall() {
}

void CannonBallGenerator::init() {
	for (GLuint i = 0; i < totalSize; i++) {
		// create cannonballs
		cannonBalls.push_back(CannonBall());
	}
}
