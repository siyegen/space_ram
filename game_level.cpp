#include "game_level.h"
#include <iostream>

GameLevel::GameLevel(std::string name, const GLchar *file, GLuint width, GLuint height, Renderer &firstRenderer, Renderer &effectRenderer) {
	LevelName = name;
	Width = width;
	Height = height;
	int numberOfCubes = Width*Height;
	CenterPoint = glm::vec3((Width/2)-0.5f, 0.5f, (Height/2-0.5f));

	LevelCubes.reserve(numberOfCubes);
	fromFile(file, width, height, firstRenderer, effectRenderer);
}

void GameLevel::Draw(glm::mat4 camera, LightSource *lightSource) {
	for (auto &cube : LevelCubes) {
		cube.cubeObj.Draw(cube.mainRenderer, camera, lightSource);
		cube.cubeObj.Draw(cube.outlineRenderer, camera, lightSource);
	}
}

void GameLevel::fromFile(const GLchar *file, GLuint width, GLuint height, Renderer &first, Renderer &effect) {
	LevelCubes.clear();
	std::string line;
	std::ifstream levelFile;
	// Apparently don't set failbit as getline reading an empty newline
	// raises both an eof and failbit and crashes...
	levelFile.exceptions(std::ifstream::badbit);

	GLuint i = 0, j = 0;
	glm::vec3 normalColor(0.31f, 1.0f, 0.31f);
	glm::vec3 waterColor(0.21f, 0.51f, 0.9f);
	try {
		levelFile.open(file);
		while (std::getline(levelFile, line)) {
			glm::vec3 *colorPointer = &normalColor;
			if (line.empty()) {
				std::cout << "empty line" << std::endl;
				continue;
			}
			std::cout << line.size() << std::endl;
			for (auto &tile : line) { // char by char
				int t = tile - '0';
				GLfloat x = 1.0f * i++;
				GLfloat z = -1.0f * j;
				CubeState state = GameLevel::getState(t);
				if (state == CubeState::Normal) {
					colorPointer = &normalColor;
				} else if (state == CubeState::Water) {
					colorPointer = &waterColor;
				} else {
					colorPointer = &normalColor;
				}
				GameObject obj(glm::vec3(x, 0.0f, z), glm::vec3(), *colorPointer, 0.0f);
				LevelCubes.push_back(Cube{ first, effect, obj, state, glm::vec2(i, j) });
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
	default:
		break;
	}
	return state;
}
