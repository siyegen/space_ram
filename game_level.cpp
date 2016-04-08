#include "game_level.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

GameLevel::GameLevel(std::string name, const GLchar *file, GLuint width, GLuint height, Renderer *firstRenderer, Renderer *effectRenderer) {
	LevelName = name;
	Width = width;
	Height = height;
	ExpectedNumberOfCubes = Width*Height;
	srand(static_cast <unsigned> (time(0)));
	CenterPoint = glm::vec3((Width/2)-0.5f, 0.5f, (Height/2-0.5f));

	LevelCubes.reserve(ExpectedNumberOfCubes);
	fromFile(file, width, height, firstRenderer, effectRenderer);
}

void GameLevel::Draw(glm::mat4 camera, LightSource *lightSource) {
	for (auto &cube : LevelCubes) {
		cube.CubeObj.Draw(cube.MainRenderer, camera, lightSource);
		cube.CubeObj.Draw(cube.OutlineRenderer, camera, lightSource, &cube.OutlineColor);
	}

	for (auto &cube : Turrets) {
		cube.CubeObj.Draw(cube.MainRenderer, camera, lightSource, nullptr, false);
		cube.CubeObj.Draw(cube.OutlineRenderer, camera, lightSource, &cube.OutlineColor, false);
	}

	for (auto &cube : Enemies) {
		if (cube.IsAlive) {
			cube.CubeObj.Draw(cube.MainRenderer, camera, lightSource, nullptr, false);
			cube.CubeObj.Draw(cube.OutlineRenderer, camera, lightSource, &cube.OutlineColor, false);
		}
	}
}

Cube* GameLevel::CubeFromPosition(glm::vec2 position) {
	int xCoord = (int)position.x;
	int yCoord = (int)position.y*-1; // Adjust for -z going out from camera
	// Since x and y are multiplied together to get the
	// index in the vector negative numbers aren't used.
	if (xCoord < 0 || yCoord < 0) {
		return nullptr;
	}

	int index = (yCoord * Width) + xCoord;
	if (index >= 0 && index < LevelCubes.size()) {
		return &LevelCubes[index];
	}
	return nullptr;
}

void GameLevel::fromFile(const GLchar *file, GLuint width, GLuint height, Renderer *first, Renderer *effect) {
	LevelCubes.clear();
	std::string line;
	std::ifstream levelFile;
	// Apparently don't set failbit as getline reading an empty newline at eof
	// raises both an eof and failbit and crashes...
	levelFile.exceptions(std::ifstream::badbit);

	GLuint i = 0, j = 0;
	glm::vec3 normalColor(0.31f, 1.0f, 0.31f);
	glm::vec3 waterColor(0.21f, 0.51f, 0.9f);
	glm::vec4 normalOutline(0.2f, 0.7f, 0.2f, 0.7f);
	glm::vec4 waterOutline(0.0f, 0.4f, 0.9f, 0.7f);

	glm::vec3 turretColor(1.0f, 0.31f, 0.31f);
	glm::vec4 turretOutline(0.7f, 0.2f, 0.0f, 0.7f);

	glm::vec3 enemyColor(0.31f, 0.31f, 0.31f);
	glm::vec4 enemyOutline(1.0f, 0.2f, 0.0f, 0.7f);
	try {
		levelFile.open(file);
		if (!levelFile) {
			throw std::runtime_error("Can't find file");
		}
		while (std::getline(levelFile, line)) {
			// init default colors
			const glm::vec3 *colorPointer = &normalColor;
			const glm::vec4 *outlinePointer = &normalOutline;

			// Build tile based on each character
			for (auto &tile : line) {
				int t = tile - '0'; // ascii code to int, has to be a better way
				CubeState state = GameLevel::getState(t);
				GLfloat x = 1.0f * i++;
				GLfloat z = -1.0f * j;
				GLfloat y = GameLevel::getHeight(state, -0.1f, 0.65f);

				switch (state) {
				case CubeState::Normal:
					colorPointer = GameLevel::randomGroundColor();
					outlinePointer = &normalOutline;
					break;
				case CubeState::Enemy:
					colorPointer = &normalColor;
					outlinePointer = &normalOutline;
					break;
				case CubeState::Water:
					colorPointer = &waterColor;
					outlinePointer = &waterOutline;
					break;
				case CubeState::Turret:
					colorPointer = &turretColor;
					outlinePointer = &turretOutline;
					break;
				default:
					colorPointer = &normalColor;
					outlinePointer = &normalOutline;
					break;
				}
				GameObject obj(glm::vec3(x, y, z), 1.0f, *colorPointer, 0.0f);
				LevelCubes.push_back(Cube{ *first, *effect, obj, state, glm::vec2(i, j), *outlinePointer });
				if (state == CubeState::Turret) {
					GameObject obj(glm::vec3(x, y+1.0f, z), 1.0f, *colorPointer, 0.0f);
					Turrets.push_back(Cube{ *first, *effect, obj, state, glm::vec2(i, j), *outlinePointer });
				} else if (state == CubeState::Enemy) {
					GameObject obj(glm::vec3(x, y + 1.0f, z), 1.0f, enemyColor, 0.0f);
					Enemies.push_back(Cube{ *first, *effect, obj, state, glm::vec2(i, j), enemyOutline, true });
				}
			}
			j++; i = 0;
		}
	} catch (std::exception e) {
		std::cout << "ERROR::LEVEL::LOAD " << std::string(e.what()) << std::endl;
	}
	if (LevelCubes.size() != ExpectedNumberOfCubes) {
		std::cout << "WARN::LEVEL::LOAD level size and file mismatch" << std::endl;
	}
	if (j != height) {
		std::cout << "WARN::LEVEL::LOAD level height and file mismatch" << std::endl;
	}
	levelFile.close();
}

CubeState GameLevel::getState(GLuint tile) {
	CubeState state = CubeState::Normal;
	switch (tile) {
	case 1:
		state = CubeState::Normal;
		break;
	case 2:
		state = CubeState::Water;
		break;
	case 3:
		state = CubeState::Enemy;
		break;
	case 4:
		state = CubeState::Turret;
		break;
	default:
		break;
	}
	return state;
}

GLfloat GameLevel::getHeight(CubeState state, GLfloat minVal, GLfloat maxVal) {
	if (state == CubeState::Water || state == CubeState::Enemy) {
		return 0.0f;
	} else if (state == CubeState::Turret) {
		return 0.7f;
	}

	GLfloat y = minVal + (rand() / (RAND_MAX / (maxVal - minVal)));
	return ((int)(y *100.0f)) / 100.0f; // 2 dec places
}

const glm::vec3* GameLevel::randomGroundColor() {
	GLint cc = rand() / (RAND_MAX / (NumberOfGroundColors));
	return &GroundColors[cc];
}
