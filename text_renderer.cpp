#include "text_renderer.h"

#include <iostream>
#include <ctype.h>


TextRenderer::TextRenderer(glm::mat4 projection, Shader textShader) {
	Projection = projection;
	TextShader = textShader;
}

glm::vec2 TextRenderer::DrawText(std::string toRender, Text textFont, glm::vec2 position, GLfloat size, glm::vec4 color, bool center) {
	TextShader.Use();
	TextShader.SetMatrix4("projection", Projection);
	TextShader.SetVector2f("imgSize", textFont.Width, textFont.Height);
	TextShader.SetVector2f("size", textFont.CellSize, textFont.CellSize);

	

	GLfloat i = 0;
	GLfloat offset = textFont.EffectiveCharSize * (size / textFont.CellSize);// size / textFont.CharacterSize;
	if (center) {
		GLfloat width = toRender.length() * offset;
		position.x = position.x/2 - width / 2;
	}

	glDepthMask(GL_FALSE);
	glClear(GL_DEPTH_BUFFER_BIT);
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

		TextShader.SetVector2f("offset", textFont.CellSize * xOff, textFont.CellSize * yOff);
		TextShader.SetVector4f("textColor", color);

		//glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(textFont.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		i += offset;
	}

	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	return glm::vec2(i, position.y);
}
