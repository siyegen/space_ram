#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>

#include <glm/glm.hpp>

#include <renderer>

class GameObject {
public:
  glm::vec3 position, size, color;

  // Maybe make 2-pass renderer and use base class?
  std::vector<*Renderer> renderers;


  // light is optional
  void Draw(glm::vec3* lightPos=nullptr, glm::vec3* lightColor=nullptr);
}

#endif

// vector of renderer, order matters
// Or, base renderer and effect renderer?
// color
// position, size


// other state here?
// constructor and destructor

// draw
