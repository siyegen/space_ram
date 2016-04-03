#pragma once

#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <string>

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
};

class GameLevel {
public:
	GLuint Width, Height;
	std::string LevelName; // For rendering

	glm::vec3 CenterPoint;

	std::vector<Cube> LevelCubes;

	// This is ugly, but not sure how else to do it atm.
	GameLevel(std::string name, const std::string &level, GLuint width, GLuint height, Renderer &firstRenderer, Renderer &effectRenderer);
	GameLevel(std::string name);

	void Draw(glm::mat4 camera, LightSource *lightSource); // Cube will draw
};

#endif
