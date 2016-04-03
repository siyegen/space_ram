#pragma once

#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <string>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "renderer.h"


enum class CubeState {
	Normal,
	Dangerous,
	Water,
};

struct Cube {
	Renderer &mainRenderer;
	Renderer &outlineRenderer;
	GameObject cubeObj;
	CubeState state;
	glm::vec2 coords; // row/col values
	glm::vec4 outlineColor;
};

class GameLevel {
public:
	GLuint Width, Height;
	std::string LevelName; // For rendering

	glm::vec3 CenterPoint;

	std::vector<Cube> LevelCubes;

	// This is ugly, but not sure how else to do it atm.
	GameLevel(std::string name, const GLchar *file, GLuint width, GLuint height, Renderer *firstRenderer, Renderer *effectRenderer);

	void Draw(glm::mat4 camera, LightSource *lightSource); // Cube will draw

private:
	void fromFile(const GLchar *file, GLuint width, GLuint height, Renderer *first, Renderer *effect);
	static CubeState getState(GLuint tile);
};

#endif
