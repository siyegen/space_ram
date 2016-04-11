#include "game.h"

#include <iostream>
#include <irrklang/irrKlang.h>


Renderer *cubeRenderer;
Renderer *outlineRenderer;
LightSource *lightSource;
LightSource *cannonballLight;
TextRenderer *textRenderer;
Text *hudFont;
GameLevel *demoLevel;

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
	delete demoLevel;
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
	cannonballLight = new LightSource{
		glm::vec3((24 / 2) - 0.5f, 30.0f, -10.0f),
		glm::vec3(1.0f, 0.2f, 0.0f),
	};

	SoundManager::LoadSound("menu_up", "audio/menu_up.wav");
	SoundManager::LoadSound("menu_down", "audio/menu_down.wav");
	SoundManager::LoadSound("menu_select", "audio/menu_select.wav");
	SoundManager::LoadSound("cannon_fire", "audio/cannon_fire.wav");
	SoundManager::LoadSound("explode", "audio/explode.wav");

	Shader testCube = ResourceManager::LoadShader("testCube", "shaders/simple3d.vs", "shaders/diffuse_only.frag");
	Shader outlineCube = ResourceManager::LoadShader("outlineCube", "shaders/outline.vs", "shaders/outline.frag", "shaders/outline.gs");
	Shader textShader = ResourceManager::LoadShader("texture", "shaders/text.vs", "shaders/text.frag");

	cubeRenderer = new Renderer(testCube, vertices);
	outlineRenderer = new Renderer(outlineCube, vertices);
	textRenderer = new TextRenderer(HUD, textShader);

	hudFont = new Text("imgs/minecraft_font.bmp", 32.0f, 16.0f, 32);

	testCube.Use().SetMatrix4("projection", projection);
	outlineCube.Use().SetMatrix4("projection", projection);

	GameLevel testLevel1("Simple and Easy", "levels/level_one.txt", 24, 30, cubeRenderer, outlineRenderer);
	GameLevel testLevel2("More Targets", "levels/level_two.txt", 24, 30, cubeRenderer, outlineRenderer);
	demoLevel = new GameLevel("Demo", "levels/demo.txt", 24, 30, cubeRenderer, outlineRenderer);
	Levels.push_back(testLevel1);
	Levels.push_back(testLevel2);

	// Build level select HUD
	int i = 1;
	for (auto &level : Levels) {
		std::string display = "Level" + std::to_string(i++);
		LevelSelectHud ll(glm::vec2(0.0f), display, 32.0f, false);
		LevelSelect.push_back(ll);
	}

	// Ready cannon balls
	// look to adjust number if we reclaim 'active' balls often
	Cannon = new CannonBallGenerator(cubeRenderer, 250);
}

void Game::Update(GLfloat dt) {
	GameLevel &level = Levels[CurrentLevel];
	if (State == GameState::ACTIVE) {
		if (level.NumberKilled == level.Enemies.size()) {
			// switch to rotating around screen on win
			State = GameState::WIN;
			LevelSelect[CurrentLevel].Beaten = true;
			return;
		}
		if (level.HasTarget) {
			// find angle from turret to target, rotate towards
			glm::vec2 target(level.Target.x, level.Target.y);
			// adjust for origin position, move to render function
			for (auto &turret : level.Turrets) {
				glm::vec2 turretVec(turret.CubeObj.Position.x + 0.5f, turret.CubeObj.Position.z - 0.5f);
				glm::vec2 facing = turretVec - target;
				GLfloat angle = glm::atan(facing.y, facing.x);
				turret.CubeObj.Rotation = glm::degrees(-angle);
			}
		}
		Cannon->Update(dt);
		CheckHit();
	} else if (State == GameState::MENU) {
		LevelSelectHud &hud = LevelSelect[CurrentSelection];
		SelectCursor = hud.Position;
	}
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
				SoundManager::PlaySound("explode", 0.2f);
				std::cout << "Boom, hit!" << std::endl;
			}
		}
	}
	return false;
}

void Game::Render() {
	if (State == GameState::MENU) {
		demoLevel->Draw(GameCamera.GetViewMatrix(), lightSource);
		glm::vec2 offset = textRenderer->DrawText("SpaceRam", *hudFont, glm::vec2(Width, Height / 4), 256.0f, glm::vec4(1.0f, 0.21f, 0.21f, 1.0f), true);
		offset = textRenderer->DrawText("Press Space To Start", *hudFont, glm::vec2(Width, offset.y + 128.0f), 64.0f, glm::vec4(1.0f), true);
		offset.y += 25.0f;
		for (auto &level : LevelSelect) {
			offset = textRenderer->DrawText(level.Display, *hudFont, glm::vec2(Width, offset.y + 20.0f), level.Size, glm::vec4(1.0f), true);
			if (level.Beaten) {
				textRenderer->DrawText("(Completed)", *hudFont, glm::vec2(offset.x, offset.y), level.Size, glm::vec4(0.7f, 0.7f, 0.1f, 1.0f));
			}
			level.Position.y = offset.y;
			level.Position.x = offset.x - textRenderer->DimensionsOfText(level.Display, *hudFont, level.Size).x - textRenderer->DimensionsOfText(">>", *hudFont, level.Size).x;
			
		}
		textRenderer->DrawText(">> ", *hudFont, SelectCursor, 32.0f, glm::vec4(1.0f, 1.0f, 0.6f, 1.0f));
	} else if (State == GameState::ACTIVE) {
		GameLevel &level = Levels[CurrentLevel];
		level.Draw(GameCamera.GetViewMatrix(), lightSource);
		Cannon->Draw(GameCamera.GetViewMatrix(), cannonballLight);
		textRenderer->DrawText(level.LevelName, *hudFont, glm::vec2(Width, Height-32.0f), 64.0f, glm::vec4(1.0f), true);
		glm::vec2 offset = textRenderer->DrawText("Enemies left", *hudFont, glm::vec2(5.0f, 5.0f), 48.0f, glm::vec4(1.0f));
		textRenderer->DrawText(std::to_string(level.Enemies.size() - level.NumberKilled), *hudFont, glm::vec2(offset.x+10.0f, 5.0f), 48.0f, glm::vec4(1.0, 0.21f, 0.1f, 1.0f));
	} else if (State == GameState::WIN) {
		Levels[CurrentLevel].Draw(GameCamera.GetViewMatrix(), lightSource);
		textRenderer->DrawText("You Win", *hudFont, glm::vec2(Width, Height / 2), 128.0f, glm::vec4(1.0f), true);
		textRenderer->DrawText("Press Space To Go to the Menu", *hudFont, glm::vec2(Width, (Height / 2) + 128.0f), 32.0f, glm::vec4(1.0f), true);
	}
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
	} else if (State == GameState::MENU) {
		if (Keys[GLFW_KEY_W]) {
			CurrentSelection = (CurrentSelection + 1) % LevelSelect.size();
			SoundManager::PlaySound("menu_up", 0.6f);
			Keys[GLFW_KEY_W] = GL_FALSE;
		} else if (Keys[GLFW_KEY_S]) {
			CurrentSelection = (CurrentSelection - 1) % LevelSelect.size();
			SoundManager::PlaySound("menu_down", 0.6f);
			Keys[GLFW_KEY_S] = GL_FALSE;
		}
		if (Keys[GLFW_KEY_SPACE]) {
			SoundManager::PlaySound("menu_select", 0.6f);
			StartLevel(CurrentSelection);
			Keys[GLFW_KEY_SPACE] = GL_FALSE;
		}
	} else if (State == GameState::WIN) {
		if (Keys[GLFW_KEY_SPACE]) {
			State = GameState::MENU;
			Keys[GLFW_KEY_SPACE] = GL_FALSE;
		}
	}
}

void Game::HandleClick(GLuint button, double xPos, double yPos) {
	if (State == GameState::ACTIVE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			GameLevel &level = Levels[CurrentLevel];
			glm::vec3 world = screenToWorld(xPos, yPos);
			// XXX: Check y, if it's greater than the highest level point
			// then return as it's clicking on a bullet/ball
			if (world.y >= 2.0f) {
				return;
			}
			glm::vec2 levelXY = glm::vec2(world.x, world.z);
			Cube *cubeTarget = level.CubeFromPosition(levelXY);
			if (cubeTarget && (cubeTarget->State != CubeState::Turret || cubeTarget->State != CubeState::Enemy)) {
				Cube turret = level.Turrets[FiringFrom++%level.Turrets.size()];
				SoundManager::PlaySound("cannon_fire", 0.1f);
				Cannon->Fire(1, turret.CubeObj.Position, turret.CubeObj.Rotation, world);
			}
		}
	}
}

void Game::MoveCursor(double xPos, double yPos) {
	if (State == GameState::ACTIVE) {
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

void Game::StartLevel(int level) {
	CurrentLevel = level;
	Levels[CurrentLevel].ResetLevel();
	Cannon->ResetAll();
	State = GameState::ACTIVE;
}
