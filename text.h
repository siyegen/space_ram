#pragma once

#ifndef TEXT_H
#define TEXT_H

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SOIL/SOIL.h>

class Text {
public:
	GLuint ID;
	GLuint VBO, VAO;

	int Width, Height;
	int StartCharacter;
	GLfloat CellSize;
	GLfloat EffectiveCharSize;
	int Rows, Cols;

	Text(std::string fontImg, GLfloat characterSize, GLfloat effectiveCharSize, int startCharacter);
};

#endif
