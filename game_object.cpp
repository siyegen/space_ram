#include "game_object.h"

#include <iostream>
GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 color, GLfloat rotation) : IsAlive(GL_TRUE) {
	Position = pos;
	Size = size;
	Color = color;
}

void GameObject::Draw(Renderer &renderer, glm::mat4 camera, const LightSource *lightSource, const glm::vec4 *outlineColor, bool skipRot) {
	Shader currentShader = renderer.shader.Use();
	if (lightSource) {
		currentShader.SetVector3f("lightColor", lightSource->lightColor);
		currentShader.SetVector3f("lightPos", lightSource->lightPos);
	}
	currentShader.SetVector3f("objectColor", Color);
	if (outlineColor) {
		currentShader.SetVector4f("outlineColor", *outlineColor);
	}
	if (skipRot) {
		renderer.Draw(camera, Position);
	} else {
		renderer.Draw(camera, Position, &Rotation);
	}
}


