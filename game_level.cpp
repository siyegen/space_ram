#include "game_level.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

GameLevel::GameLevel(std::string name, const GLchar *file, GLuint width, GLuint height, Renderer *firstRenderer, Renderer *effectRenderer) {
	LevelName = name;
	Width = width;
	Height = height;
	int numberOfCubes = Width*Height;
	srand(static_cast <unsigned> (time(0)));
	CenterPoint = glm::vec3((Width/2)-0.5f, 0.5f, (Height/2-0.5f));


	LevelCubes.reserve(numberOfCubes);
	fromFile(file, width, height, firstRenderer, effectRenderer);
}

void GameLevel::Draw(glm::mat4 camera, LightSource *lightSource) {
	for (auto &cube : LevelCubes) {
		cube.cubeObj.Draw(cube.mainRenderer, camera, lightSource);
		cube.cubeObj.Draw(cube.outlineRenderer, camera, lightSource, &cube.outlineColor);
	}

	for (auto &cube : Turrets) {
		cube.cubeObj.Draw(cube.mainRenderer, camera, lightSource, nullptr, false);
		cube.cubeObj.Draw(cube.outlineRenderer, camera, lightSource, &cube.outlineColor, false);
	}
}

void GameLevel::fromFile(const GLchar *file, GLuint width, GLuint height, Renderer *first, Renderer *effect) {
	LevelCubes.clear();
	std::string line;
	std::ifstream levelFile;
	// Apparently don't set failbit as getline reading an empty newline
	// raises both an eof and failbit and crashes...
	levelFile.exceptions(std::ifstream::badbit);

	GLuint i = 0, j = 0;
	glm::vec3 normalColor(0.31f, 1.0f, 0.31f);
	glm::vec3 waterColor(0.21f, 0.51f, 0.9f);
	glm::vec4 normalOutline(0.2f, 0.7f, 0.2f, 0.7f);
	glm::vec4 waterOutline(0.0f, 0.4f, 0.9f, 0.7f);

	glm::vec3 turretColor(1.0f, 0.31f, 0.31f);
	glm::vec4 turretOutline(0.7f, 0.2f, 0.0f, 0.7f);
	try {
		levelFile.open(file);
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
				case CubeState::Dangerous:
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
				GameObject obj(glm::vec3(x, y, z), glm::vec3(), *colorPointer, 0.0f);
				LevelCubes.push_back(Cube{ *first, *effect, obj, state, glm::vec2(i, j), *outlinePointer });
				if (state == CubeState::Turret) {
					GameObject obj(glm::vec3(x, y+1.0f, z), glm::vec3(), *colorPointer, 0.0f);
					Turrets.push_back(Cube{ *first, *effect, obj, state, glm::vec2(i, j), *outlinePointer });
				}
			}
			j++; i = 0;
		}
	} catch (std::exception e) {
		std::cout << "ERROR::LEVEL::LOAD" << std::string(e.what()) << std::endl;
		exit(-1);
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
		state = CubeState::Dangerous;
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
	if (state == CubeState::Water) {
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
