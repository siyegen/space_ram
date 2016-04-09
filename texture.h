#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include "shader.h"

class TempTexture {
public:
	GLuint Texture;
	Shader TextureShader;

	GLuint VBO, VAO;

	void LoadImage(const GLchar *name, Shader shader);
	void Draw(glm::mat4 projection);
};

#endif
