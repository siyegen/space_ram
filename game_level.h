#pragma once

#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <string>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "renderer.h"


const int NumberOfGroundColors = 4;

const glm::vec3 GroundColors[NumberOfGroundColors] {
	glm::vec3(0.31f, 1.0f, 0.31f),
	glm::vec3(0.39f, 1.0f, 0.31f),
	glm::vec3(0.31f, 0.9f, 0.31f),
	glm::vec3(0.31f, 1.0f, 0.39f)
};

enum class CubeState {
	Normal,
	Enemy,
	Water,
	Turret,
};

struct Cube {
	Renderer &MainRenderer;
	Renderer &OutlineRenderer;
	GameObject CubeObj;
	CubeState State;
	glm::vec2 Coords; // row/col values
	glm::vec4 OutlineColor;
	bool IsAlive; // Leaky, most cubes don't care about this
};

class GameLevel {
public:
	GLuint Width, Height;
	std::string LevelName; // For rendering

	unsigned int ExpectedNumberOfCubes = 0;

	glm::vec3 CenterPoint;

	std::vector<Cube> LevelCubes;
	std::vector<Cube> Turrets;
	std::vector<Cube> Enemies;

	unsigned int NumberKilled = 0;

	glm::vec2 Target;
	bool HasTarget = false;

	// This is ugly, but not sure how else to do it atm.
	GameLevel(std::string name, const GLchar *file, GLuint width, GLuint height, Renderer *firstRenderer, Renderer *effectRenderer);

	void Draw(glm::mat4 camera, LightSource *lightSource);
	// Takes float x/y position and finds the Cube that contains that
	// Or nullptr if none.
	// Other options: pass in out ref, or return Nonetype cube;
	Cube* CubeFromPosition(glm::vec2 position);

private:
	void fromFile(const GLchar *file, GLuint width, GLuint height, Renderer *first, Renderer *effect);
	static CubeState getState(GLuint tile);
	static GLfloat getHeight(CubeState state, GLfloat minVal, GLfloat maxVal);
	static const glm::vec3* randomGroundColor();
};

#endif
