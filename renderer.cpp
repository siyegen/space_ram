#include "renderer.h"

#include <iostream>

Renderer::Renderer(Shader shader, std::vector<GLfloat> vertices) {

	this->shader = shader;
	this->vertices = vertices;
	initRenderData();
}

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &VAO);
}

void Renderer::Draw(glm::vec3 position, glm::mat4 view, glm::mat4 projection) {
	shader.Use();

	glm::mat4 model;
	model = glm::translate(model, position);

	shader.SetMatrix4("view", view);
	shader.SetMatrix4("projection", projection);
	shader.SetMatrix4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Renderer::initRenderData() {
	std::cout << "size" << this->vertices.size() << std::endl;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // Unbind VAO
}
