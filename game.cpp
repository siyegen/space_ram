#include "game.h"

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
	GameCamera = Camera(glm::vec3((24 / 2) - 0.5f, 21.0f, 11.5f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -40.0f);
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

void Game::Render() {
	Levels[CurrentLevel].Draw(GameCamera.GetViewMatrix(), lightSource);
}