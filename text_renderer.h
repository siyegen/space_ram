#pragma once
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>

#include "shader.h"
#include "text.h"

class TextRenderer {
public:
	glm::mat4 Projection;
	Shader TextShader;

	TextRenderer(glm::mat4 projection, Shader textShader);

	// Returns last position drawn at
	glm::vec2 DrawText(std::string toRender, Text text, glm::vec2 position, GLfloat size, glm::vec4 color, bool center=false);
	// Used to calculate positions without rendering them.
	glm::vec2 DimensionsOfText(std::string toRender, Text textFont, GLfloat size);
};

#endif
