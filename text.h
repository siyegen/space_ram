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

	Text(std::string name);
};

#endif
