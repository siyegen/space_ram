#pragma once

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera.h"
#include "resource_manager.h"
#include "renderer.h"
#include "game_object.h"
#include "game_level.h"
#include "cannonball_generator.h"

enum class GameState {
	ACTIVE,
	MENU,
	WIN
};

class Game {
public:
	// Game state
	GameState  State;
	GLboolean  Keys[1024];
	GLuint	   Width, Height;

	GLuint firingFrom = 0;

	std::vector<GameLevel> Levels;
	GLuint CurrentLevel = 0;

	CannonBallGenerator *Cannon = nullptr;

	Camera GameCamera;

	Game(GLuint width, GLuint height);
	~Game();

	bool MouseHeld = false;

	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void HandleClick(GLuint button, double xPos, double yPos);
	void MoveCursor(double xPos, double yPos);
	void Update(GLfloat dt);
	void Render();
private:
	glm::vec3 screenToWorld(double xPos, double yPos);
};
#endif
