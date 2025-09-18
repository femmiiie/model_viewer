#ifndef STRUCT_H_
#define STRUCT_H_

#include <glm/glm.hpp>
#include <vector>

struct vertex
{
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec3 normal;
};

struct light
{
  light(glm::vec3 pos, glm::vec3 color)
  {
    this->pos = pos;
    this->color = color;
  }
  
  glm::vec3 pos;
  glm::vec3 color;
};

#endif