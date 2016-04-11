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
#include "sound_manager.h"
#include "renderer.h"
#include "game_object.h"
#include "game_level.h"
#include "cannonball_generator.h"
#include "text_renderer.h"
#include "text.h"


struct LevelSelectHud {
	glm::vec2 Position;
	std::string Display;
	GLfloat Size;
	bool Beaten = false;
	LevelSelectHud(glm::vec2 position, std::string display, GLfloat size, bool beaten) {
		Position = position; Display = display; Size = size; Beaten = beaten;
	};
};

enum class GameState {
	ACTIVE,
	MENU,
	WIN
};

class Game {
public:
	// Game state
	GameState State;
	GLboolean Keys[1024];
	GLuint Width, Height;

	GLuint FiringFrom = 0;

	std::vector<GameLevel> Levels;
	GLuint CurrentLevel = 0;

	std::vector<LevelSelectHud> LevelSelect;
	GLuint CurrentSelection = 0;
	glm::vec2 SelectCursor;

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
	bool CheckHit();
private:
	glm::vec3 screenToWorld(double xPos, double yPos);
	void StartLevel(int level);
};

#endif
