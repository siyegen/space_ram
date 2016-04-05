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

	lightSource = new LightSource{
		glm::vec3((24 / 2) - 0.5f, 10.0f, -15.0f),
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
	int index = ((int)pos.z) * -24 + (int)pos.x;
	std::cout << "index: " << index << std::endl;
	Cube *cube = nullptr;
	if (index >= 0 && index <= Levels[CurrentLevel].LevelCubes.size()) {
		cube = &Levels[CurrentLevel].LevelCubes[index];
		cube->cubeObj.Color = glm::vec3(1.0f);
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

void Game::Render() {
	Levels[CurrentLevel].Draw(GameCamera.GetViewMatrix(), lightSource);
}
