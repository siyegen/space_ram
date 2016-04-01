#include "renderer.h"

Renderer::Renderer(Shader &shader, const GLfloat* vertices[]) {
	shader = shader;
	vertices = vertices;
}

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &VAO);
}

void Renderer::Draw(glm::vec3 position, glm::mat4 view, glm::mat4 projection) {
	glBindVertexArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Renderer::initRenderData() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // Unbind VAO
}