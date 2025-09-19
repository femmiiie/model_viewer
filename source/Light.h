#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct Light
{
  Light(glm::vec3 pos, glm::vec3 color) : lightPosition(glm::vec4(pos, 1.0)), lightColor(glm::vec4(color, 1.0)) {};

  glm::vec4 lightPosition;
  glm::vec4 lightColor;
};

#endif