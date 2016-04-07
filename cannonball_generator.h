#pragma once

#ifndef CANONBALLGENERATOR_H
#define CANONBALLGENERATOR_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "renderer.h"


struct CannonBall {
	GameObject CubeObj;
	GLboolean IsActive;

	CannonBall() : CubeObj(), IsActive() {};
};

class CannonBallGenerator {
public:

	CannonBallGenerator(Renderer *renderer, GLuint amount);

	void Update(GLfloat dt);
	void Draw(glm::mat4 camera);
	void Fire(GLuint amount, glm::vec3 origin);
private:
	std::vector<CannonBall> cannonBalls;
	Renderer *cannonBallRenderer;
	GLuint totalSize;

	GLuint getFirstReadyCannonBall();
	void resetCannonBall();

	void init();
};

#endif // !CANONBALLGENERATOR_H
