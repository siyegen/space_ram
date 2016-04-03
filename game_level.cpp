#include "game_level.h"

GameLevel::GameLevel(std::string name, const std::string &level, GLuint width, GLuint height, Renderer &firstRenderer, Renderer &effectRenderer) {
	LevelName = name;
	Width = width;
	Height = height;
	int numberOfCubes = Width*Height;
	CenterPoint = glm::vec3((Width/2)-0.5f, 0.5f, (Height/2-0.5f));

	glm::vec3 normalColor(0.31f, 1.0f, 0.31f);
	glm::vec3 waterColor(0.21f, 0.51f, 0.9f);
	for (int i = 0, j = 0; i < numberOfCubes; i++) {
		glm::vec3 *colorPointer = &normalColor;
		GLfloat x = 1.0f * (i%Width);
		if (i != 0 && i%Width == 0) j++;
		GLfloat z = -1.0f * j;

		if (i >= numberOfCubes / 2) colorPointer = &waterColor;
		GameObject obj(glm::vec3(x, 0.0f, z), glm::vec3(), *colorPointer, 0.0f);
		LevelCubes.push_back(Cube{firstRenderer, effectRenderer, obj, CubeState::Normal});
	}
}

GameLevel::GameLevel(std::string name) {
}

void GameLevel::Draw(glm::mat4 camera, LightSource *lightSource) {
	for (auto &cube : LevelCubes) {
		cube.cubeObj.Draw(cube.mainRenderer, camera, lightSource);
		cube.cubeObj.Draw(cube.outlineRenderer, camera, lightSource);
	}
}
