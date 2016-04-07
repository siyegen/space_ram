#include "game_object.h"


GameObject::GameObject() : Position(), Scale(1.0f), Color(1.0f, 0.5f, 0.0f), Rotation() {
}

GameObject::GameObject(glm::vec3 pos, GLfloat scale, glm::vec3 color, GLfloat rotation) : IsAlive(GL_TRUE) {
	Position = pos;
	Scale = scale;
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
		renderer.Draw(camera, Position, &Scale);
	} else {
		renderer.Draw(camera, Position, &Scale, &Rotation);
	}
}


