#include "text_renderer.h"

#include <iostream>
#include <ctype.h>


TextRenderer::TextRenderer(glm::mat4 projection, Shader textShader) {
	Projection = projection;
	TextShader = textShader;
}

void TextRenderer::DrawText(std::string toRender, Text textFont, glm::vec2 position, GLfloat size, glm::vec4 color) {
	TextShader.Use();
	TextShader.SetMatrix4("projection", Projection);
	TextShader.SetVector2f("imgSize", textFont.Width, textFont.Height);
	TextShader.SetVector2f("size", textFont.CharacterSize, textFont.CharacterSize);

	GLfloat i = 0;
	GLfloat spacing = 18.0f;
	glBindTexture(GL_TEXTURE_2D, textFont.ID);
	for (auto &letter : toRender) {
		glm::mat4 model;
		// Move to starting point
		int c = (int)toupper(letter) - textFont.StartCharacter;
		GLuint xOff = c % textFont.Rows;
		GLuint yOff = c / textFont.Cols;
		model = glm::translate(model, glm::vec3(position.x + i, position.y, 0.0f));
		// Scale size of image we will draw
		model = glm::scale(model, glm::vec3(size, size, 1.0f));
		TextShader.SetMatrix4("model", model);

		TextShader.SetVector2f("offset", textFont.CharacterSize * xOff, textFont.CharacterSize * yOff);
		TextShader.SetVector4f("textColor", color);

		//glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(textFont.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		i += 32.0f + spacing;
	}

	glBindVertexArray(0);
}
