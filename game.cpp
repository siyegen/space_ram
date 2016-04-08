#include "game.h"

#include <iostream>

Renderer *cubeRenderer;
Renderer *outlineRenderer;
LightSource *lightSource;
LightSource *cannonballLight;

Game::Game(GLuint width, GLuint height)
	: State(GameState::MENU), Keys(), Width(width), Height(height) {

}

Game::~Game() {
	delete cubeRenderer;
	delete outlineRenderer;
	delete lightSource;
	delete cannonballLight;
	delete Cannon;
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
	// Need to adjust for level
	cannonballLight = new LightSource{
		glm::vec3((24 / 2) - 0.5f, 30.0f, -10.0f),
		//glm::vec3((6 / 2), 10.0f, -3.0f),
		glm::vec3(1.0f, 0.2f, 0.0f),
	};

	Shader testCube = ResourceManager::LoadShader("testCube", "shaders/simple3d.vs", "shaders/diffuse_only.frag");
	Shader outlineCube = ResourceManager::LoadShader("outlineCube", "shaders/outline.vs", "shaders/outline.frag", "shaders/outline.gs");
	ResourceManager::LoadShader("Debug", "shaders/outline.vs", "shaders/lamp.frag");

	cubeRenderer = new Renderer(testCube, vertices);
	outlineRenderer = new Renderer(outlineCube, vertices);

	testCube.Use().SetMatrix4("projection", projection);
	outlineCube.Use().SetMatrix4("projection", projection);

	GameLevel testLevel("testLevel", "levels/level_two.txt", 24, 30, cubeRenderer, outlineRenderer);
	Levels.push_back(testLevel);

	// Ready cannon balls
	Cannon = new CannonBallGenerator(cubeRenderer, 100);
	std::cout << "starting angle: " << testLevel.Turrets[0].CubeObj.Rotation << std::endl;
}

void Game::Update(GLfloat dt) {
	GameLevel &level = Levels[CurrentLevel];
	if (level.NumberKilled == level.Enemies.size()) {
		std::cout << "You win!" << std::endl;
	}
	if (level.HasTarget) {
		// find angle from turret to target, rotate towards
		glm::vec2 target(level.Target.x, level.Target.y);
		// adjust for origin position, move to render function
		for (auto &turret : level.Turrets) {
			glm::vec2 turretVec(turret.CubeObj.Position.x+0.5f, turret.CubeObj.Position.z - 0.5f);
			glm::vec2 facing = turretVec - target;
			GLfloat angle = glm::atan(facing.y, facing.x);
			turret.CubeObj.Rotation = glm::degrees(-angle);
		}
	}
	Cannon->Update(dt);
	CheckHit();
}

bool Game::CheckHit() {
	// Loop over all hittable items on the level
	// pass to cannon to check if any valid balls
	// have colided with it.
	GameLevel &level = Levels[CurrentLevel];
	for (auto &enemy : level.Enemies) {
		if (enemy.IsAlive) {
			if (Cannon->CheckCollision(enemy)) {
				enemy.IsAlive = false;
				level.NumberKilled++;
				std::cout << "Boom, hit!" << std::endl;
			}
		}
	}
	return false;
}

void Game::Render() {
	Levels[CurrentLevel].Draw(GameCamera.GetViewMatrix(), lightSource);
	Cannon->Draw(GameCamera.GetViewMatrix(), cannonballLight);
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

void Game::HandleClick(GLuint button, double xPos, double yPos) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		GameLevel &level = Levels[CurrentLevel];
		glm::vec3 world = screenToWorld(xPos, yPos);
		std::cout << "x: " << world.x << " y: " << world.y << " z: " << world.z << std::endl;
		glm::vec2 levelXY = glm::vec2(world.x, world.z);
		Cube *cubeTarget = level.CubeFromPosition(levelXY);
		if (cubeTarget && cubeTarget->State != CubeState::Turret || cubeTarget->State != CubeState::Enemy) {
			cubeTarget->CubeObj.Color = glm::vec3(0.2f, 0.2f, 0.61f);
			Cube turret = level.Turrets[firingFrom++%level.Turrets.size()];
			Cannon->Fire(1, turret.CubeObj.Position, turret.CubeObj.Rotation, world);
		}
	}
}

void Game::MoveCursor(double xPos, double yPos) {
	GameLevel &level = Levels[CurrentLevel];
	glm::vec3 world = screenToWorld(xPos, yPos);
	glm::vec2 levelXY = glm::vec2(world.x, world.z);
	Cube *cubeTarget = level.CubeFromPosition(levelXY);
	if (cubeTarget) {
		level.Target = levelXY;
		level.HasTarget = true;
	} else {
		level.HasTarget = false;
	}
}

glm::vec3 Game::screenToWorld(double xPos, double yPos) {
	glm::mat4 projection = glm::perspective(GameCamera.Zoom, (GLfloat)(Width / Height), 0.1f, 100.0f);
	glm::mat4 modelView = GameCamera.GetViewMatrix()*glm::mat4(0.5f);
	glm::vec4 viewport(0, 0, Width, Height);
	GLfloat winX = (GLfloat)xPos;
	GLfloat winY = (GLfloat)(Height - yPos);
	GLfloat winZ;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	return glm::unProject(glm::vec3(winX, winY, winZ), modelView, projection, viewport);
}
