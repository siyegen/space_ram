#include "texture_renderer.h"

#include <iostream>


void TextureRenderer::LoadImage(std::string name, Shader shader) {
	TextureShader = shader;


	std::cout << "text " << Texture << std::endl;
	glGenTextures(1, &Texture);
	std::cout << "text " << Texture << std::endl;

	glBindTexture(GL_TEXTURE_2D, Texture);
	int width, height;
	unsigned char *image = SOIL_load_image(name.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextureRenderer::Draw(glm::mat4 projection, glm::mat4 view) {
	TextureShader.Use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(500.0f, 500.0f, 1.0f));
	TextureShader.SetMatrix4("projection", projection);
	TextureShader.SetMatrix4("model", model);
	TextureShader.SetVector4f("textColor", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
