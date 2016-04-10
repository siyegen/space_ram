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
	GLfloat CharacterSize;
	int Rows, Cols;

	Text(std::string fontImg, GLfloat characterSize, int s);
};

#endif
