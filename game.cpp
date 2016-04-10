#include "game.h"

#include <iostream>

Renderer *cubeRenderer;
Renderer *outlineRenderer;
LightSource *lightSource;
LightSource *cannonballLight;
TextRenderer *textRenderer;
Text *hudFont;

Game::Game(GLuint width, GLuint height)
	: State(GameState::MENU), Keys(), Width(width), Height(height) {
}

Game::~Game() {
	delete cubeRenderer;
	delete outlineRenderer;
	delete lightSource;
	delete cannonballLight;
	delete Cannon;
	delete textRenderer;
	delete hudFont;
}

void Game::Init() {
	// Camera Init
	GameCamera = Camera(glm::vec3((24 / 2), 21.0f, 11.5f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -40.0f);

	//GameCamera = Camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	glm::mat4 projection = glm::perspective(GameCamera.Zoom, (GLfloat)(Width / Height), 0.1f, 100.0f);
	glm::mat4 HUD = glm::ortho(0.0f, (GLfloat)Width, (GLfloat)Height, 0.0f, -1.0f, 1.0f);
	
	// One cube to rule them all
	std::vector<GLfloat> vertices{
		// Position			//Normal
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};

	// Need to adjust for level
	lightSource = new LightSource{
		glm::vec3((24 / 2) - 0.5f, 10.0f, -15.0f),
		glm::vec3(0.8f, 0.5f, 1.0f),
	};
	// Need to adjust for level
	cannonballLight = new LightSource{
		glm::vec3((24 / 2) - 0.5f, 30.0f, -10.0f),
		glm::vec3(1.0f, 0.2f, 0.0f),
	};

	Shader testCube = ResourceManager::LoadShader("testCube", "shaders/simple3d.vs", "shaders/diffuse_only.frag");
	Shader outlineCube = ResourceManager::LoadShader("outlineCube", "shaders/outline.vs", "shaders/outline.frag", "shaders/outline.gs");
	Shader textShader = ResourceManager::LoadShader("texture", "shaders/text.vs", "shaders/text.frag");

	cubeRenderer = new Renderer(testCube, vertices);
	outlineRenderer = new Renderer(outlineCube, vertices);
	textRenderer = new TextRenderer(HUD, textShader);

	hudFont = new Text("imgs/minecraft_font.bmp", 32.0f, 32);

	testCube.Use().SetMatrix4("projection", projection);
	outlineCube.Use().SetMatrix4("projection", projection);

	GameLevel testLevel1("Simple and Easy", "levels/level_one.txt", 24, 30, cubeRenderer, outlineRenderer);
	GameLevel testLevel2("More Targets", "levels/level_two.txt", 24, 30, cubeRenderer, outlineRenderer);
	Levels.push_back(testLevel1);
	Levels.push_back(testLevel2);
	CurrentLevel = 1;

	// Ready cannon balls
	// look to adjust number if we reclaim 'active' balls often
	Cannon = new CannonBallGenerator(cubeRenderer, 250);
}

void Game::Update(GLfloat dt) {
	GameLevel &level = Levels[CurrentLevel];
	if (level.NumberKilled == level.Enemies.size()) {
		std::cout << "You win!" << std::endl;
		// switch to rotating around screen on win
		return;
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
			if (Cannon->CheckCollision(enemy.CubeObj)) {
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
	glDepthMask(GL_FALSE);
	glClear(GL_DEPTH_BUFFER_BIT);
	textRenderer->DrawText("Look At", *hudFont, glm::vec2(600.0f, 100.0f), 128.0f, glm::vec4(1.0f));
	textRenderer->DrawText("My Text", *hudFont, glm::vec2(600.0f, 164.0f), 128.0f, glm::vec4(1.0f, 0.5f, 0.5, 1.0f));
	textRenderer->DrawText("Woo", *hudFont, glm::vec2(600.0f, 228.0f), 128.0f, glm::vec4(0.5f, 0.7, 0.3f, 1.0f));
	glDepthMask(GL_TRUE);
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
		// XXX: Check y, if it's greater than the highest level point
		// then return as it's clicking on a bullet/ball
		if (world.y >= 2.0f ) {
			return;
		}
		glm::vec2 levelXY = glm::vec2(world.x, world.z);
		Cube *cubeTarget = level.CubeFromPosition(levelXY);
		if (cubeTarget && (cubeTarget->State != CubeState::Turret || cubeTarget->State != CubeState::Enemy)) {
			Cube turret = level.Turrets[FiringFrom++%level.Turrets.size()];
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
