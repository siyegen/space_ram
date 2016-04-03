#pragma once

#ifndef GAME_H
#define GAME_H

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera.h"
#include "resource_manager.h"
#include "renderer.h"
#include "game_object.h"
#include "game_level.h"


enum class GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game {
public:
	// Game state
	GameState  State;
	GLboolean  Keys[1024];
	GLuint	   Width, Height;

	std::vector<GameLevel> Levels;
	GLuint CurrentLevel = 0;

	Camera GameCamera;

	Game(GLuint width, GLuint height);
	~Game();

	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};
#endif