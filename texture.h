#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include "shader.h"

class TempTexture {
public:
	GLuint Texture;
	Shader TextureShader;

	GLuint VBO, VAO;

	void LoadImage(std::string name, Shader shader);
	void Draw(glm::mat4 projection, glm::mat4 view);
};

#endif
