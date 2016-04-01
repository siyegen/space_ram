#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"


class Renderer {
public:
	Renderer(Shader &shader, const GLfloat* verticies[]);
	~Renderer();

	void Draw(glm::vec3 position, glm::mat4 view, glm::mat4 projection);
private:
	// Render state
	Shader shader;
	GLuint VAO, VBO;
	// Other object, cube/etc will own this and be responsible for 
	// cleaning up when it's done
	GLfloat* vertices[];

	void initRenderData();
};

#endif