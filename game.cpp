#include "game.h"

#include <iostream>

Renderer *cubeRenderer;
Renderer *outlineRenderer;
LightSource *lightSource;

Game::Game(GLuint width, GLuint height)
	: State(GameState::MENU), Keys(), Width(width), Height(height) {

}

Game::~Game() {
	delete cubeRenderer;
	delete outlineRenderer;
	delete lightSource;
}

void Game::Init() {
	// Camera Init
	GameCamera = Camera(glm::vec3((24 / 2), 21.0f, 11.5f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -40.0f);
	glm::mat4 projection = glm::perspective(GameCamera.Zoom, (GLfloat)(Width / Height), 0.1f, 100.0f);

	// Test cube
	std::vector<GLfloat> vertices{
		// Position			//Normal
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f
	};

	// Need to adjust for level
	lightSource = new LightSource{
		glm::vec3((24 / 2) - 0.5f, 10.0f, -15.0f),
		//glm::vec3((6 / 2), 10.0f, -3.0f),
		glm::vec3(0.8f, 0.5f, 1.0f),
	};

	Shader testCube = ResourceManager::LoadShader("testCube", "shaders/simple3d.vs", "shaders/diffuse_only.frag");
	Shader outlineCube = ResourceManager::LoadShader("outlineCube", "shaders/outline.vs", "shaders/outline.frag", "shaders/outline.gs");
	ResourceManager::LoadShader("Debug", "shaders/outline.vs", "shaders/lamp.frag");

	cubeRenderer = new Renderer(testCube, vertices);
	outlineRenderer = new Renderer(outlineCube, vertices);

	testCube.Use().SetMatrix4("projection", projection);
	outlineCube.Use().SetMatrix4("projection", projection);

	GameLevel testLevel("testLevel", "levels/level_one.txt", 24, 30, cubeRenderer, outlineRenderer);
	Levels.push_back(testLevel);
}

void Game::Update(GLfloat dt) {
	GameLevel &level = Levels[CurrentLevel];
	if (level.HasTarget) {
		// find angle from turret to target, rotate towards
		glm::vec2 target(level.Target.x, level.Target.y);
		// adjust for origin position, move to render function
		for (auto &turret : level.Turrets) {
			glm::vec2 turretVec(turret.cubeObj.Position.x - 0.5f, turret.cubeObj.Position.z - 0.5f);
			glm::vec2 facing = turretVec - target;
			GLfloat angle = atan2(facing.y, facing.x);
			std::cout << "angle " << glm::degrees(angle) << std::endl;
			turret.cubeObj.Rotation = glm::degrees(-angle);
		}
	}
}

void Game::Render() {
	Levels[CurrentLevel].Draw(GameCamera.GetViewMatrix(), lightSource);
}

void Game::ProcessInput(GLfloat dt) {
	if (State == GameState::ACTIVE) {
		if (Keys[GLFW_KEY_Q]) {
			GameCamera.RotateLeft();
			Keys[GLFW_KEY_Q] = GL_FALSE;
		} else if (Keys[GLFW_KEY_E]) {
			GameCamera.RotateRight();
			Keys[GLFW_KEY_E] = GL_FALSE;
		}
	}
}

void Game::HandleClick(GLuint button, glm::vec2 position) {
	//std::cout << position.x << "--" << position.y << std::endl;
	GameLevel &level = Levels[CurrentLevel];
	glm::mat4 projection = glm::perspective(GameCamera.Zoom, (GLfloat)(Width / Height), 0.1f, 100.0f);
	
	glm::mat4 modelView = GameCamera.GetViewMatrix()*glm::mat4(0.5f);
	glm::vec4 viewport(0, 0, Width, Height);
	GLfloat winX = position.x;
	GLfloat winY = Height - position.y;
	GLfloat winZ;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	//std::cout << winX << " " << winY << " " << winZ << std::endl;
	//glm::vec3 rayStart = glm::unProject(startPoint, modelView, projection, viewport);
	glm::vec3 pos = glm::unProject(glm::vec3(winX, winY, winZ), modelView, projection, viewport);
	std::cout << (int)pos.x << " " << " " << (int)pos.z << std::endl;
	int index = ((int)pos.z) * (-1 *level.Width) + (int)pos.x;
	std::cout << "index: " << index << std::endl;
	Cube *cube = nullptr;
	if (index >= 0 && index < Levels[CurrentLevel].LevelCubes.size()) {
		cube = &Levels[CurrentLevel].LevelCubes[index];
		cube->cubeObj.Color = glm::vec3(0.2f, 0.2f, 0.61f);
	}
}

void Game::MoveCursor(double xPos, double yPos) {
	//std::cout << position.x << "--" << position.y << std::endl;
	GameLevel &level = Levels[CurrentLevel];
	glm::mat4 projection = glm::perspective(GameCamera.Zoom, (GLfloat)(Width / Height), 0.1f, 100.0f);

	glm::mat4 modelView = GameCamera.GetViewMatrix()*glm::mat4(0.5f);
	glm::vec4 viewport(0, 0, Width, Height);
	GLfloat winX =xPos;
	GLfloat winY = Height - yPos;
	GLfloat winZ;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	//std::cout << winX << " " << winY << " " << winZ << std::endl;
	//glm::vec3 rayStart = glm::unProject(startPoint, modelView, projection, viewport);
	glm::vec3 pos = glm::unProject(glm::vec3(winX, winY, winZ), modelView, projection, viewport);
	std::cout << (int)pos.x << " " << " " << (int)pos.z << std::endl;
	int index = ((int)pos.z) * (-1 * level.Width) + (int)pos.x;
	std::cout << "index: " << index << std::endl;
	if (index >= 0 && index < level.LevelCubes.size()) {
		level.Target = glm::vec2(pos.x, pos.z);
		level.HasTarget = true;
		//Levels[CurrentLevel].target->cubeObj.Color = glm::vec3(0.2f, 0.2f, 0.61f);
	} else {
		level.HasTarget = false;
	}
}

void Game::MoveCursor(glm::vec2 position) {
	std::cout << position.x << "--" << position.y << std::endl;
	glm::mat4 projection = glm::perspective(GameCamera.Zoom, (GLfloat)(Width / Height), 0.1f, 100.0f);

	glm::mat4 modelView = GameCamera.GetViewMatrix()*glm::mat4();
	glm::vec4 viewport(0, 0, Width, Height);
	GLfloat winX = position.x;
	GLfloat winY = Height - position.y;
	GLfloat winZ;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	std::cout << winX << " " << winY << " " << winZ << std::endl;
	//glm::vec3 rayStart = glm::unProject(startPoint, modelView, projection, viewport);
	glm::vec3 pos = glm::unProject(glm::vec3(winX, winY, winZ), modelView, projection, viewport) + glm::vec3(1.0f, 1.0f, -1.0f);
	std::cout << (int)pos.x << " " << " " << (int)pos.z << std::endl;
}

