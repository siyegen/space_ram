#pragma once

#ifndef CANONBALLGENERATOR_H
#define CANONBALLGENERATOR_H

#include <vector>
#include <ctime>
#include <random>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "renderer.h"


struct CannonBall {
	GameObject CubeObj;
	glm::vec3 Velocity;
	glm::vec3 Grav;
	bool IsActive;

	CannonBall() : CubeObj(), Velocity(), Grav(), IsActive(false) {};
};

class CannonBallGenerator {
public:

	CannonBallGenerator(Renderer *renderer, GLuint amount);

	void Update(GLfloat dt);
	void Draw(glm::mat4 camera, LightSource *lightSource);
	void Fire(GLuint amount, glm::vec3 origin, GLfloat launchRotation, glm::vec3 target);
	bool CheckCollision(const GameObject &target);
private:
	std::vector<CannonBall> cannonBalls;
	Renderer *cannonBallRenderer;
	GLuint totalSize;
	GLuint currentIndex = 0;

	GLuint getFirstReadyCannonBall();
	void resetCannonBall(CannonBall &cannonball, glm::vec3 position);
};

#endif
