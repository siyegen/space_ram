#include "game_object.h"

GameObject::GameObject() {
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 color, GLfloat rotation) : IsAlive(GL_TRUE) {
	Position = pos;
	Size = size;
	Color = color;
}

void GameObject::Draw(Renderer &renderer, glm::mat4 camera, const LightSource *lightSource) {
	Shader currentShader = renderer.shader.Use();
	if (lightSource) {
		currentShader.SetVector3f("lightColor", lightSource->lightColor);
		currentShader.SetVector3f("lightPos", lightSource->lightPos);
	}
	currentShader.SetVector3f("objectColor", Color);

	renderer.Draw(Position, camera);
}


