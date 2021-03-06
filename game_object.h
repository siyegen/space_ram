#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>

#include <glm/glm.hpp>

#include "renderer.h"

struct LightSource {
	glm::vec3 lightPos;
	glm::vec3 lightColor;
};

class GameObject {
public:
  glm::vec3 Position, Color;

  GLfloat Scale = 1.0f;
  GLfloat Rotation = 0.0f;
  GLboolean IsAlive;

  GameObject();
  GameObject(glm::vec3 pos, GLfloat scale, glm::vec3 color, GLfloat rotation);

  // light is optional
  void Draw(Renderer &renderer, glm::mat4 camera, const LightSource *lightSource=nullptr, const glm::vec4 *outlineColor=nullptr, bool skipRot=true);
};

#endif
