#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"


class Renderer {
public:
	Renderer(Shader shader, const std::vector<GLfloat> verticies);
	~Renderer();

	// Uniform scaling only
	void Draw(glm::mat4 view, glm::vec3 position, GLfloat *scale=nullptr, GLfloat *rotation=nullptr);
	// Expose shader to pass uniforms in
	Shader shader;
private:
	GLuint VAO, VBO;
	// Other object, cube/etc will own this and be responsible for
	// cleaning up when it's done
	std::vector<GLfloat> vertices;

	void initRenderData();
};

#endif
