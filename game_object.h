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
  glm::vec3 Position, Size, Color;

  GLfloat Rotation;
  GLboolean IsAlive;

  GameObject();
  GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 color, GLfloat rotation);

  // light is optional
  void Draw(Renderer &renderer, glm::mat4 camera, const LightSource *lightSource=nullptr);
};

#endif
