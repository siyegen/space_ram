#include "texture_renderer.h"

#include <iostream>
#include <ctype.h>


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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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
	model = glm::translate(model, glm::vec3(100.0f, 100.0f, 0.0f));
	model = glm::scale(model, glm::vec3(128.0f, 128.0f, 1.0f));
	TextureShader.SetMatrix4("projection", projection);
	TextureShader.SetMatrix4("model", model);
	TextureShader.SetVector2f("imgSize", 256.0f, 256.0f);
	TextureShader.SetVector2f("size", 32.0f, 32.0f);
	TextureShader.SetVector2f("offset", 32.0f*4, (32.0f*4));
	TextureShader.SetVector4f("textColor", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void TextureRenderer::DrawWord(std::string text, glm::mat4 projection, glm::mat4 view) {
	TextureShader.Use();
	TextureShader.SetMatrix4("projection", projection);

	GLfloat i = 0;
	GLfloat spacing = 18.0f;
	glBindTexture(GL_TEXTURE_2D, Texture);
	for (auto &letter : text) {
		glm::mat4 model;
		// Move to starting point
		int c = (int)toupper(letter) - 32;
		GLuint xOff = c % 8; // 8 is number of rows
		GLuint yOff = c / 8;
		model = glm::translate(model, glm::vec3(100.0f+i, 100.0f, 0.0f));
		// Scale size of image we will draw
		model = glm::scale(model, glm::vec3(128.0f, 128.0f, 1.0f));
		TextureShader.SetMatrix4("model", model);

		TextureShader.SetVector2f("imgSize", 256.0f, 256.0f);
		TextureShader.SetVector2f("size", 32.0f, 32.0f);
		TextureShader.SetVector2f("offset", 32.0f * xOff, 32.0f * yOff);
		TextureShader.SetVector4f("textColor", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

		//glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		i += 32.0f + spacing;
	}

	glBindVertexArray(0);
}
